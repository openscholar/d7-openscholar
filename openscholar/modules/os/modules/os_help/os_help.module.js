(function () {
  var m = angular.module('osHelp', []);

  m.directive('documentation', [function () {
    return {
      templateUrl: Drupal.settings.paths.documentation,
      link: function (scope) {
        scope.base_url = window.location.origin+Drupal.settings.basePath;
      }
    };
  }]);

  m.directive('support', [function () {
    return {
      templateUrl: Drupal.settings.paths.support,
      link: function (scope) {
        scope.base_url = window.location.origin+Drupal.settings.basePath;
        scope.os_support_mail_to = Drupal.settings.os_support_mail_to;
      }
    };
  }]);
})();
