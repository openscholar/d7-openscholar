/**
 * A pager for a set of content that can be filtered.
 * The number of pages updates as the filters get more or less precise.
 *
 * Other modules implement the actual filtering. This module handles all interactions the filter has with the pager.
 */

(function () {
  var rootPath = '';

  angular.module('JSPager', [])
    .config(function($sceDelegateProvider) {
      var whitelist = $sceDelegateProvider.resourceUrlWhitelist(),
          domain = osCommonHelpers.findDomain(rootPath);

      domain = domain+'/**';
      whitelist.push(domain);

      $sceDelegateProvider.resourceUrlWhitelist(whitelist);

      if (typeof Drupal != 'undefined' && typeof Drupal.settings != 'undefined') {
        rootPath = Drupal.settings.paths.JSPager;
      }
    })
    .filter('PagerCurrentPage', function () {
      function currentPage(input, pager) {
        if (input) {
          var start = (pager.currentPage() - 1) * pager.pageSize;
          if (Array.isArray(input)) {
            return input.slice(start, start + pager.pageSize);
          }
          else if (typeof input == "object") {
            var i = 0,
              output = {};
            for (var key in input) {
              if (i >= start && i < start + pager.pageSize) {
                output[key] = input[key]
              }
              i++
            }
            return output;
          }
        }
        return '';
      }
      return currentPage;
    })
    .directive('jsPager', ['$parse', function($parse) {
      var currentPages = [],
        idMap = {};
      return {
        templateUrl: rootPath+'/pager.html',
        transclude: true,
        controller: [function () {

          this.getJSPagerId = function (string) {
            if (idMap[string] == undefined) {
              currentPages.push(1);
              idMap[string] = currentPages.length - 1;
            }

            return idMap[string];
          }

          this.page = function (id, page) {
            if (typeof page != 'undefined') {
              currentPages[id] = page;
            }

            return currentPages[id];
          }
        }],
        compile: function pagerCompile(element, attr) {
          var loop = attr.jsPager,
          /* this regex matches the following patterns:
           * var in collection | filter:item1 | filter:item2 track by var.property
           * if you try to do 'var in collection track by var.property | filters' it won't work properly
           */
            match = attr.jsPager.match(/^\s*([\s\S]+?)\s+in\s+([\s\S]+?)(?:\s+as\s+([\s\S]+?))?(?:\s+track\s+by\s+([\s\S]+?))?\s*$/),
            index = match[1],
            collection = match[2],
            trackExp = match[4],
            elements = [],
            pageSize = attr.pageSize || 10,
            //allElements = {$id: hashKey},
            trackExpGetter, trackByArray;

          /*  come back to this later
          if (trackExp) {
            trackExpGetter = $parse(trackExp);
          }
          else {
            trackByArray = function(key, value) {
              return hashKey(value);
            }
          }*/

          return function($scope, $element, $attr, _c, linker) {
            /* come back to this later. It's all performance optimization and I'm going cross-eyed.
            if (trackByExpGetter) {
              trackByIdExpFn = function(key, value, index) {
                // assign key, value, and $index to the locals so that they can be used in hash functions
                if (keyIdentifier) allElements[keyIdentifier] = key;
                allElements[valueIdentifier] = value;
                allElements.$index = index;
                return trackByExpGetter($scope, allElements);
              };
            }*/

            var $transclude = jQuery('ng-transclude, .ng-transclude, [ng-transclude]', $element),
              $target = $element;

            if ($transclude.length) {
              $target = $transclude.parent();
              $transclude.remove();
            }

            pagerLink($scope, $element, $attr, _c);
            $scope.$watchCollection(collection, function(collection) {
              $scope.collectionLength = 0;
              if (Array.isArray(collection)) {
                $scope.collectionLength = collection.length;
              }
              else {
                for (var key in collection) {
                  $scope.collectionLength++;
                }
              }
            });

            $scope.$watchCollection(collection + ' | PagerCurrentPage:pager', function(collection) {
              var i, block, childScope;

              // check if elements have already been rendered
              if (elements.length > 0){
                // if so remove them from DOM, and destroy their scope
                for (i = 0; i < elements.length; i++) {
                  elements[i].el.remove();
                  elements[i].scope.$destroy();
                };
                elements = [];
              }

              i = 0;
              for (var key in collection) {
                // create a new scope for every element in the collection.
                childScope = $scope.$new();
                // pass the current element of the collection into that scope
                childScope[index] = collection[key];

                linker(childScope, function(clone){
                  // clone the transcluded element, passing in the new scope.
                  $target.append(clone); // add to DOM
                  block = {};
                  block.el = clone;
                  block.scope = childScope;
                  elements.push(block);
                });
              };

            });
          }
        }
      };
    }]);


  function pagerLink(scope, iElement, iAttrs, controller) {
    var JSPagerId = controller.getJSPagerId(iAttrs.jsPager),
      current = controller.page(JSPagerId);

    scope.pager = {
      currentPage: currentPage,
      numPages: numPages,
      canPage: canPage,
      changePage: changePage,
      pageSize: parseInt(iAttrs.pageSize) || 10
    };

    function currentPage() {
      var pages = numPages();
      if (pages === 0) {
        return 0;
      }
      else if (Number.isInteger(pages) && current > pages) {
        current = pages;
      }
      return current;
    }

    function numPages() {
      return Math.ceil(scope.collectionLength/scope.pager.pageSize);
    }

    function canPage(dir) {
      dir = parseInt(dir);
      var newPage = currentPage() + dir;

      return (1 <= newPage && newPage <= numPages());
    }

    function changePage(dir) {
      if (canPage(dir)) {
        dir = parseInt(dir);
        current += dir;
        controller.page(JSPagerId, current);
      }
    }
  }
})();

// Number.isInteger is not supported by IE11, below Polyfill enables IE 11 support with backward compatibility.
Number.isInteger = Number.isInteger || function(value) {
  return typeof value === "number" && isFinite(value) && Math.floor(value) === value;
};