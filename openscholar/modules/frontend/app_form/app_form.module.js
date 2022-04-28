(function () {

  var m = angular.module('AppForm', ['angularModalService', 'os-buttonSpinner']);

  m.directive('appFormModal', ['ModalService', function (ModalService) {
    var dialogOptions = {
      minWidth: 850,
      minHeight: 100,
      modal: true,
      position: 'top+100px',
      dialogClass: 'app-form'
    };

    function clickHandler(e) {
      e.preventDefault();
      e.stopPropagation();

      ModalService.showModal({
        controller: 'appFormController',
        templateUrl: Drupal.settings.paths.app_form + '/app_form.template.html',
      })
      .then(function (modal) {
        dialogOptions.close = function (event, ui) {
          modal.element.remove();
        }
        modal.element.dialog(dialogOptions);
        modal.close.then(function (result) {
          if (result == 'reload') {
            window.location.reload();
          }
        });
      });
    }
    return {
      link: function (scope, elem, attr) {
        elem.bind('click', clickHandler);
      }
    }
  }]);

  m.controller('appFormController', ['$scope', 'appFormService', 'buttonSpinnerStatus', 'close', function ($s, afs, bfs, close) {
    $s.apps = [];
    afs.fetch().then(function (r) {
      for (var k in r.data) {
        $s.apps.push(r.data[k]);
      };
    });

    $s.submit = function () {
      bfs.SetState('app-form', true);
      for (var i = 0; i < $s.apps.length; i++) {
        if ($s.apps[i].disable) {
          $s.apps[i].enabled = false;
        }
        else if ($s.apps[i].enable) {
          $s.apps[i].enabled = true;
        }
      }

      afs.save($s.apps).then(function (status) {
        if (status == 'success') {
          close('reload');
        }
        else {
          close();
        }
        bfs.SetState('app-form', false);
      })
    }

    $s.close = function () {
      close();
    }
  }]);

  m.directive('appPrivacySelector', [function () {
    function mainClickHandler(e) {
      this.show = true;
      document.body.addEventListener('click', this.cancelClickHandler, true);
      this.$digest();
    }

    function subClickHandler(e) {
      this.val = angular.element(e.currentTarget).attr('data-value');
      this.show = false;
      this.$apply();
      e.stopPropagation();
      e.preventDefault();
    }

    function cancelClickHandler(e) {
      this.show = false;
      this.$digest();
    }

    return {
      template: '{{ val == "1" ? "Site Members Only" : "The Public" }}<div class="privacy-popup" ng-show="show"><div class="privacy-popup-selector private" data-value="1" ng-class="{selected: val == 1}">Site Members Only</div><div class="privacy-popup-selector everyone" data-value="0" ng-class="{selected: val == 0}">The Public</div></div>',
      scope: {
        val: '=ngModel',
      },
      link: function (scope, elem, attr) {
        scope.show = false;
        elem.bind('click', angular.bind(scope, mainClickHandler));
        angular.element(elem[0].querySelectorAll('.privacy-popup-selector')).bind('click', angular.bind(scope, subClickHandler));
        scope.cancelClickHandler = angular.bind(scope, cancelClickHandler);
      }
    }
  }]);

  m.service('appFormService', ['$http', '$q', '$timeout', function ($http, $q, $t) {
    var pristine = [];
    var map = {};
    var fetchDefered;
    function fetch() {
      if (!fetchDefered) {
        fetchDefered = $q.defer();
        var queryArgs = {};
        if (Drupal.settings.spaces && Drupal.settings.spaces.id) {
          queryArgs.vsite = Drupal.settings.spaces.id;
        }

        var baseUrl = Drupal.settings.paths.api;
        var config = {
          params: queryArgs
        };

        $http.get(baseUrl + '/apps', config).then(function (r) {
          pristine = angular.copy(r.data.data);
          fetchDefered.resolve(r.data);

          for (var i = 0; i < r.data.data.length; i++) {
            map[r.data.data[i].machine_name] = i;
          }
        },
        function (e) {
          fetchDefered.reject(e);
        });
      }

      return fetchDefered.promise;
    }

    var saveDefer;
    function save(values) {
      if (saveDefer) {
        var cancel = $q.defer();
        $t(function () {
          cancel.reject('progress');
        });
        return cancel.promise;
      }

      var queryArgs = {};
      if (Drupal.settings.spaces && Drupal.settings.spaces.id) {
        queryArgs.vsite = Drupal.settings.spaces.id;
      }

      var baseUrl = Drupal.settings.paths.api;
      var config = {
        params: queryArgs
      };

      saveDefer = $q.defer();
      var dirty = [];
      for (var i = 0; i < values.length; i++) {
        var machineName = values[i].machine_name,
          key = map[machineName],
          pris = pristine[key];

        for (var k in pris) {
          if (pris[k] != values[i][k]) {
            dirty.push(values[i]);
            break;
          }
        }
      }

      if (dirty.length == 0) {
        $t(function () {
          saveDefer.resolve('no changes');
        });
      }
      else {
        $http.patch(baseUrl + '/apps', dirty, config).then(function (r) {
          // success
          saveDefer.resolve('success');
        }, function (e) {
          console.log(e);
          saveDefer.reject('error');
        });
      }

      return saveDefer.promise;
    }

    return {
      fetch: fetch,
      save: save
    }
  }]);

  m.run(['appFormService', function (afs) {
    afs.fetch();
  }]);

})()
