(function () {

  var m = angular.module('formElement', ['basicFormElements', 'osHelpers', 'ngSanitize', 'DependencyManager']);

  m.run(['Dependencies', function (dm) {
    var deps = dm.GetDependencies('formElement');
    Array.prototype.push.apply(m.requires, deps);
  }]);

  m.directive('formElement', ['$compile', '$filter', '$sce', '$timeout', function ($compile, $filter, $sce, $t) {
    return {
      scope: {
        element: '=',
        value: '='
      },
      template: '<div class="form-wrapper" ng-class="classname">' +
        '<span ng-if="element.prefix" ng-bind-html="to_trusted(element.prefix)"></span>' +
        '<span>Placeholder</span>' +
        '<div class="description" ng-bind-html="description"></div>' +
      '</div>',
      link: function (scope, elem, attr) {
        scope.id = $filter('idClean')(scope.element.name, 'edit');
        scope.description = $sce.trustAsHtml(scope.element.description);
        scope.label = scope.element.title;
        scope.access = scope.element.access;
        scope.classname = '';
        scope.to_trusted = function(html_code) {
          return $sce.trustAsHtml(html_code);
        }
        angular.forEach(scope.element.class, function(classname, key) {
          scope.classname += ' ' + classname;
        });

        if (scope.access == undefined) {
          scope.access = true;
        }

        if (scope.access) {
          var copy = elem.find('span').clone();
          for (var k in scope.element) {
            if (k == 'type') {
              copy.attr('fe-'+scope.element[k], '');
            }
            else if (k == 'custom_directive') {
              copy.attr(scope.element[k], '');
            }
            else if (k == 'name') {
              copy.attr('name', scope.element[k]);
            }
            else if (k == 'custom_directive_parameters') {
              for (var l in scope.element[k]) {
                copy.attr(l, scope.element[k][l]);
              }
            }
          }

          copy.attr('element', 'element');

          copy.attr('input-id', scope.id);
          copy.attr('ng-model', 'value');
          elem.find('span').replaceWith(copy);
          copy = $compile(copy)(scope);
          if (scope.element.attached && scope.element.attached.js) {
            $t(function () {
              for (var i in scope.element.attached.js) {
                if (angular.isObject(scope.element.attached.js[i])) {
                  Drupal.behaviors.states.attach(jQuery(elem), scope.element.attached.js[i].data);
                }
              }
            });
          }
        }
        else {
          elem.remove();
        }
      }
    }
  }]);

  m.filter('weight', [function () {
    return function (input) {
      if (angular.isArray(input)) {
        throw new Exception('weight filter does not support arrays. Please use an object instead');
      }
      var keys = Object.keys(input),
        basics = [],
        defaultWeight = 0,
        weightIncrement = 0.001;
      for (var i = 0; i < keys.length; i++) {
        var w = defaultWeight;
        if (input[keys[i]].weight != undefined) {
          w = input[keys[i]].weight;
        }
        else {
          defaultWeight += weightIncrement;
        }
        basics.push({key: keys[i], weight: w})
      }

      basics.sort(function (a,b) {
        return a.weight - b.weight;
      });

      var output = {};
      for (i = 0; i < basics.length; i++) {
        var key = basics[i].key;
        output[key] = input[key];
      }

      return output;
    }
  }]);

})();
