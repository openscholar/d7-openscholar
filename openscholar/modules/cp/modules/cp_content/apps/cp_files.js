(function() {
  angular.module('cp-files', ['mediaBrowser', 'FileEditorModal'])
    .run(['$window', '$timeout', function($w, $t) {
      var elem = angular.element(document.querySelectorAll('.view-id-cp_files'));
      elem.attr('ng-controller', 'CpFilesController');

      var addNew = document.querySelector('.add_new');
      addNew = angular.element(addNew);

      var hash = $w.location.hash;
      if (hash == '#open') {
        $t(function () {
          addNew.triggerHandler('click');
        }, 0);
        $w.location.hash = '';
      }
      angular.element(window).on('hashchange', function (e) {
        if (e.fragment == 'open') {
          $t(function () {
            addNew.triggerHandler('click');
          }, 0);
          $w.location.hash = '';
        }
      });
    }])
    .controller('CpFilesController', ['$scope', 'FILEEDITOR_RESPONSES', function ($scope, FER) {
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