(function () {
	var rootPath;
  var restPath;
  var vsite;
  var notify_settings;
  var user_data;
  var paths;

  angular.module('UserPanel', ['AdminPanel', 'ActiveUser'])
  .config(function () {
    rootPath = Drupal.settings.paths.adminPanelModuleRoot;
    restPath = Drupal.settings.paths.api;

    if(typeof(Drupal.settings.spaces) == 'undefined') {
      vsite = false;
    } else {
      vsite = Drupal.settings.spaces.id;
    }

    user_data = Drupal.settings.user_panel.user;
    paths = Drupal.settings.paths;
  }).controller("UserMenuController",['$scope', '$http', '$timeout', function ($scope, $http, $timeout) {
    
      $scope.user = user_data;
      $scope.vsite = { id: vsite };
      $scope.paths = paths;
      $scope.close_others = function(id){
    	//Protect the current link
    	if(!jQuery("#rightMenuSlide .click-processing").length) {
          jQuery("#rightMenuSlide [data-id='"+id+"']").addClass('click-processing');
    	}
        // After closing the modal, setting the search text field as blank and resetting search results.
        $scope.searchString = '';
        $timeout(function() {
          jQuery('#rightMenuSlide .menu_modal_open').not("[data-id='"+id+"']").not('.click-processing').click();
          $timeout(function() {
            jQuery("#rightMenuSlide .click-processing").removeClass('click-processing');
          });
      	});
      };

  }]).controller("UserSitesController",['$scope', '$http', 'ActiveUserService', function ($scope, $http, aus) {
    $scope.baseUrl = Drupal.settings.basePath;
    $scope.purlBaseDomain = Drupal.settings.admin_panel.purl_base_domain + "/";
    $scope.baseDomain = Drupal.settings.admin_panel.base_domain + "/";

    aus.getUser(function (user) {
      $scope.site_data = user.og_user_node || [];
      $scope.create_access = user.create_access;
    });

    // var url = paths.api + '/users/' + user_data.uid;
    // $http({method: 'get', url: url}).then(function(response) {
    //   if(typeof(response.data.data[0].og_user_node) == 'undefined') {
    //     $scope.site_data = [];
    //   } else {
    //     $scope.site_data = response.data.data[0].og_user_node;
    //   }
    //   $scope.create_access = response.data.data[0].create_access;
    // });
    $scope.pageSize = 7;
    if (Drupal.settings.spaces) {
      $scope.spaceId = Drupal.settings.spaces.id;
      $scope.delete_destination_root = '?destination=<base_domain>';
      $scope.delete_req_destination_root = '?destination=?<base_domain>';
      $scope.delete_destination = '?'+encodeURIComponent('destination=node/' + Drupal.settings.spaces.id + encodeURIComponent('?destination=' + window.location.pathname.replace(/^\/|\/$/g, '')));
    } else {
      $scope.delete_destination = '';
    }
    $scope.numberOfPages=function(data){
      return Math.ceil(data.length/$scope.pageSize);
    }
  }]).directive('rightMenu', function() {
      return {
       templateUrl: rootPath+'/templates/user_menu.html?vers='+Drupal.settings.version.userPanel,
       controller: 'UserMenuController',
     };
  }).directive('addDestination', function() {
      //Add the destination to a URL
	  return {
	    link: function(scope, element, attrs) {
		  var url = (typeof(attrs.href) == 'undefined') ? attrs.ngHref : attrs.href;
	      element.attr('href', url + ((url.indexOf('?') == -1) ? '?' : '&') + 'destination=' + encodeURIComponent(location.href));
  	    },
	  }
  }).directive('loadMySites', function() {
    	 
    return {
        templateUrl: rootPath+'/templates/user_sites.html?vers='+Drupal.settings.version.userPanel,
        controller: 'UserSitesController',
      };
       
  }).directive('rightMenuToggle', function() {
        return {
            link: function(scope, element, attrs) {
        	  element.bind('click', function(e) {
        		  e.preventDefault();
        		  jQuery('div#rightMenuSlide').each( function () {
        	        if(this.style.display == 'none') {
        	          jQuery(this).fadeIn('fast');
        	          jQuery(this).addClass('open');
        	        } else {
        	          jQuery(this).fadeOut('fast');
        	          jQuery(this).removeClass('open');
        	        }
        	      });
              })  
            },
          }
        });
})();
