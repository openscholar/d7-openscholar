(function () {
  var rootPath, paths, defaultIndividualScholar, defaultProjectLabSmallGroup, defaultDepartmentSchool;

  var m = angular.module('SiteCreationForm', ['angularModalService', 'ngMessages', 'os-buttonSpinner', 'os-auth', 'ActiveUser', 'DependencyManager', 'ngSanitize'])
  .config(function (){
    rootPath = Drupal.settings.paths.siteCreationModuleRoot;
    paths = Drupal.settings.paths;
    defaultIndividualScholar = Drupal.settings.site_creation.default_individual_scholar;
    defaultProjectLabSmallGroup = Drupal.settings.site_creation.default_project_lab_small_group;
    defaultDepartmentSchool = Drupal.settings.site_creation.default_department_school;
  });

  m.run(['Dependencies', function (dm) {
    var deps = dm.GetDependencies('UserPanel');
    Array.prototype.push.apply(m.requires, deps);
  }]);

  m.service('passwordStrength', [function () {
    var tests = [/[0-9]/, /[a-z]/, /[A-Z]/, /[^A-Z-0-9]/i];
    this.checkStrength = function(pass) {
      if (pass == null) {
        return -1;
      }
      var s = 0;
      if (pass.length < 6) {
        return 0;
      }
      for (var i in tests) {
        if (tests[i].test(pass)) {
          s++;
        }
      }
      return s;
    }
  }]);

  m.controller('siteCreationCtrl', ['$scope', '$http', '$q', '$rootScope', 'buttonSpinnerStatus', '$filter', '$sce', '$timeout', 'passwordStrength', 'authenticate-token', 'ActiveUserService', 'parent', function($scope, $http, $q, $rootScope, bss, $filter, $sce, $timeout, ps, at, aus, parent) {

  //Set default value for vsite
  $scope.vsite_private = {
    value: '0'
  };
  $scope.privacyLevels = Drupal.settings.site_creation.privacy_levels;
  $scope.trustAsHtml = function (arg) {
    return $sce.trustAsHtml(arg);
  }

  var user;
  aus.getUser(function (u) {
    user = u;

    var types = {
        'create personal content': true,
        'create project content': true,
        'create department content': true
      },
      typecount = 0;
    for (var perm in user.permissions) {
      if (types[perm] != undefined) {
        if (user.permissions[perm]) {
          typecount++;
        }
        types[perm] = user.permissions[perm];
      }
    }
    if (typecount == 1) {
      for (perm in user.permissions) {
        if (types[perm]) {
          switch (perm) {
            case 'create personal content':
              $scope.display = 'individualScholar';
              break;
            case 'create project content':
              $scope.display = 'projectLabSmallGroup';
              break;
            case 'create department content':
              $scope.display = 'department';
              break;
          }
        }
      }
    }
  });

  // Set site creation status
  $scope.siteCreated = false;
  $scope.tos = Drupal.settings.site_creation.tos_url;
  $scope.tos_label = Drupal.settings.site_creation.tos_label;

  var presetList = Drupal.settings.site_creation.presets;
  $scope.presets = function (type) {
    var output = [];
    for (var i in presetList) {
      if (presetList[i].site_type == type) {
        output.push(presetList[i]);
      }
    }
    return output;
  };

  // Initialize the $timout var
  var timer;

  //Toggle open/close for 'who can view your site'
  $scope.showAll = false;
  $scope.toggleFunc = function() {
    $scope.showAll = !$scope.showAll;
  };

  //Reset value for other 'Type of site' based on selection
  $scope.clearRest = function(field) {
    if (field != 'individualScholar') {
      $scope.individualScholar = null;
    }
    if (field != 'projectLabSmallGroup') {
      $scope.projectLabSmallGroup = null;
    }
    if (field != 'departmentSchool') {
      $scope.departmentSchool = null;
    }
  };

  //Set status of next button to disabled initially
  $scope.btnDisable = true;
  $scope.vicariousUser = Drupal.settings.paths.hasOsId;
  $scope.siteNameValid = false;
  $scope.newUserResistrationEmail = false;
  $scope.newUserResistrationName = false;
  $scope.newUserResistrationPwd = false;
  $scope.newUserValidPwd = false;
  $scope.newUserResistrationPwdMatch = false;
  $scope.tosChecked = !$scope.tos;  // circumvent if no tos was provided by site

  //Navigate between screens
  $scope.page1 = true;
  $scope.navigatePage = function(pagefrom, pageto) {
    $scope[pagefrom] = false;
    $scope[pageto] = true;
    if (pagefrom == 'page1' && pageto == 'page2') {
      if ($scope.individualScholar != null) {
        $scope.contentOption = {
          value: defaultIndividualScholar
        };
      } else if ($scope.projectLabSmallGroup != null) {
         $scope.contentOption = {
          value: defaultProjectLabSmallGroup
        };
      } else {
        $scope.contentOption = {
          value: defaultDepartmentSchool
        };
      }
    } else if (pagefrom == 'page2' && pageto == 'page3') {
      var featuredThemeTop = angular.element(document.querySelectorAll('.featured-scrolltop')).position().top;
      featuredThemeTop = featuredThemeTop > 0 ? featuredThemeTop : 650;
      angular.element(document.querySelectorAll('#body-container-page3')).animate({ scrollTop: featuredThemeTop + 200}, 200);
    }
  };

  $scope.canBeCreated = function (type) {
    if (user && !user.permissions['create ' +type+' content']) {
      return false;
    }

    if (!parent) {
      return true;
    }

    return Drupal.settings.site_creation.subsite_types[type] != undefined;
  };

  var queryArgs = {};
  if (Drupal.settings.spaces != undefined) {
    if (Drupal.settings.spaces.id) {
      queryArgs.vsite = Drupal.settings.spaces.id;
    }
  }
  var config = {
    params: queryArgs
  };
  $http.get(paths.api+'/themes', config).then(function (response) {
    $scope.themes = response.data.data;
  });
  $scope.selected = false;
  $scope.selectedOption = {key: 'default'};
  $scope.setTheme = function(themeKey, flavorKey) {
    $scope.selected = themeKey + '-os_featured_flavor-' + flavorKey;
  };

  $scope.changeSubTheme = function(item, themeKey) {
    angular.forEach($scope.themes.others, function(value, key) {
      if (value.themeKey == themeKey) {
        $scope.themes.others[key].flavorKey = item.key;
        angular.forEach(value.flavorOptions, function(v, k) {
          if (v.key == item.key) {
            if (v.screenshot.indexOf('png') > -1) {
              $scope.themes.others[key].screenshot = v.screenshot;
            } else {
              $scope.themes.others[key].screenshot = $scope.themes.others[key].defaultscreenshot;
            }
          }
        });
      }
    });
  };

  $scope.navigateToSite = function() {
    window.location.href = $scope.vsiteUrl;
  };

  //Set default value for Content Option
  $scope.contentOption = {
    value: 'os_department_minimal'
  };
  $scope.selected = 'hwpi_classic-os_featured_flavor-default';
  //Site URL
  $scope.baseURL = Drupal.settings.admin_panel.purl_base_domain + '/';

  //Get all values and save them in localstorage for use
  $scope.saveAllValues = function() {
    bss.SetState('site_creation_form', true);
    $timeout.cancel(timer);

    $scope.btnDisable = true;
    var url = $scope.individualScholar ? $scope.individualScholar : ($scope.projectLabSmallGroup ? $scope.projectLabSmallGroup : $scope.departmentSchool),
      bundle = $scope.individualScholar ? 'personal' : ($scope.projectLabSmallGroup ? 'project' : 'department'),
      theme = $scope.selected;

    var user = {};
    if (Drupal.settings.user_panel != undefined) {
      // existing user who came to the page logged in
      user.uid = Drupal.settings.user_panel.user.uid;
    }
    else if ($scope.vicariousUser) {
      // brand new user
      user.first_name = $scope.fname;
      user.last_name = $scope.lname;
      user.mail = $scope.email;
      user.name = $scope.userName;
      user.password = $scope.confirmPwd;

      $scope.submitStateText = 'User Information...';
      $http.post(Drupal.settings.paths.api+'/users', user).then(function (response) {
        // we were just logged in. We need to fetch the RESTful authentication token
        at.fetch().then (function (resp) {
          submitGroup(response.data.data.id, url, bundle, $scope.contentOption.value, theme, $scope.vsite_private.value);
          return resp;
        });
      });
    }

    if (user.uid) {
      submitGroup(user.uid, url, bundle, $scope.contentOption.value, theme, $scope.vsite_private.value)
    }

    // var formdata = {
    //   individualScholar: $scope.individualScholar,
    //   projectLabSmallGroup: $scope.projectLabSmallGroup,
    //   departmentSchool: $scope.departmentSchool,
    //   vsite_private: $scope.vsite_private.value,
    //   contentOption: $scope.contentOption.value,
    //   vicarious_user: $scope.vicariousUser,
    //   name: $scope.userName,
    //   first_name: $scope.fname,
    //   last_name: $scope.lname,
    //   mail: $scope.email,
    //   password: $scope.confirmPwd,
    //  };
    //
    // // Send the theme key
    // if (typeof $scope.selected !== 'undefined') {
    //   formdata['themeKey'] = $scope.selected;
    // }
    // $http.post(paths.api + '/purl', formdata).then(function (response) {
    //   $scope.successData = response.data.data.data;
    //   $scope.vsiteUrl = response.data.data.data;
    //   $scope.siteCreated = true;
    //   bss.SetState('site_creation_form', false);
    // });
  };

  function submitGroup(owner, url, bundle, starter, theme, privacy) {
    var fields = {
      owner: owner,
      label: url,
      type: bundle,
      purl: url,
      preset: starter,
      theme: theme,
      privacy: privacy
    };
    if (parent) {
      fields.parent = parent;
    }
    $http.post(Drupal.settings.paths.api+'/group', fields).then(function (response) {
      if (response.data.data[0].id != undefined) {
        bss.SetState('site_creation_form', false);
        $scope.submitSuccess = true;
        $scope.submitting = false;
        $scope.submitted = true;
        $scope.siteCreated = true;
        if (response.data['batch-id']) {
          var link = document.createElement('a');
          link.href = response.data.data[0].url;

          document.cookie = 'has_js=1;domain='+link.hostname+'path=/';
          $scope.vsiteUrl = response.data.data[0].url + '/batch?id=' + response.data['batch-id'] + '&op=start';
        } else {
          $scope.vsiteUrl = response.data.data[0].url;
        }
      }
      // gotta figure out what an error looks like
    }, function (errorResponse) {
      bss.SetState('site_creation_form', false);
      $scope.submitError = errorResponse.data.title;
      $scope.submtitted = true;
      $scope.submitting = false;
    });
  }

  $scope.checkUserName = function() {
    $scope.newUserResistrationName = false;
    if (typeof $scope.userName !== 'undefined' && $scope.userName != '') {
      var formdata = {
        name: $scope.userName
      };
      $http.post(paths.api + '/purl/name', formdata).then(function (response) {
        if (response.data.data.length == 0) {
          $scope.showUserError = false;
          $scope.userErrorMsg = '';
          $scope.newUserResistrationName = true;
        } else {
          $scope.showUserError = true;
          $scope.userErrorMsg = $sce.trustAsHtml(response.data.data[0]);
        }
      });
    }
    $scope.isCompletedRes();
  };

  $scope.checkEmail = function() {
    $scope.newUserResistrationEmail = false;
    if (typeof $scope.email !== 'undefined' && $scope.email != '') {
      var formdata = {
        email: $scope.email
      };
      $http.post(paths.api + '/purl/email', formdata).then(function (response) {
        if (response.data.data.length == 0) {
          $scope.showEmailError = false;
          $scope.emailErrorMsg = '';
          $scope.newUserResistrationEmail = true;
        } else {
          $scope.showEmailError = true;
          $scope.emailErrorMsg = $sce.trustAsHtml(response.data.data[0]);
        }
      });
    }
    $scope.isCompletedRes();
  };

  $scope.checkPwd = function() {
    $scope.newUserResistrationPwd = false;
    if (typeof $scope.password !== 'undefined' && $scope.password != '') {
      var formdata = {
        password: $scope.password
      };
      $http.post(paths.api + '/purl/pwd', formdata).then(function (response) {
        if (response.data.data.length == 0) {
          $scope.showPwdError = false;
          $scope.pwdErrorMsg = '';
          $scope.newUserResistrationPwd = true;
        } else {
          $scope.showPwdError = true;
          $scope.pwdErrorMsg = $sce.trustAsHtml(response.data.data[0]);
        }
      });
    }
    $scope.isCompletedRes();
  };

  $scope.isCompletedRes = function() {
    timer = $timeout(function () {
      if ($scope.newUserResistrationEmail && $scope.newUserResistrationName && $scope.newUserValidPwd && $scope.newUserResistrationPwd && $scope.siteNameValid && $scope.newUserResistrationPwdMatch) {
        $scope.btnDisable = false;
      } else {
        $scope.btnDisable = true;
      }
    }, 2000);
  };

  $scope.validateForms = function() {
      return $scope.btnDisable || !$scope.tosChecked;
  };

  $scope.score = function() {
    $scope.newUserValidPwd = false;
    var pwdScore = ps.checkStrength($scope.password);
    if (pwdScore < 1 ) {
      $scope.strength = "At least 6 characters";
    } else if (pwdScore == 1) {
      $scope.strength = "Weak";
    } else if (pwdScore == 2) {
      $scope.strength = "Good";
    } else if (pwdScore == 3) {
      $scope.strength = "Fair";
    } else if (pwdScore > 3) {
      $scope.strength = "Strong";
    }
    if (pwdScore > 0) {
      $scope.newUserValidPwd = true;
    }
    return pwdScore;
  };

 $scope.pwdMatch = function() {
  $scope.newUserResistrationPwdMatch = false;
  if (typeof $scope.password !== 'undefined' && $scope.password != '') {
    if (angular.equals($scope.password, $scope.confirmPwd)) {
      $scope.newUserResistrationPwdMatch = true;
      $scope.isCompletedRes();
      return 'yes';
    } else {
      return 'no';
    }
  } else {
    return '';
  }
 }
}]);
  /**
   * Open modals for the site creation forms
   */
  m.directive('siteCreationForm', ['ModalService', '$rootScope', function (ModalService,$rootScope) {
    var dialogOptions = {
      minWidth: 900,
      minHeight: 300,
      modal: true,
      position: 'center',
      dialogClass: 'site-creation-form'
    };

    function link(scope, elem, attrs) {
      elem.bind('click', function (e) {
        e.preventDefault();
        e.stopPropagation();
        $rootScope.siteCreationFormId = attrs.id;

        openModal(scope);
      });

      if (scope.autoOpen) {
        openModal(scope);
      }

      function openModal(scope) {
        ModalService.showModal({
          controller: 'siteCreationCtrl',
          templateUrl: rootPath + '/templates/os_site_creation.html',
          inputs: {
            form: scope.form,
            parent: scope.parent
          }
        })
        .then(function (modal) {
          dialogOptions.title = scope.title;
          dialogOptions.close = function (event, ui) {
            modal.element.remove();
          };
          modal.element.dialog(dialogOptions);
          modal.close.then(function (result) {
            if (result) {
              window.location.reload();
            }
          });
        });
      }
    }

    return {
      link: link,
      scope: {
        form: '@',
        autoOpen: '@?',
        parent: '@'
      }
    };
  }]);

//Validate form for existing site names
  m.directive('formcheckDirective', ['$http', function($http) {
  var responseData;
  return {
    require: 'ngModel',
    link: function(scope, element, attr, siteCreationCtrl) {
      function formValidation(ngModelValue) {
        siteCreationCtrl.$setValidity('isinvalid', true);
        siteCreationCtrl.$setValidity('sitename', true);
        siteCreationCtrl.$setValidity('permission', true);
        scope.btnDisable = true;
        var baseUrl = Drupal.settings.paths.api;
        if(ngModelValue){
          //Ajax call to get all existing sites
          $http.get(baseUrl + '/purl/' + encodeURIComponent(ngModelValue)).then(function mySuccess(response) {
            responseData = response.data.data;
            if (responseData.msg == "Not-Permissible") {
              siteCreationCtrl.$setValidity('permission', false);
              siteCreationCtrl.$setValidity('isinvalid', true);
              siteCreationCtrl.$setValidity('sitename', true);
              scope.btnDisable = true;
              scope.siteNameValid = false;
            }
            else if (responseData.msg == "Invalid"){
              siteCreationCtrl.$setValidity('permission', true);
              siteCreationCtrl.$setValidity('isinvalid', false);
              siteCreationCtrl.$setValidity('sitename', true);
              scope.btnDisable = true;
              scope.siteNameValid = false;
            }
            else if (responseData.msg == "Not-Available") {
              siteCreationCtrl.$setValidity('permission', true);
              siteCreationCtrl.$setValidity('isinvalid', true);
              siteCreationCtrl.$setValidity('sitename', false);
              scope.btnDisable = true;
              scope.siteNameValid = false;
            }
            else{
              siteCreationCtrl.$setValidity('permission', true);
              siteCreationCtrl.$setValidity('isinvalid', true);
              siteCreationCtrl.$setValidity('sitename', true);
              scope.siteNameValid = true;
              if (scope.vicariousUser) {
                scope.isCompletedRes();
              } else {
                scope.btnDisable = false;
              }
            }
          }, function (response) {
            // this triggers if the entered URL has a slash or backslash in it
            if (response.status == 404 || response.status == -1) {
              siteCreationCtrl.$setValidity('permission', true);
              siteCreationCtrl.$setValidity('isinvalid', false);
              siteCreationCtrl.$setValidity('sitename', true);
              scope.btnDisable = true;
              scope.siteNameValid = false;
            }
            else {
              // error on server end
            }
          });
        }
        return ngModelValue;
      }
      siteCreationCtrl.$parsers.push(formValidation);
    }
  };
}]);
jQuery(document).ready(function(){
  var highestBox = 0;
  jQuery('.starter-content .form-item').each(function(){
    if (jQuery(this).height() > highestBox) {
      highestBox = jQuery(this).height();
    }
  });
  jQuery('.starter-content .form-item').height(highestBox);

});

})();


