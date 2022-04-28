(function () {

  /**
   * Behavioral spec:
   * Selecting parent does NOT affect selection state of children
   * Selecting children does NOT affect selection state of parent
   * Completely unselected ree should begin completely collapsed
   * All selected nodes must be visible to start
   */

  var m = angular.module('TreeSelector', []);

  m.directive('treeSelectorWidget', [function () {

    function treeLinker(scope, elem, attr, contr, transFn) {

      scope.range = function(n) {
        return new Array(n);
      }

      scope.$watch('tree', function (newTree) {
        var result = [];
        for (var i = 0; i < scope.tree.length; i++) {
          result = result.concat(flattenTree(scope.tree[i]));
        }
        scope.flatTree = result;
      });

      var selected = [];
      scope.$watchCollection('selected', function (newItems) {
        selected = [];

        for (var i = 0; i < newItems.length; i++) {
          var id = 0;
          if (typeof newItems[i] == 'object') {
            id = newItems[i].value || newItems[i].id || 0;
          }
          else if (typeof newItems[i] == 'Number') {
            id = newItems[i];
          }
          selected.push(id);
        }

        if (selected.length) {
          openAncestors();
        }
      });

      attr.$observe('filter', function (value) {
        scope.filterString = value;
      })

      /**
       * Loop backwards through the flat tree, looking for children that have been selected
       * then setting the ancestors of any selected children to be expanded
       */
      function openAncestors() {
        var parent = 0;
        for (var i = scope.flatTree.length - 1; i >= 0; i--) {
          if (scope.flatTree[i].depth && scope.isChecked(scope.flatTree[i].value)) {
            parent = scope.flatTree[i].parent;
          }

          if (scope.flatTree[i].value == parent) {
            scope.flatTree[i].collapsed = false;
            if (scope.flatTree[i].depth) {
              parent = scope.flatTree[i].parent;
            }
          }
        }
      }

      scope.parentCollapsed = function (parent) {
        var node = findNode(parent);
        if (node) {
          return node.collapsed;
        }
        return true;
      }

      function findNode(id) {
        var i = 0,
          l = scope.flatTree.length;

        for(;i<l;i++) {
          if (scope.flatTree[i].value == id) {
            return scope.flatTree[i];
          }
        }
      }

      scope.isChecked = function (id) {
        var i = 0,
          l = selected.length;

        for(;i<l;i++) {
          if (selected[i] == id) {
            return true;
          }
        }

        return false;
      }

      scope.toggleNode = function (node) {
        scope.onChange({$node: node})
      }

    }

    /**
     * Given a node, returns an array with the node and all its descendants on the same level
     * @param node
     */
    function flattenTree(node) {
      var output = [],
        depth = (node.depth !== undefined) ? node.depth : 0;
      node.collapsed = true;

      if (node.depth === undefined) {
        node.depth = depth;
      }
      output.push(node);
      if (Array.isArray(node.children)) {
        node.hasChildren = true;
        node.isLeaf = false;
        for (var i = 0; i < node.children.length; i++) {
          node.children[i].parent = node.value
          node.children[i].depth = depth + 1;
          output = output.concat(flattenTree(node.children[i]));
        }
      }
      else {
        node.isLeaf = true;
        node.hasChildren = false;
      }

      return output;
    }

    return {
      restrict: 'AE',
      scope: {
        tree: '=',  // proper tree
        selected: '=', // flat array of selected ids
        onChange: '&' // event handler to invoke when a node is changed
      },
      link: treeLinker,
      template: '<ul><li ng-repeat="node in flatTree | filter:{label:filterString}" ng-show="node.depth == 0 || !parentCollapsed(node.parent)">' +
        '<span class="spacer" ng-repeat="i in range(node.depth)"></span>' +
        '<span class="expander" ng-class="{collapsed: node.collapsed, empty: !node.hasChildren}" ng-click="node.collapsed = !node.collapsed">&nbsp;</span>' +
        '<input type="checkbox" value="node.value" ng-click="toggleNode(node)" ng-checked="isChecked(node.value)"> {{node.label}}' +
      '</li></ul>'
    };

  }]);
})();