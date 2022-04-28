(function () {

  var reportModule = angular.module('ReportModule', ['os-auth']);
  reportModule.controller('SiteReportQuery', ['$sce', '$http', '$scope', function ($sce, $http, $scope) {
    $scope.params = {};
    $scope.params.includesites = 'all';
    $scope.report_url = 'report_sites';

    $scope.fieldConversion = {
      'site_owner_email' : {
        'display' : 'site owner email',
        'field_name' : 'u.mail',
      },
      'username' : {
        'field_name' : 'u.name',
      },
      'id' : {
        'display' : 'Vsite ID',
        'field_name' : 'purl.id',
      },
      'site_title' : {
        'display' : 'site title',
        'field_name' : 'n.title',
      },
      'os_install' : {
        'display' : 'os install',
      },
      'other_site_changes' : {
        'display' : 'non-content site changes',
        'sort' : false,
      },
      'site_created' : {
        'display' : 'site created',
      },
      'site_created_by' : {
        'display' : 'site created by',
      },
      'site_privacy_setting' : {
        'display' : 'site privacy setting',
      },
      'custom_domain' : {
        'display' : 'custom domain',
      },
      'custom_theme_uploaded' : {
        'display' : 'custom theme uploaded',
      },
      'content_last_updated' : {
        'display' : 'content last updated',
      },
      'preset' : {
        'display' : 'site type (preset)',
        'sort' : false,
      },
      'site_owner_linked_huid' : {
        'display' : 'site Owner has Associated HUID',
      },
      'site_owner_huid' : {
        'display' : 'site owner HUID',
        'sort' : false,
      },
      'num_nodes' : {
        'display' : 'number of nodes',
      },
      'num_files' : {
        'display' : 'number of files',
      },
      'num_widgets' : {
        'display' : 'number of widgets',
      },
      'num_members' : {
        'display' : 'number of site members',
      },
      'num_redirects' : {
        'display' : 'number of site redirects',
      },
    };

    $scope.pager = function($direction) {
      var url = '';
      eval ('url = $scope.' + $direction + ';');
      $scope.params = convertRestURLtoObj(url);
      $scope.update();
    }

    $scope.updateCheckedValues = function updateCheckedValues($set, $value) {
      if (eval("!$scope.queryform." + $set)) {
        eval ("$scope.queryform." + $set + " = {};");
      }
      $checked = eval("$scope.queryform." + $set + "." + $value);

      if ($scope.fieldConversion[$value] && $scope.fieldConversion[$value]['field_name']) {
        $value = $scope.fieldConversion[$value]['field_name'];
      }

      if ($checked && !$scope.params[$set]) {
        $scope.params[$set] = new Array($value);
      }
      else {
        $valueArray = new Array();
        for ($key in $scope.queryform[$set]) {
          if ($scope.queryform[$set][$key]) {
            if ($scope.fieldConversion[$key] && $scope.fieldConversion[$key]['field_name']) {
              $key = $scope.fieldConversion[$key]['field_name'];
            }
            $valueArray.push($key);
          }
        }
        $scope.params[$set] = $valueArray;
        if ($scope.params[$set].length == 0) {
          delete $scope.params[$set];
        }
      }

      // reset to page 1 and no sort
      $scope.params.page = '1';
      $scope.params.sort = "";
    };

    $scope.updateValues = function updateValues($obj) {
      if ($obj != null) {
        // reset to page 1 and no sort
        $scope.params.page = '1';
        $scope.params.sort = "";
      }
    };

    $scope.update = function update() {
      // make sure request isn't already in process
      if (!jQuery("div.results").attr("style")) {
        // reset values
        $scope.headers = [];
        $scope.rows = [];
        jQuery("div.results").css("background-image", "url('/profiles/openscholar/modules/frontend/os_common/FileEditor/large-spin_loader.gif')");
        jQuery(".pager a").hide();
        $scope.status = "";
        jQuery("div#page #messages").remove();

        $scope.params.exclude = ['feed_importer', 'profile', 'harvard_course'];

        var $request = {
          method: 'POST',
          url : Drupal.settings.paths.api + '/v1/' + $scope.report_url,
          headers : {'Content-Type' : 'application/json'},
          data : $scope.params,
        };

        $http($request).then(function($response) {
          if(!$response.data.data) {
            jQuery("div#page").prepend('<div id="messages"><div class="messages error">' + jQuery($response.data).text() + '</div></div>');
            jQuery("div.results").attr("style", "");
          }
          else {
            var $responseData = angular.fromJson($response.data.data);

            $scope.total = $response.data.count;

            if ($response.data.next != null) {
              $scope.next = $response.data.next.href;
              jQuery(".pager .next a").show();
            }
            else {
              $scope.next = null;
              jQuery(".pager .next a").hide();
            }

            if ($response.data.previous != null) {
              $scope.previous = $response.data.previous.href;
              jQuery(".pager .previous a").show();
            }
            else {
              $scope.previous = null;
              jQuery(".pager .previous a").hide();
            }

            var $keys = [];

            // get table headers from returned data
            for ($key in $responseData[0]) {
              if ($key && $key != "site_url") {
                $scope.headers.push($key);
              }
            }
            jQuery("div.results").attr("style", "");
            $scope.rows = $responseData;

            if (!$scope.params || !$scope.params.page) {
              $scope.params.page = 1;
            }

            if (($scope.params.page * $scope.params.range) < $scope.total) {
              $end = ($scope.params.page * $scope.params.range);
            }
            else {
              $end = $scope.total;
            }

            if ($scope.total) {
              $scope.status = "showing " + ((($scope.params.page - 1) * $scope.params.range) + 1) + " - " + $end + " of " + $scope.total;
            }
            else {
              jQuery("div#page").prepend('<div id="messages"><div class="messages warning">No results in report.</div></div>');
            }
          }
        },
        // error
        function() {
          jQuery("div.results").attr("style", "");
          jQuery("div#page").prepend('<div id="messages"><div class="messages error">An error occurred.</div></div>');
        }
      );
     }
    };

    $scope.reset = function() {
      jQuery("div.results").attr("style", "");
      jQuery("div#page #messages").remove();
      for (var key in $scope.params) {
        if ($scope.params.hasOwnProperty(key) && key != "range") {
          delete $scope.params[key];
        }
      }
      $scope.params.includesites = 'all';
      for (var key in $scope.queryform) {
        delete $scope.queryform[key];
      }
    };

    $scope.sort = function sort($obj) {
      if ($scope.fieldConversion[$obj.header]['sort'] !== false) {
        if ($scope.params.sort && ($scope.params.sort == $obj.header)) {
          $scope.params.sort = "-" + $obj.header;
        }
        else if ($scope.params.sort && ($scope.params.sort == "-" + $obj.header)) {
          delete $scope.params.sort;
        }
        else {
          $scope.params.sort = $obj.header;
        }
        // reset to page 1 and update
        $scope.params.page = '1';
        $scope.update();
      }
    };

    $scope.isActive = function isActive($header) {
      if ($scope.params.sort == $header) {
        return "active desc";
      }
      else if ($scope.params.sort == ("-" + $header)) {
        return "active asc";
      }
      else if ($scope.fieldConversion[$header]['sort'] === false) {
        return "nosort";
      }
    };

    $scope.formatHeader = function formatHeader($header) {
      if ($scope.fieldConversion[$header]) {
        return $sce.trustAsHtml($scope.fieldConversion[$header]['display']);
      }
      else {
        return $sce.trustAsHtml($header);
      }
    };
  }]);

  reportModule.filter('makelink', ['$sce', function($sce) {
    return function($value, $header, $row) {
      if ($header == "site_title" && $value) {
        $html = '<a href="' + $row['site_url'] + '" target="_new">' + $value + '</a>'
        return $sce.trustAsHtml($html);
      }
      else if ($header == "site_title") {
        $html = '<a href="' + $row['site_url'] + '" target="_new">[No Title]</a>'
        return $sce.trustAsHtml($html);
      }
      else {
        return $sce.trustAsHtml($value);
      }
    };
  }]);

  // function to take URL and return a javascript object of the query string
  var convertRestURLtoObj = function (url) {
      var query = decodeURIComponent(url.split('?').slice(1).toString());
      $queryArray = query.split('&');
      $queryObj = new Object();

      jQuery.each($queryArray, function($key, $value) {
        $pair = $value.split('=');
        $arrayFlag = 0;
        if ($pair[0].match(/\[\d+\]/)) {
          $arrayFlag = 1;
          $pair[0] = $pair[0].replace(/\[\d+\]/g, '');
        }
        if ($queryObj[$pair[0]]) {
          if (typeof $queryObj[$pair[0]] === "string") {
            $queryObj[$pair[0]] = new Array($queryObj[$pair[0]], $pair[1]);
          }
          else {
            $queryObj[$pair[0]].push($pair[1]);
          }
        }
        else {
          if ($arrayFlag) {
            $queryObj[$pair[0]] = new Array($pair[1]);
          }
          else {
            $queryObj[$pair[0]] = $pair[1];
          }
        }
      });

      return $queryObj;
  };


})();
