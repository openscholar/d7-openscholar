(function () {

  var m = angular.module('grouper', ['DependencyManager', 'os-buttonSpinner']);

  m.config(['DependenciesProvider', function ($depProvider) {
    $depProvider.AddDependency('formElement', 'grouper');
  }]);

  m.directive('grouper', ['$http', 'buttonSpinnerStatus', function ($http, bss) {
    var groups = [];

    var status = {
      loading: true,
    }
    bss.SetState('grouper_AllGroups', true);
    $http.get(Drupal.settings.paths.api + '/grouper').then(function (resp) {
      console.log(resp);
      status.loading = false;
      bss.SetState('grouper_AllGroups', false);
      Array.prototype.push.apply(groups, resp.data.data); // this adds group elements without creating a new array instance
    }, function (errorResp) {
      console.log(errorResp);
      status.loading = false;
      bss.SetState('grouper_AllGroups', false);
    });
    return {
      require: 'ngModel',
      scope: {
        selected: '=ngModel'
      },
      templateUrl: Drupal.settings.paths.grouper + '/grouper.template.html',
      link: function (scope, elem, attrs, ngModelController) {
        scope.status = status;
        scope.Groups = function () {
          return groups;
        }

        if (!angular.isArray(scope.selected)) {
          scope.selected = [];
        }

        scope.search = '';
        /**
         * Find a group by its unique path identifier
         * @param path
         * @returns {*}
         */
        function findGroupByPath(path) {
          for (var i = 0; i < groups.length; i++) {
            if (groups[i].name == path) {
              return groups[i];
            }
          }
        }

        /**
         * Gets or sets the panel's visibile status
         */
        var showPanel = false;
        scope.ShowPanel = function (toggle) {
          if (toggle != undefined) {
            showPanel = !showPanel;
          }

          return showPanel;
        }

        /**
         * @returns string - human-readable, comma-separated list of all selected groups, on a single line
         */
        scope.SelectedGroupNames = function () {
          var gs = groups.filter(function (g) {
            return (scope.selected.indexOf(g.name) > -1);
          });

          var names = gs.map(function (g) {
            return g.displayExtension;
          });

          return names.join(' | ');
        }

        /**
         * Returns the human readable label for a group
         */
        scope.GroupLabel = function (path) {
          var g = findGroupByPath(path);

          if (g) {
            return g.displayExtension;
          }
          return '';
        }

        /**
         *
         */
        scope.Remove = function (path) {
          var k = scope.selected.indexOf(path);
          if (k > -1) {
            scope.selected.splice(k, 1);
            ngModelController.$setDirty();
          }
        }

        /**
         *
         */
        scope.AddGroup = function (path) {
          if (scope.selected.length < 7) {
            var k = scope.selected.indexOf(path);
            if (k == -1) {
              scope.selected.push(path);
              ngModelController.$setDirty();
            }
          }
        }

        /**
         *
         */
        scope.IsGroupSelected = function (path) {
          var k = scope.selected.indexOf(path);
          return k > -1;
        }
      }
    }
  }]);
})();
