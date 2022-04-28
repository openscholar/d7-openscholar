/**
 * Adds a spinner to buttons, that can be enabled and disabled with the buttonSpinnerStatus service.
 * Does not support <input> elements. Use <button> instead.
 *
 * Usage:
 *  <input type="submit" value="Save" ng-click="save()" button-spinner="uniqueId" [spinningText="Saving..."]>
 *
 *  function save() {
 *    buttonSpinnerStatus.SetState('uniqueId', true);
 *    businessLogic().then(function() {
 *      buttonSpinnerStatus.SetState('uniqueId', false);
 *    });
 *  }
 */
(function () {
  var m = angular.module('os-buttonSpinner', []);

  m.service('buttonSpinnerStatus', [function () {

    var states = {},
      handlers = {};

    this.SetState = function(id, state) {
      states[id] = !!state;
      if (handlers[id]) {
        for (var i = 0; i < handlers[id].length; i++) {
          handlers[id][i](states[id]);
        }
      }
    }

    this.GetState = function(id) {
      return typeof states[id] != undefined ? states[id] : false;
    }

    this.$observe = function(id, func) {
      if (angular.isFunction(func)) {
        handlers[id] = handlers[id] || [];
        handlers[id].push(func);

        func(states[id]);
      }
    }
  }]);

  m.directive('buttonSpinner', ['buttonSpinnerStatus', function ($bss) {
    return {
      restrict: 'A',
      scope: {
        spinnerID: '@buttonSpinner',
        spinningText: '@?'
      },
      transclude: true,
      template:
        "<ng-transclude></ng-transclude>" +
        '<span ng-show="spinning" class="spinner">',
      link: function (scope, elem, attr) {
        var textElem = elem.find('ng-transclude').find('span')[0],
          original = '';

        $bss.$observe(scope.spinnerID, function (state) {
          if (original == '') {
            original = elem.text();
          }
          scope.spinning = state;
          if (scope.spinningText) {
            textElem.innerHTML = state ? scope.spinningText : original;
          }
        })
      }
    }
  }])
})();