(function () {

  angular.module('locationFix', [])
    .config(['$provide', function ($provide) {
      $provide.decorator('$browser', ['$delegate', function ($delegate) {
        $delegate.onUrlChange = function () {
        };
        $delegate.url = function () {
          return ""
        };
        return $delegate;
      }]);
    }]);
})();