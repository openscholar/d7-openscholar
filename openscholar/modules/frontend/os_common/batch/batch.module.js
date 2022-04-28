
(function () {
  var m = angular.module('batchApi', []);

  m.directive('batchProgressBar', ['$http', '$timeout', function ($http, $t) {
    return {
      template: '<div>' +
        '<div class="progress" aria-live="polite" id="{{id}}">' +
          '<div class="bar"><div class="filled" style="width: {{progress}}%"></div></div>' +
          '<div class="percentage">{{progress}}%</div>' +
          '<div class="message">{{message}}</div>' +
        '</div>' +
      '</div>',
      scope: {
        id: '@batchProgressBar',
        onChange: '&'
      },
      link: function ($s, elem, attr) {
        $s.progress = 0;
        $s.message = 0;

        var host = window.location.host;
        if (Drupal.settings.admin_panel) {
          var a = document.createElement('a');
          a.href = Drupal.settings.admin_panel.purl_base_domain;
          host = 'a.' + a.host;
        }
        else {
          host = host.replace(/[a-zA-Z0-9-]*(\.[a-zA-Z0-9]*\.[a-zA-Z0-9]*(:[\d]+)?)$/, 'a$1')
        }

        $s.$watch('id', function (newVal) {
          if (!newVal) return;

          var params = {
            op: 'do',
            id: newVal
          };
          var conf = {
            headers: {
              'Content-Type': 'application/x-www-form-urlencoded',
            },
            transformRequest: function (obj) {
              var str = [];
              for (var p in obj) {
                str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
              }
              return str.join('&');
            }
          };
          $http.post(url, params, conf).then(handleResponse);
          $s.onChange({status: 'started'});

          function handleResponse(resp) {
            var data = resp.data;
            if (data.status) {
              $s.progress = data.percentage;
              $s.message = data.message;
              if (data.percentage < 100) {
                params.op = 'do';
              }
              else {
                params.op = 'finished';
              }
              $t(function () {
                $http.post(url, params, conf).then(handleResponse);
              }, 1000);

            }
            else {
              if (data.finished) {
                $s.onChange({status: 'finished'});
              }
              else if (data.errored) {
                $s.onChange({status: 'errored'});
              }
            }
          }
        });
      }
    }
  }]);
})();