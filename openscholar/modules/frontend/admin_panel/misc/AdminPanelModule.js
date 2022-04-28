(function ($) {
  var paths;
  var vsite;
  var cid;
  var uid;
  var morphButton;

    angular.module('AdminPanel', [ 'os-auth', 'ngCookies','ngStorage', 'RecursionHelper', 'ApSettingsForm', 'AppForm'])
    .config(function (){
       paths = Drupal.settings.paths
       vsite = typeof Drupal.settings.spaces != 'undefined' ? Drupal.settings.spaces.id : 0;
       cid = Drupal.settings.admin_panel.cid + Drupal.settings.version.adminPanel;
       uid = Drupal.settings.admin_panel.user;

    }).service('adminMenuStateService', ['$sessionStorage', '$cookies', function ($ss, $cookies) {
      $ss['menuState'] = $ss['menuState'] || {};
      var cookieConfig = {
        path: Drupal.settings.basePath + Drupal.settings.pathPrefix
      };

      this.SetState = function (key, state) {
        $ss['menuState'][key] = state;
        if (key == 'main') {
          $cookies.put('AdminMenuState', state?1:0, cookieConfig)
        }
      }

      this.GetState = function (key) {
        return $ss['menuState'][key] === true;
      }

      this.ChkLocalStorage = function () {
        try {
          localStorage.setItem('testLocalStorage', 'test');
          localStorage.removeItem('testLocalStorage');
          return true;
        } catch(e) {
          return false;
        }
      }

    }])
    .controller("AdminMenuController",['$scope', '$http', 'adminMenuStateService', '$localStorage', function ($scope, $http, $menuState, $localStorage) {

      var menu = 'admin_panel';
      $scope.paths = paths;

      $scope.getListStyle = function(id) {
        if ($menuState.GetState(id)) {
          return {'display':'block'};
        }
        return {};
      };

      //Force menu open Special case
       if (window.location.search.indexOf('login=1') > -1) {
         $menuState.SetState('main', true);
       }
      //Init storage
      if ($menuState.ChkLocalStorage()) {
        $localStorage.admin_menu = $localStorage.admin_menu || {};
        $localStorage.admin_menu[uid] = $localStorage.admin_menu[uid] || {};
        $localStorage.admin_menu[uid][vsite] = $localStorage.admin_menu[uid][vsite] || {};

        // Check for the menu data in local storage.
        if ($localStorage.admin_menu[uid][vsite][cid]) {
          $scope.admin_panel = $localStorage.admin_menu[uid][vsite][cid];

          $scope.open = $menuState.GetState('main');
          return;
        }
      }

      //Go get the admin menu from the server.
      params = { cid: cid, uid: uid};
      if (vsite) {
        params.vsite = vsite;
      }

      var url = paths.api + '/cp_menu/' + menu;
      $http({method: 'get', url: url, params: params, cache: true}).
        then(function(response) {
          //Clear out old entries.
          if ($menuState.ChkLocalStorage()) {
            $localStorage.admin_menu[uid][vsite] = {};
            $localStorage.admin_menu[uid][vsite][cid] = response.data.data;
          }
          $scope.admin_panel = response.data.data;
          $scope.open = $menuState.GetState('main');
        });


    }]).directive('toggleOpen', ['$cookies', '$timeout', 'adminMenuStateService', function($cookies, $t, $menuState) {

      function openLink(elm) {
        elm.addClass('open');
        elm.children('ul').slideDown(200);
      }

      function closeLink(elm) {
        elm.removeClass('open');
        elm.children('ul').css('display', 'none');
        //elm.find('li').removeClass('open');
        //elm.find('ul').slideUp(200);
      }

      function getAncestor(elem, tagName) {
        tagName = tagName.toUpperCase();
        while (elem[0].tagName != tagName) {
          if (elem[0].tagName == 'BODY') {
            return false;
          }
          elem = elem.parent();
        }

        return elem;
      }

      return {
        link: function(scope, element, attrs) {
          var parent = getAncestor(element, 'li');

          if ($menuState.GetState(attrs.id)) {
            $t(function () {
              openLink(parent);
            });
          }

          element.bind('click', function() {
            // close all sibling links regardless of what type of link this is
            var isOpen = $menuState.GetState(attrs.id);

            if ( element.hasClass('close-siblings') ) {
              var li = element.parent().parent();
              var togglers = angular.element(li.parent()[0].querySelectorAll('li.open'));

              togglers.each(function() {
                var sibling = angular.element(this),
                  id = sibling.children().first().find("span").attr('id');
                $menuState.SetState(id, false);
                closeLink(sibling);
              });
            }

            // if this link has children, toggle their visibility.
            if (element.hasClass('toggleable')){
              element.removeAttr('href');

              if (!isOpen) {
                $menuState.SetState(attrs.id, true);
                openLink(parent);
              }
            }
          });
        },
      };

    }]).directive('leftMenu', ['$timeout', 'adminMenuStateService', function($t, $menuState) {

      return {
        templateUrl: paths.adminPanelModuleRoot+'/templates/admin_menu.html?vers='+Drupal.settings.version.adminPanel,
        controller: 'AdminMenuController',
        link: function(scope, element, attrs) {
          angular.element(element[0].querySelectorAll('.close-menu-panel')).click(function () {
            scope.open = !scope.open;
            $menuState.SetState('main', scope.open);
            setClass();
          });
          var body = angular.element(document).find('body');
          angular.element(element[0].querySelectorAll('#cssmenu')).simplebar();

          function setClass() {
            if (scope.open) {
              element.removeClass('closed');
              body.removeClass('admin-menu-closed');
            }
            else {
              element.addClass('closed');
              body.addClass('admin-menu-closed');
            }
          }

          scope.open = $menuState.GetState('main');
          setClass();
        }
      };
   }]).directive('addLocation', function() {
    //For Qualtrics URL Remove after beta
      return {
        link: function(scope, element, attrs) {
        element.attr('href', attrs.href + '?osurl=' + encodeURIComponent(location.href) + '&uid=' + uid);
        },
      }
    })
    .directive('adminPanelMenuRow', ['$compile', 'RecursionHelper', function ($compile, RecursionHelper) {

      function link(scope, elem, attrs) {
        scope.getListStyle = function (id) {
          if (typeof(menu_state) !== 'undefined' && typeof(menu_state[id]) !== 'undefined' && menu_state[id]) {
            return {'display':'block'};
          }
          return {};
        };

        scope.isActive = function (row) {
          if (row.children) {
            for (var k in row.children) {
              var c = row.children[k];
              if (scope.isActive(c)) {
                return true;
              }
            }
            return false;
          }
          else if (row.type == 'link' && row.href == location.href) {
            return true;
          }
          else {
            return false;
          }
        }
      }

      return {
        scope: {
          menuRow: '=',
          key: '@'
        },
        templateUrl: paths.adminPanelModuleRoot + '/templates/adminPanelMenuRow.template.html?vers=' + Drupal.settings.version.adminPanel,
        compile: function (element) {
          // workaround so directives can be nested
          return RecursionHelper.compile(element, link);
        }
      };
    }])
    /**
     * Allows directives to be added dynmically to this element at runtime
     */
    .directive('adminPanelDirectiveLink', ['$compile', function ($compile) {
        return {
          link: function (scope, elem, attrs) {
            var copy = elem.find('span').clone();
            var directives = scope.menuRow.directive;
            for (var k in directives) {
              if (!isNaN(parseFloat(k)) && isFinite(k)) {
                copy.attr(directives[k], '');
              }
              else {
                copy.attr(k, directives[k]);
              }
            }

            copy = $compile(copy)(scope);
            elem.find('span').replaceWith(copy);
          }
        }
    }]);


})(jQuery);
