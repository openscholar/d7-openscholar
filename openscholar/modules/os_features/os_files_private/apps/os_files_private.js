(function() {
  angular.module('os-files-private', ['mediaBrowser', 'FileEditorModal'])
    .config(['EntityConfigProvider', function(ecp) {
      var elem = angular.element(document.querySelectorAll('.view-id-os_files_private'));
      elem.attr('ng-controller', 'OSFilesPrivateController');

      ecp.addField('files', 'private', 'only');
    }])
    .run (['$window', '$timeout', function ($w, $t) {
      var elem = document.querySelector('.add_new');
      elem = angular.element(elem);

      var hash = $w.location.hash;
      if (hash == '#open') {
        $t(function () {
          elem.triggerHandler('click');
        }, 0);
        $w.location.hash = '';
      }
      angular.element(window).on('hashchange', function (e) {
        if (e.fragment == 'open') {
          $t(function () {
            elem.triggerHandler('click');
          }, 0);
          $w.location.hash = '';
        }
      });
    }])
    .controller('OSFilesPrivateController', ['$scope', 'FILEEDITOR_RESPONSES', function ($scope, FER) {
      $scope.reload = function (result) {
        var reload = false;
        if (result == FER.SAVED || result == FER.REPLACED) {
          reload = true;
        }
        if (Array.isArray(result) && result.length) {
          reload = true;
        }
        if (result === true) {
          reload = true;
        }

        if (reload) {
          window.location.reload();
        }
      }
    }]);
})();