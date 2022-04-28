(function () {

  var m = angular.module("DependencyManager", []);

  m.provider("Dependencies", function () {
    var dependencies = {};

    this.AddDependency = function (mod, dep) {
      dependencies[mod] = dependencies[mod] || [];

      dependencies[mod].push(dep);
    };

    this.$get = [function () {
      return {
        GetDependencies: function(mod) {
          if (!dependencies[mod]) {
            return [];
          }
          return dependencies[mod].slice(0);
        }
      }
    }];
  })

})();