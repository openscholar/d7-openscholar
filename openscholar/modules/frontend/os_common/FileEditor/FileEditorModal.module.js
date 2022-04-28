(function () {
  var service,
    dialogParams = {
      minWidth: 800,
      minHeight: 650,
      modal: true,
      position: 'center',
      dialogClass: 'file-editor'
    },
    loading,
    fetchPromise;


  angular.module('FileEditorModal', ['EntityService', 'FileEditor', 'angularModalService', 'angularFileUpload', 'locationFix'])
    .run(['EntityService', function (EntityService) {
      service = new EntityService('files', 'id');
      loading = true;
      fetchPromise = service.fetch().then(function() {
        loading = false;
      });
    }])
    .service('FileEditorOpenModal', ['ModalService', function (ModalService) {

      return {
        open: function (fid, close) {
          ModalService.showModal({
            template: '<div><div class="file-entity-loading" ng-show="loading"><div class="file-entity-loading-message">Loading files...<br />' +
                '<div class="progress-bar progress-striped"><div class="progress-bar-completed" style="width: 100%"></div></div></div></div>' +
                '<div file-edit file="file" on-close="closeModal(saved)"></div></div>',
            controller: 'FileEditorModalController',
            inputs: {
              fid: fid
            }
          })
          .then (function (modal) {
            modal.element.dialog(dialogParams);
            modal.close.then(function(result) {
              if (angular.isFunction(close)) {
                close({result: result});
              }
            });
          })
        }
      }

    }])
    .directive('fileEditorModal', ['FileEditorOpenModal', function(feom) {

      function link(scope, elem, attr) {
        var data = {
          scope: scope
        }
        elem.bind('click', data, clickHandler);
        scope.runViews = false;
        if (!attr.onClose && attr.viewsClose) {
          scope.runViews = true;
        }

        elem.parent().find('.fid').change(function (e) {
          // Media removes all click events on the edit button, so we have to add the handler again if we want
          // this to continue working.
          elem.bind('click', data, clickHandler);
          elem.attr('fid', e.target.value);
        })
      }

      function clickHandler(event) {
        event.preventDefault();
        event.stopPropagation();
        var fid = event.currentTarget.attributes.fid.value,
          scope = event.data.scope,
          closer = scope.runViews ? scope.viewsClose : scope.onClose;

        feom.open(fid, closer);

        return false;
      }

      return {
        link: link,
        scope: {
          onClose: '&',
          viewsClose: '&'
        }
      }
    }])
    .controller('FileEditorModalController', ['$scope', 'EntityService', 'fid', 'close', function ($scope, EntityService, fid, close) {
      $scope.loading = loading;
      $scope.asset_path = Drupal.settings.paths.FileEditor;
      if (loading) {
        fetchPromise.then(function () {
          $scope.file = angular.copy(service.get(fid));
          $scope.loading = loading;
        });
      }
      else {
        $scope.file = angular.copy(service.get(fid));
      }

      $scope.closeModal = function (saved) {
        close(saved);
      }
    }]);
})();
