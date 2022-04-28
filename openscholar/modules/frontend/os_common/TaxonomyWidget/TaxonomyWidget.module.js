(function () {
/**
 * Provides mechanisms for choosing a taxonomy term for a given entity.
 */
var taxonomy = angular.module('TaxonomyWidget', ['EntityService', 'os-auth', 'ui.select', 'ngSanitize', 'ui.bootstrap', 'ui.bootstrap.typeahead', 'TreeSelector']);

taxonomy.directive('taxonomyWidget', ['EntityService', function (EntityService) {
  var path = Drupal.settings.paths.TaxonomyWidget;
  return {
    restrict: 'E',
    scope: {
      terms: "=",
      bundle: "@"
    },
    templateUrl: path + '/TaxonomyWidget.html?vers='+Drupal.settings.version.TaxonomyWidget,
    link: function (scope, elem, attrs) {
      var entityType = attrs.entityType;
      var vocabService = new EntityService('vocabulary', 'id');
      var termService = new EntityService('taxonomy', 'id');
      var termChange = false;
      var selectChange = false;
      scope.allTerms = {};
      scope.termsTree = [];
      scope.selectedTerms = {};
      scope.disabled = true;

      // Any change in the selected term scope will affect the file terms.
      // This can be done thanks to a "Two way binding" implements using the
      // = operator which defined in the isolated scope.
      scope.$watch('terms', function(newTerms, oldTerms) {
        if (!selectChange) {
          termChange = true;
          var bundle = attrs.bundle;
          if (newTerms instanceof Array) {
            vocabService.fetch({entity_type: entityType, bundle: bundle}).then(function (result) {

              scope.vocabs = result;
              for (var i = 0; i < scope.vocabs.length; i++) {
                var vocab = scope.vocabs[i];

                scope.termsTree[vocab.id] = vocab.tree;
                scope.allTerms[vocab.id] = [];

                scope.selectedTerms[vocab.id] = scope.selectedTerms[vocab.id] || [];

                termService.fetch({vid: vocab.id}).then(function (result) {
                  // If this vocab has no terms, bail out.
                  if (result.length == 0) return;

                  // Get the vocab so we can check it later
                  // We can't use the vocab var in the parent closure, because it changes before this code is executed
                  // and will always be set to the last vocab processed.
                  var vocab;
                  for (var i = 0; i < scope.vocabs.length; i++) {
                    if (scope.vocabs[i].id == result[0].vid) {
                      vocab = scope.vocabs[i];
                      break;
                    }
                  }

                  // Loop over the terms we just fetched.
                  for (var j = 0; j < result.length; j++) {
                    // Create a new object so we can ensure it passes equality tests
                    var t = result[j],
                      term = {
                        id: t.id,
                        label: t.label,
                        vid: t.vid
                      };
                    scope.allTerms[t.vid].push(term);

                    // Build scope.selectedTerms using the same object in allTerms
                    for (var i = 0; i < newTerms.length; i++) {
                      if (newTerms[i].id == t.id) {
                        scope.selectedTerms[t.vid].push(term);
                        break;
                      }
                    }
                  }

                  scope.disabled = false;
                });
              }
            });
          }
        }
        selectChange = false;
      }, true);

      /**
       * Converts our multi-tiered selectedTerms array into a flat array like what was passed to us earlier.
       */
      scope.$watch('selectedTerms', function(newTerms, oldTerms) {
        if (!termChange) {
          selectChange = true;
          scope.terms = [];
          for (var k in newTerms) {
            for (var i = 0; i < newTerms[k].length; i++) {
              if (newTerms[k][i] && newTerms[k][i].id) {
                scope.terms.push(newTerms[k][i]);
              }
            }
          }
        }
        termChange = false;
      }, true);

      /**
       * Add another term to the selected terms object.
       */
      scope.onSelect = function ($item, $model, $label) {
        scope.selectedTerms[$item.vid].splice(0, 0, $item);
        this.autocompleteTerm = null;
      };

      /**
       * Remove term from the selected terms object
       * @param vid
       * @param $index
       */
      scope.removeTerm = function (vid, $index) {
        scope.selectedTerms[vid].splice($index, 1);
      }

      /**
       * Add and remove a term when checking/un-checking the checkbox.
       */
      scope.termsSelected = function(term, object) {
        if (!scope.selectedTerms[term.vid]) {
          scope.selectedTerms[term.vid] = [];
        }

        var found = false;
        for (var i = 0; i < scope.selectedTerms[term.vid].length; i++) {
          if (scope.selectedTerms[term.vid][i].id == term.id) {
            scope.selectedTerms[term.vid].splice(i, 1);
            found = true;
            break;
          }
        }

        if (!found) {
          scope.selectedTerms[term.vid].push(term);
        }
      };

      scope.termSet = function(term) {
        for (var i = 0; i < scope.selectedTerms[term.vid].length; i++) {
          if (scope.selectedTerms[term.vid][i].id == term.id) {
            return true;
          }
        }

        return false;
      }

      scope.termTreeChangeCallback = function(node, tree) {

        /*if (Object.keys(node).indexOf('children') != -1) {
          // Iterating over the children's of the term.
          angular.forEach(node.children, function(value, key) {
            scope.termTreeChangeCallback(value);
          });
        }*/

        scope.termsSelected(termService.get(node.value));
      };

      scope.alreadySelected = function ($item) {
        if ($item && scope.selectedTerms[$item.vid].indexOf($item) == -1) {
          return $item;
        }
      }
    }
  }
}]);
})();