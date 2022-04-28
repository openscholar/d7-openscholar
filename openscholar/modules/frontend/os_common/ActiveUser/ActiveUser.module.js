(function () {
  var m = angular.module('ActiveUser', []);

  m.service('ActiveUserService', ['$http', '$q', function ($http, $q) {
    var user = {
      uid: Drupal.settings.user.uid,
      name: Drupal.settings.user.name,
      permissions: {}
    };
    var restPath = Drupal.settings.paths.api;

    var deferred;
    this.init = function () {
      deferred = $q.defer();
      $http.get(restPath+'/users/'+user.uid).then(function (resp) {
        user = resp.data.data[0];
        deferred.resolve(angular.copy(user));
      });
    };

    this.getUser = function (callback) {
      deferred.promise.then(callback);
    };
    this.init();

  }]);
})();