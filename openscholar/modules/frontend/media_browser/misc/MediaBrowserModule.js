(function ($) {
  var rootPath,
    open = angular.noop;

  angular.module('mediaBrowser', ['JSPager', 'EntityService', 'os-auth', 'ngSanitize', 'angularFileUpload', 'angularModalService', 'FileEditor', 'mediaBrowser.filters', 'locationFix', 'angularSlideables'])
    .config(function (){
       rootPath = Drupal.settings.paths.moduleRoot;
    })
    .run(['mbModal', 'FileEditorOpenModal', function (mbModal, feom) {
      // Disable drag and drop behaviors on the window object, to prevent files
      // from.
      angular.element(window).on('dragover drop', function(e) {
        e = e || event;
        e.preventDefault();
      });

      // if the File object is not supported by this browser, fallback to the
      // original media browser.
      if (mbModal.requirementsMet()) {
        Drupal = Drupal || {};
        Drupal.media = Drupal.media || {};
        Drupal.media.popups = Drupal.media.popups || {};
        var oldPopup = Drupal.media.popups.mediaBrowser;
        Drupal.media.popups.mediaBrowserOld = oldPopup;
        Drupal.media.popups.mediaBrowser = function (onSelect, globalOptions, pluginOptions, widgetOptions) {
          var options = Drupal.media.popups.mediaBrowser.getDefaults();
          options.global = $.extend({}, options.global, globalOptions);
          options.plugins = pluginOptions;
          options.widget = $.extend({}, options.widget, widgetOptions);


          // Params to send along to the iframe. WIP.
          var params = {};
          $.extend(params, options.global);
          params.plugins = options.plugins;
          params.onSelect = onSelect;

          mbModal.open(params);
        };

        for (var k in oldPopup) {
          if (!Drupal.media.popups.mediaBrowser[k]) {
            Drupal.media.popups.mediaBrowser[k] = oldPopup[k];
          }
        }

        var oldStyleSelector = Drupal.media.popups.mediaStyleSelector;
        Drupal.media.popups.mediaStyleSelector = function (file, onSelect, options) {
          if (file.type == 'media') {
            feom.open(file.fid, function () {
              onSelect();
            });
          }
          else {
            oldStyleSelector(file, onSelect, options);
          }
        };

        for (var k in oldStyleSelector) {
          Drupal.media.popups.mediaStyleSelector[k] = oldStyleSelector[k];
        }
      }
    }])
  .controller('BrowserCtrl', ['$scope', '$filter', '$http', 'EntityService', 'EntityConfig', '$sce', '$q', '$upload', '$timeout', 'FILEEDITOR_RESPONSES', 'params', 'close', '$location',
      function ($scope, $filter, $http, EntityService, config, $sce, $q, $upload, $timeout, FER, params, close, $location) {

    // Initialization
    var service = new EntityService('files', 'id'),
      toEditForm = false,
      directInsert = true;
    $scope.files = [];
    $scope.numFiles = 0;
    $scope.templatePath = rootPath;
    $scope.selection = 0;
    $scope.form = '';
    $scope.pane = 'upload';
    $scope.toBeUploaded = [];
    $scope.dupes = [];
    $scope.showButtons = false;
    $scope.params = params.browser;
    $scope.editing = false;
    $scope.deleting = false;
    $scope.activePanes = params.browser.panes;
    $scope.activePanes.edit = true;
    $scope.activePanes.delete = true;
    $scope.loading = true;
    $scope.loadingMessage = '';
    $scope.sortType = 'timestamp';
    $scope.sortReverse = true;
    $scope.button_text = params.replace ? 'Select Replacement File' : 'Select files to Add';

    $scope.toInsert = [];

    var allTypes = [
      {label: 'Image', value: 'image'},
      {label: 'Document', value: 'document'},
      {label: 'Video', value: 'video'},
      {label: 'HTML', value: 'html'},
      {label: 'Executable', value: 'executable'},
      {label: 'Audio', value: 'audio'},
      {label: 'Icon', value: 'icon'}
    ];

    var defaultFilteredTypes = params.types;
    $scope.availTypes = [];
    $scope.availFilter = [];
    for (var j in defaultFilteredTypes) {
      for (var k=0; k<allTypes.length; k++) {
        if (defaultFilteredTypes[j] == allTypes[k].value) {
          $scope.availTypes.push(allTypes[k]);
          $scope.availFilter.push(allTypes[k].value);
        }
      }
    }

    $scope.extensions = [];
    if (params.file_extensions) {
      $scope.extensions = params.file_extensions.split(' ');
    }
    if (!params.override_extensions) {
      var types = params.types;
      for (var t in types) {
        var ext = Drupal.settings.extensionMap[types[t]],
          i = 0, l = ext ? ext.length : false;

        if (!ext) {
          $scope.extensions.push(types[t]);
        } else {
          for (var i=0; i<l; i++) {
            if ($scope.extensions.indexOf(ext[i]) === -1) {
              $scope.extensions.push(ext[i]);
            }
          }
        }
      }
    }
    $scope.extensions.sort();
    $scope.whitelist = Drupal.settings.embedWhitelist;

    if (params.max_filesize) {
      $scope.maxFilesize = params.max_filesize;
    }
    else {
      $scope.maxFilesize = Drupal.settings.maximumFileSize;
    }

    $scope.filteredTypes = [];
    $scope.isFiltered = function () {
      return $scope.filteredTypes.length || $scope.search;
    }

    $scope.clearFilters = function () {
      $scope.filteredTypes = defaultFilteredTypes;
      $scope.search = '';
    }

    $scope.showHelp = false;

    if (close) {
      $scope.showButtons = true;
    }

    $scope.messages = {
      next: 0
    };

    // Watch for changes in file list
    $scope.$on('EntityService.files.add', function (event, file) {
      if (file.changed == file.timestamp) {
        $scope.files.push(file);
      }
    });

    $scope.$on('EntityService.files.update', function (event, file) {
      var t = $scope.selected_file;
      for (var i=0; i < $scope.files.length; i++) {
        if ($scope.files[i].id == file.id) {
          if ($scope.files[i].replaced) {
            file.replaced = $scope.files[i].replaced;
          }
          if ($scope.files[i].new) {
            file.new = $scope.files[i].new;
          }
          $scope.files[i] = file;
          break;
        }
      }

      for (i=0; i<$scope.toInsert.length; i++) {
        if ($scope.toInsert[i].id == file.id) {
          $scope.toInsert[i] = file;
        }
      }

      if ($scope.selected_file.id == file.id) {
        $scope.selected_file = angular.copy(file);
      }
    });

    $scope.$on('EntityService.files.delete', function (event, id) {
      // Don't want to worry about what happens when you modify an array you're
      // looping over.
      if ($scope.selected_file.id == id) {
        $scope.selected_file = null;
      }
      var deleteMe = false;
      for (var i=0; i<$scope.files.length; i++) {
        if ($scope.files[i].id == id) {
          deleteMe = i;
          break;
        }
      }

      if (deleteMe !== false) {
        $scope.files.splice(deleteMe, 1);
      }
    })

    var fetching = service.fetch({})
      .then(function (result) {
        console.log(result);

        $scope.files = result;
        $scope.numFiles = $scope.files.length;
        $scope.loading = false;
      }, function (error) {
        // there was an error getting results. We should tell the user and
        // advise them.
      }, function (message) {
        // notification received.
        $scope.loadingMessage = message;
      });


    $scope.changePanes = function (pane, result) {
      if ($scope.activePanes[pane]) {
        $scope.pane = pane;
        return true;
      }
      else {
        close(result != undefined ? result : true);
      }
    }

    $scope.validate = function($file) {
      // Deleting previous error messages
      $scope.removeMsg();
      var file = $file;
      if (file && file instanceof File) {
        // TODO: Get validating properties from somewhere and check the file against them

        var maxFilesize = params.max_filesize_raw || Drupal.settings.maximumFileSizeRaw;
        var size = maxFilesize > file.size,   // file is smaller than max
          ext = file.name.slice(file.name.lastIndexOf('.')+1).toLowerCase(),
          extension = $scope.extensions.indexOf(ext) !== -1,    // extension is found
          id;

        if (!size) {
          addMessage(file.name + ' is larger than the maximum filesize of ' + (params.max_filesize || Drupal.settings.maximumFileSize));
        }
        if (!extension) {
          addMessage(file.name + ' is not an accepted file type.');
        }
        // if file is image and params specify max dimensions
        if (file.type.indexOf('image/') !== -1) {
        }

        return size && extension;
      }
    }

    $scope.removeMsg = function() {
      angular.forEach($scope.messages, function(value, id){
        if (!isNaN(id)) {
          delete $scope.messages[id];
        }
      });
    }

    function addMessage(message) {
      var id = $scope.messages.next++;
      $scope.messages[id] = {
        text: message
      };
      //$timeout(angular.bind($scope, removeMessage, id), 5000);
    }

    function removeMessage(id) {
      delete this.messages[id];
    }


    // filter out weird characters that look like normal characters, like –, which gets converted to â€“
    function cleanPaths(text) {
      var s = text;
      // smart single quotes and apostrophe
      s = s.replace(/[\u2018\u2019\u201A]/g, "\'");
      // smart double quotes
      s = s.replace(/[\u201C\u201D\u201E]/g, "\"");
      // ellipsis
      s = s.replace(/\u2026/g, "...");
      // dashes
      s = s.replace(/[\u2013\u2014]/g, "-");
      // circumflex
      s = s.replace(/\u02C6/g, "^");
      // open angle bracket
      s = s.replace(/\u2039/g, "<");
      // close angle bracket
      s = s.replace(/\u203A/g, ">");
      // spaces
      s = s.replace(/[\u02DC\u00A0]/g, " ");

      return s;
    }

    // looks for any files with a similar basename and extension to this file
    // if it finds any, it adds it to a list of dupes, then scans every file to find what the new name should be
    $scope.checkForDupes = function($files, $event, $rejected) {
      if ($files.length == 1) {
        toEditForm = true;
      }
      var toBeUploaded = [];
      $scope.dupes = [];
      $scope.toInsert = [];
      var promises = [];
      $scope.checkingFilenames = true;
      for (var i = 0; i < $files.length; i++) {
        if (params.replace) {
          if (params.replace.filename == $files[i].name) {
            // the fact that replace is set means we're trying to Replace a certain file
            // If the file in question has the same name as the new one, just skip all the duplicate
            // processing and upload it immediately.
            $files[i].replacing = params.replace;
            toEditForm = false;
            directInsert = true;
            $scope.upload([$files[i]]);
            continue;
          }
        }

        // replace # in filenames cause they will break filename detection
        var newName = $files[i].name.replace(/[#|\?]/g, '_').replace(/__/g, '_').replace(/_\./g, '.');
        var hadHashtag = newName != $files[i].name;
        $files[i].sanitized = newName;

        var url = Drupal.settings.paths.api + '/files/filename/' + $files[i].sanitized;

        if (Drupal.settings.spaces) {
          url += '?vsite=' + Drupal.settings.spaces.id;
        }
        var config = {
          originalFile: $files[i]
        };
        promises.push($http.get(url, config).then(function (response) {
            var file = response.config.originalFile;
            var data = response.data.data;
            file.filename = file.sanitized;
            if (data.collision) {
              file.newName = data.expectedFileName;
              $scope.dupes.push(file);
            }
            else {
              if (data.invalidChars || hadHashtag) {
                addMessage("This file was renamed from \"" + file.name + "\" due to having invalid characters in its name. The new file will replace any file with the same name.");
              }
              toBeUploaded.push(file);
            }
          },
          function (errorResponse) {
            console.log(errorResponse);
          }));
      }

      var promise = $q.all(promises).then(function () {
          $scope.checkingFilenames = false;
          $scope.upload(toBeUploaded);
        },
        function () {
          $scope.checkingFilenames = false;
          console.log('Error happened with all promises');
        })
    }

/*
        var similar = [],
            basename = $files[i].name.replace(/\.[a-zA-Z0-9]*$/, ''),   // remove extension from filename
            extension = $files[i].name.replace(basename, ''),           // remove filename from filename to get extension
            dupeFound = false;

        // rewrite the filename the same way PHP will
        basename = cleanPaths(basename);
        basename = basename.toLowerCase().replace(/ /g, '_').replace(/[^a-zA-Z0-9-_.~]/g, '');
        $files[i].filename = basename + extension;

        for (var j=0; j<$scope.files.length; j++) {
          // find any file with a name that matches "basename{_dd}.ext" and add it to list of similar files
          if ($scope.files[j].filename.indexOf(basename) !== -1 && $scope.files[j].filename.indexOf(extension) !== -1) {
            similar.push($scope.files[j]);
            // also check if there is a file with the full filename and save this fact for later
            // this allows file.jpg to be uploaded when file_01.jpg already exists
            if ($scope.files[j].filename == $files[i].filename) {
              dupeFound = true;
            }
          }
        }

        if (dupeFound) {
          // only one similar file found, drop _01 at the end
          if (similar.length == 1) {
            $files[i].newName = basename + '_01' + extension;
          }
          else {
            // lots of them found, look through all of them, find the highest number at the end, and add it
            // to the end of the original file name
            var max = 0;
            for (j=0; j<similar.length; j++) {
              var rem = similar[j].filename.replace(basename, '').replace(extension, '').replace('_', ''),
                num = rem ? parseInt(rem) : 0;

              if (num > max) {
                max = num;
              };
            }
            var num = max + 1;
            // convert num to a 2 digit string
            num = num < 10 ? '0'+num : num
            // and make the new file name
            $files[i].newName = basename+'_'+num+extension;
          }
          // with the new name complete, we can push this onto the list of dupes
          $scope.dupes.push($files[i]);
        }
        else {
          if ($files[i].filename != $files[i].name) {
            addMessage("This file was renamed from \"" + $files[i].name + "\" due to having invalid characters in its name.")
          }
          // not a dupe, just upload it silently
          toBeUploaded.push($files[i]);
        }
      }

      $scope.upload(toBeUploaded);
    }*/

    // renames the file before uploading
    $scope.rename = function ($index, $last) {
      $scope.dupes[$index].processed = true;

      if ($last) {
        finalizeDupes();
      }
    }

    // tells the server to replace the old file on disk with this new one
    // (just performs a swap on the hard drive)
    $scope.replace = function ($index, $last) {
      $scope.dupes[$index].processed = true;
      delete $scope.dupes[$index].newName;

      if ($last) {
        finalizeDupes();
      }
    }

    // cancels the upload process for this file
    $scope.cancelUpload = function ($index, $last) {
      $scope.dupes[$index].doNotUpload = true;
      $scope.dupes[$index].processed = true;

      if ($last) {
        finalizeDupes();
      }
    }

    function finalizeDupes() {
      var toBeUploaded = [];
      for (var i in $scope.dupes) {
        if (!$scope.dupes[i].doNotUpload) {
          toBeUploaded.push($scope.dupes[i]);
        }
      }

      $scope.upload(toBeUploaded);
      $scope.dupes = [];
    }

    (function () {
      var toBeUploaded = [],
        uploading = false,
        progress = null,
        currentlyUploading = 0;

      $scope.upload = function ($files) {
        for (var i=0; i<$files.length; i++) {
          toBeUploaded.push($files[i]);
        }

        if (!uploading && toBeUploaded.length) {
          uploading = true;
          $file = toBeUploaded[currentlyUploading];
          uploadOne($file);
        }
      }

      function uploadNext(firstId) {
        currentlyUploading++;
        if (currentlyUploading < toBeUploaded.length) {
          $file = toBeUploaded[currentlyUploading];
          uploadOne($file);
        }
        else {
          toBeUploaded = [];
          uploading = false;
          progress = null;
          currentlyUploading = 0;
          if ($scope.dupes.length == 0) {
            if (toEditForm && firstId) {
              // there's only one file, we can assume it's this one
              $scope.setSelection(firstId);
              $scope.changePanes('edit');
            }
            //else if (typeof $scope.messages[$scope.messages.next-1] != 'undefined') {
            //  // do nothing. This usually means there was an error during upload.
            //}
            else {
              if (directInsert) {
                $scope.insert();
              }
              else {
                $scope.changePanes('library');
              }
            }
          }
        }
      }

      function uploadOne($file) {
        var fields = {};
        if (Drupal.settings.spaces) {
          fields.vsite = Drupal.settings.spaces.id;
        }
        if (config.files) {
          for (var k in config.files.fields) {
            fields[k] = config.files.fields[k];
          }
        }
        $upload.upload({
          url: Drupal.settings.paths.api+'/files',
          file: $file,
          data: $file,
          fileFormDataName: 'files[upload]',
          headers: {'Content-Type': $file.type},
          method: 'POST',
          fields: fields,
          fileName: $file.newName || null
        }).progress(function (e) {
          progress = e;
        }).success(function (e) {
          for (var i = 0; i< e.data.length; i++) {
            service.register(e.data[i]);
            var found = false;
            // check to see if this file exists
            for (var j = 0; j < $scope.files.length; j++) {
              if ($scope.files[j].id == e.data[i].id) {
                // we just replaced an existing file.
                e.data[i].replaced = true;
                $scope.files[j] = e.data[i];
                found = true;
              }
            }
            if (!found) {
              // This is a brand-new file. Set the true flag and add it to the list.
              e.data[i].new = true;
              $scope.files.push(e.data[i]);
            }
            $scope.toInsert.push(e.data[i]);
          }
          uploadNext(e.data[0].id);
        }).error(function (e) {
          addMessage(e.title);
          uploadNext();
        });
      }

      $scope.uploadProgress = function () {
        return {
          uploading: uploading,
          filename: uploading ? toBeUploaded[currentlyUploading].filename : '',
          progressBar: (uploading && progress) ? parseInt(100.0 * progress.loaded / progress.total) : 0,
          index: currentlyUploading+1,
          numFiles: toBeUploaded.length
        }
      }
    })();

    function getKeyForFile(fid) {
      for (var i=0; i<$scope.files.length; i++) {
        if ($scope.files[i].id == fid) {
          return i;
        }
      }
      return FALSE;
    }

    // selected file
    $scope.setSelection = function (fid) {
      var key = getKeyForFile(fid);
      if (key !== false) {
        $scope.selection = fid;
        $scope.selected_file = $scope.files[key];
      }
    };

    $scope.deleteConfirmed = function() {
      service.delete($scope.selected_file)
        .then(function (resp) {
        });
      for (var j = 0; j < $scope.files.length; j++) {
        if ($scope.files[j].id == $scope.selected_file.id) {
          $scope.files[j].status = 'deleting';
        }
      }
      $scope.changePanes('library');
    };


    $scope.embed = '';
    $scope.embedSubmit = function () {
      // construct the entity
      var data = {
        embed: this.embed,
      }

      service.add(data).success(function (e) {
        if (e.data.length) {
          $scope.embed = '';
          e.data[0].new = e.data[0].changed == e.data[0].timestamp;
          $scope.setSelection(e.data[0].id);
          service.register(e.data[0]);

          $scope.changePanes('edit')
        }
      })
      .error(function (e) {
        if ($location.protocol() == 'https') {
          $scope.embedFailureHttps = true;
          $timeout(function () {
            $scope.embedFailureHttps = false;
          }, 5000);
        } else {
          $scope.embedFailure = true;
          $timeout(function () {
            $scope.embedFailure = false;
          }, 5000);
        }
      });
    }

    $scope.closeFileEdit = function (result) {
      if (result == FER.CANCELED && $scope.selected_file.new) {
        service.delete($scope.selected_file);
        for (var j = 0; j < $scope.files.length; j++) {
          if ($scope.files[j].id == $scope.selected_file.id) {
            $scope.files[j].status = 'deleting';
          }
        }
        $scope.selected_file = null;
      }
      else if ((result == FER.NO_CHANGES || result == FER.SAVED) && ($scope.selected_file.new || $scope.selected_file.replaced)) {
        if (directInsert) {
          $scope.insert();
        }
        else {
          $scope.changePanes('library', result);
        }
        return;
      }
      $scope.changePanes('library', result);
    }

    $scope.insert = function () {
      var results = [];
      if ($scope.toInsert.length) {
        for (var i = 0; i < $scope.toInsert.length; i++) {
          $scope.toInsert[i].fid = $scope.toInsert[i].id;
          results.push($scope.toInsert[i]);
        }
      }
      else {
        $scope.selected_file.fid = $scope.selected_file.id; // hack to prevent rewriting a lot of Media's code.
        results.push($scope.selected_file);
      }

      close(results);
    }

    $scope.cancel = function () {
      close([]);
    }

    // when a set of files are passed to the Media Browser, they were handled by some other service and then passed
    // to the Media Browser to handle
    if (params.files) {
      fetching.then(function () {
        var accepted = [], rejected = [];
        for (var i=0; i<params.files.length; i++) {
          if ($scope.validate(params.files[i])) {
            accepted.push(params.files[i]);
          }
          else {
            rejected.push(params.files[i]);
          }
        }
        $scope.checkForDupes(accepted, {}, rejected);
      });
    }
  }])
  .directive('mbOpenModal', ['$parse', 'mbModal', function($parse, mbModal) {

    function link(scope, elem, attr, contr) {
      elem.bind('click', function (event) {
        event.preventDefault();
        event.stopPropagation();
        // get stuff from the element we clicked on and Drupal.settings
        var elem = event.currentTarget,
          params = mbModal.defaultParams(),
          panes = elem.attributes['panes'].value,
          types = elem.attributes['types'].value.split(',');

        if (attr['replace']) {
          var prop = attr['replace'];
          params.replace = scope[prop];
        }

        for (var i in params.browser.panes) {
          params.browser.panes[i] = (panes.indexOf(i) !== -1);
        }

        params.types = {}
        for (i=0; i<types.length;i++) {
          params.types[types[i]] = types[i];
        }

        params.onSelect = function (inserted) {
          if (elem.attributes['on-select'].value) {
            $parse(elem.attributes['on-select'].value)(scope, {
              $inserted: inserted
            });
          }
          else {
            window.location.reload();
          }
        }

        mbModal.open(params);
      });
    }

    return {
      template: '<ng-transclude></ng-transclude>',
      link: link,
      transclude: true
    }
  }])
  .service('mbModal', ['ModalService', function (ModalService) {
      this.defaultParams = function () {
        var params = {
          dialog: {
            buttons: {},
            dialogClass: 'media-wrapper',
            modal: true,
            draggable: false,
            resizable: false,
            minWidth: 600,
            width: 800,
            height: 650,
            position: 'center',
            title: undefined,
            overlay: {
              backgroundColor: '#000000',
              opacity: 0.4
            },
            zIndex: 10000,
            close: function (event, ui) {
              $(event.target).remove();
            }
          },
          browser: {
            panes: {
              web: true,
              upload: true,
              library: true
            }
          },
          onSelect: angular.noop,
          types: {
            image: 'image',
            video: 'video',
            audio: 'audio',
            executable: 'executable',
            document: 'document',
            html: 'html'
          },
          replace: false
        };

        return params;
      }

      this.requirementsMet = function() {
        return (window.File && window.FormData);
      }

      this.open = function (params) {
        var defaults = this.defaultParams(),
          nparams = {
            dialog: angular.extend([], defaults.dialog, params.dialog),
            browser: angular.extend({}, defaults.browser, params.browser),
            onSelect: params.onSelect || defaults.onSelect,
            types: params.types || defaults.types,
            max_filesize: params.max_filesize || null,
            max_filesize_raw: params.max_filesize_raw || null,
            replace: params.replace || defaults.replace
        };

        if (params.files) {
          nparams.files = params.files;
        }

        ModalService.showModal({
          templateUrl: rootPath+'/templates/browser.html?vers='+Drupal.settings.version.mediaBrowser,
          controller: 'BrowserCtrl',
          inputs: {
            params: nparams
          }
        }).then(function (modal) {
          modal.element.dialog(nparams.dialog);
          modal.close.then(function (result) {
            // run the function passed to us
            if (Array.isArray(result)) {
              if (result.length) {
                nparams.onSelect(result);
              }
            }
            else if (result) {
              nparams.onSelect(result);
            }
          });
        });
      }
  }]);
})(jQuery);
