(function () {

  var m = angular.module('ApSettingsForm', ['angularModalService', 'redirectForm', 'MediaBrowserField', 'formElement', 'os-buttonSpinner']);

  /**
   * Fetches the settings forms from the server and makes them available directives and controllers
   */
  m.service('apSettings', ['$http', '$q', function ($http, $q, $httpParamSerializer) {

    var settingsForms = {};
    var settings = {};
    var groupInfo = {};
    var promises = [];

    var queryArgs = {};

    if (Drupal.settings.spaces != undefined) {
      if (Drupal.settings.spaces.id) {
        queryArgs.vsite = Drupal.settings.spaces.id;
      }
    }

    var baseUrl = Drupal.settings.paths.api;
    var config = {
      params: queryArgs
    };

    // fetch all the form data from the server
    promises.push($http.get(baseUrl+'/settings', config).then(function (response) {
      var data = response.data;

      for (var varName in data.data) {
        var varForm = data.data[varName];
        var group = varForm.group['#id'];

        settingsForms[group] = settingsForms[group] || {};
        settingsForms[group][varName] = varForm.form;

        groupInfo[group] = varForm.group;

        settings[varName] = varForm.form['#default_value'];
      }
      return response;
    }));

    var allPromise = $q.all(promises);

    this.SettingsReady = function() {
      return allPromise;
    }

    this.GetFormDefinitions = function (group_id) {
      if (typeof settingsForms[group_id] != 'undefined') {
        return angular.copy(settingsForms[group_id]);
      }
      throw "No form group with id " + group_id + " exists.";
    }

    this.GetFormTitle = function (group_id) {
      if (typeof groupInfo[group_id] != 'undefined') {
        return groupInfo[group_id]['#title'];
      }
      throw "No form group with the id " + group_id + " exists.";
    }

    this.GetHelpLink = function (group_id) {
      if (typeof groupInfo[group_id] != 'undefined') {
        return groupInfo[group_id]['#help_link'];
      }
      throw "No form group with the id " + group_id + " exists.";
    }

    this.IsSetting = function (var_name) {
      return var_name in settings;
    }

    this.SaveSettings = function (settings, buttonName) {
      var counts = 0;
      angular.forEach(settings, function(val, key) {
        if (val == 'Submit') {
          counts++;
        }
      });
      if (counts > 1 && buttonName != '') {
        var settingsNew = {};
          angular.forEach(settings, function(val, key) {
          if (val != 'Submit' || key == buttonName) {
            settingsNew[key] = val;
          }
        });
        settings = settingsNew;
      }
      console.log(settings);

      return $http.put(baseUrl+'/settings', settings, config);
    }

  }]);

  /**
   * Open modals for the settings forms
   */
  m.directive('apSettingsForm', ['ModalService', 'apSettings', function (ModalService, apSettings) {
    var dialogOptions = {
      minWidth: 850,
      minHeight: 100,
      modal: true,
      position: 'center',
      dialogClass: 'ap-settings-form'
    };

    function link(scope, elem, attrs) {
      apSettings.SettingsReady().then(function () {
        scope.title = apSettings.GetFormTitle(scope.form);
      });

      elem.bind('click', function (e) {
        e.preventDefault();
        e.stopPropagation();

        ModalService.showModal({
          controller: 'apSettingsFormController',
          template: '<form id="{{formId}}" name="settingsForm" ng-submit="submitForm($event)">' +
            '<div class="messages" ng-show="status.length || errors.length"><div class="dismiss" ng-click="status.length = 0; errors.length = 0;">X</div>' +
              '<div class="status" ng-show="status.length > 0"><div ng-repeat="m in status track by $index"><span ng-bind-html="m"></span></div></div>' +
              '<div class="error" ng-show="errors.length > 0"><div ng-repeat="m in errors track by $index"><span ng-bind-html="m"></span></div></div></div>' +
            '</div>' +
            '<div class="form-column-wrapper column-count-{{columnCount}}" ng-if="columnCount > 1">' +
              '<div class="form-column column-{{column_key}}" ng-repeat="(column_key, elements) in columns">' +
                '<div class="form-item" ng-repeat="(key, field) in elements | weight">' +
                  '<div form-element element="field" value="formData[key]"><span>placeholder</span></div>' +
                '</div>' +
              '</div>' +
            '</div>' +
            '<div class="form-wrapper" ng-if="columnCount == 1">' +
              '<div class="form-item" ng-repeat="(key, field) in formElements | weight">' +
                '<div form-element element="field" value="formData[key]"><span>placeholder</span></div>' +
              '</div>' +
            '</div>' +
            '<div class="help-link" ng-bind-html="help_link"></div>' +
          '<div class="actions" ng-show="showSaveButton"><button type="submit" button-spinner="settings_form_save" name="save" spinning-text="Saving">Save</button><input type="button" value="Close" ng-click="close(false)"></div></form>',
          inputs: {
            form: scope.form
          }
        })
        .then(function (modal) {
          dialogOptions.title = scope.title;
          dialogOptions.close = function (event, ui) {
            modal.element.remove();
          }
          modal.element.dialog(dialogOptions);
          modal.close.then(function (result) {
            if (result) {
              window.location.reload();
            }
          });
        });
      });
    }

    return {
      link: link,
      scope: {
        form: '@'
      }
    };
  }]);
  
  /**
   * The filter for the optgroup select dropdowns.
   */
  m.filter("filterWithItems", function() {
    return function(categories, present) {
      return jQuery.grep(categories, function(category) {
        if (present) {
         return category.items != undefined
        } else {
            return category.items == undefined
        }
      });
    }
  });

  /**
   * The controller for the forms themselves
   */
  m.controller('apSettingsFormController', ['$scope', '$sce', 'apSettings', 'buttonSpinnerStatus', 'form', 'close', function ($s, $sce, apSettings, bss, form, close) {
    var formSettings = {};
    $s.formId = form;
    $s.formElements = {};
    $s.formData = {};

    $s.status = [];
    $s.errors = [];
    $s.columns = {};
    $s.columnCount = 0;
    $s.showSaveButton = true;

    apSettings.SettingsReady().then(function () {
      var settingsRaw = apSettings.GetFormDefinitions(form);
      console.log(settingsRaw);

      $s.help_link = $sce.trustAsHtml(apSettings.GetHelpLink(form));

      for (var k in settingsRaw) {
        $s.formData[k] = settingsRaw[k]['#default_value'] || null;
        var attributes = {
          name: k
        };

        var col = settingsRaw[k]['#column'] || 'default';
        if (typeof $s.columns[col] == 'undefined') {
          $s.columns[col] = {};
          $s.columnCount++;
        }
        for (var j in settingsRaw[k]) {
          if (j.indexOf('#') === 0 && (j != '#default_value' && j != '#column')) {
            var attr = j.substr(1, j.length);
            attributes[attr] = settingsRaw[k][j];
          }
        }

        if (attributes.value) {
          attributes.origValue = attributes.value;
        }

        $s.formElements[k] = $s.columns[col][k] = attributes;

        if ($s.formElements[k].type == 'submit') {
          $s.showSaveButton = false;
        }
      }
    });

    function submitForm($event) {
      var button = document.activeElement,
        triggered = false;
      var buttonId = document.activeElement.id;
      if (apSettings.IsSetting(button.getAttribute('name'))) {
        triggered = true;
      }

      var buttonName = '';
      if (button.getAttribute('name')) {
        buttonName = button.getAttribute('name');
      }
      if ($s.settingsForm.$dirty || triggered) {
        bss.SetState('settings_form_' + buttonName, true);
        apSettings.SaveSettings($s.formData, buttonName).then(function (response) {
          var body = response.data;
          sessionStorage['messages'] = JSON.stringify(body.data.messages);
          $s.status = [];
          $s.error = [];
          var close = true;
          var reload = true;
          bss.SetState('settings_form_' + buttonName, false);
          for (var i = 0; i < body.data.length; i++) {
            switch (body.data[i].type) {
              case 'no_close':
                close = false;
              case 'no_reload':
                reload = false;
                break;
              case 'message':
                $s[body.data[i].message_type].push(body.data[i].message)
            }
          }

          if (body.messages.status) {
            for (var i = 0; i < body.messages.status.length; i++) {
              if (body.messages.status[i] == 'Your updates will go live within the next 15 minutes.') {
                $s.close(reload);
              }
            }
          } else {
            if (close && !body.messages.error) {
              $s.close(reload);
            } else if (body.messages.error) {
              $s.errors = [];
              angular.forEach(body.messages.error, function(value , key) {
                $s.errors.push($sce.trustAsHtml(value));
              });
              bss.SetState('settings_form_' + buttonName, false);
            }
          }
        }, function (error) {
          $s.errors = [];
          $s.status = [];
          if (buttonId.indexOf('edit-os-importer-submit') < 0) {
            $s.errors.push("Sorry, something went wrong. Please try another time.");
          } else {
            $s.errors.push("The import failed. Please review import <a href='https://help.theopenscholar.com/importing-content' target='_blank'>best practices</a> for optimal results.");
          }
          bss.SetState('settings_form_' + buttonName, false);
        });
      }
      else {
        $s.close(false);
      }
    }
    $s.submitForm = submitForm;

    $s.close = function (arg) {
      close(arg);
    }
  }]);
})()
