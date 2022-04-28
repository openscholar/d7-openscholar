(function ($) {
  Drupal.behaviors.osSessionTimeout = {
    attach: function (context) {
      Drupal.settings.os.current_timestamp = Math.round(new Date().getTime()/1000);
      if (ChkLocalStorage()) {
        // On page load, saving the localStorage key values, according to these values, other tabs will update their clocks.
        localStorage.setItem('last_hit_timestamp', Drupal.settings.os.current_timestamp);
        localStorage.setItem('session_expire_timestamp', Drupal.settings.os.current_timestamp + parseInt(Drupal.settings.os.session_lifetime));
        localStorage.setItem('warning_display_timestamp', Drupal.settings.os.current_timestamp + parseInt(Drupal.settings.os.session_lifetime) - parseInt(Drupal.settings.os.warning_interval_before_timeout));
      } else {
        $.cookie('last_hit_timestamp', Drupal.settings.os.current_timestamp);
        $.cookie('session_expire_timestamp', Drupal.settings.os.current_timestamp + parseInt(Drupal.settings.os.session_lifetime));
        $.cookie('session_expire_timestamp', Drupal.settings.os.current_timestamp + parseInt(Drupal.settings.os.session_lifetime) - parseInt(Drupal.settings.os.warning_interval_before_timeout));
      }
      // Starting the timer to determine when to display warning message and refresh the the after session timeout.
      // Every 1 sec interval, values of the above variables will be compared so that timing in all tabs are synced +/-3 secs 
      setInterval(checkSessionStatus, 1000);
    }
  }

  // Every 1 sec of interval, this function is called to determine the eligibilty of displaying timeout warning message and redirect user after session timeout.
  function checkSessionStatus() {
    if (ChkLocalStorage()) {
      // Obtaining values from browser local storage.
      last_hit_timestamp = localStorage.getItem('last_hit_timestamp');
      session_expire_timestamp = localStorage.getItem('session_expire_timestamp');
      warning_display_timestamp = localStorage.getItem('warning_display_timestamp');
    } else {
      last_hit_timestamp = $.cookie('last_hit_timestamp');
      session_expire_timestamp = $.cookie('session_expire_timestamp');
      warning_display_timestamp = $.cookie('warning_display_timestamp');
    }
    // Incrementing timestamp counter by 1 sec.
    Drupal.settings.os.current_timestamp++;
    // Checking if current timestamp value meets the criteria to display warning message or not.
    if (Drupal.settings.os.current_timestamp == warning_display_timestamp && !jQuery('#timeout-warning-wrapper').length) {
      displayTimeoutWarning();
    }
    // Hiding the warning message, if any other tabs are opened/refreshed/extend session link clicked on other tabs.
    if (Drupal.settings.os.current_timestamp < warning_display_timestamp && jQuery('#timeout-warning-wrapper').length) {
      // After displaying the warning, if another tab is refreshed, then hiding the warning msg.
      jQuery('#timeout-warning-wrapper').slideUp('slow', function(){jQuery('#timeout-warning-wrapper').remove();});
    }
    // If current timestamp reaches session expire timestamp, triggering ajax callback for session destroy and reloading the page.
    if (Drupal.settings.os.current_timestamp == session_expire_timestamp) {
      expireCurrentSession();
    }
  }

  // Callback to display session timeout warning message.
  function displayTimeoutWarning(){ 
    jQuery.ajax({
      url: Drupal.settings.basePath + 'check_os_session_status',
      type: 'get',
      dataType:'json',
      success: function(jData) {
        if (jData.show_warning == 1) {
          jQuery('#page').prepend('<div id="timeout-warning-wrapper" class="messages warning element-hidden"><div class="message-inner"><div class="message-wrapper">Warning: Your session will expire in <span id="session-timeout-timer" data-seconds-left=' + Drupal.settings.os.warning_interval_before_timeout + '></span> minutes. <a href="javascript:extend_os_session();" class="session-extend-link">Click here to continue your session.</div></div></div>');
          jQuery('#timeout-warning-wrapper').slideDown('slow');
          jQuery('#session-timeout-timer').startTimer();
        }
      }
    });
  }

  // Callback to destroy drupal session via ajax and reloading the current page.
  function expireCurrentSession(){
    jQuery.get(Drupal.settings.basePath + 'os_session_destroy', function(data) {
      location.reload(true);
    });
  }

})(jQuery);

// Ajax callback to regenerate Drupal session and extend session timeout.
function extend_os_session() {
  jQuery.ajax({
    url: Drupal.settings.basePath + 'extend_os_session',
    type: 'get',
    dataType:'json',
    success: function(jData) {
      // Hiding warning message div.
      jQuery('#timeout-warning-wrapper').slideUp('slow', function(){jQuery('#timeout-warning-wrapper').remove();});
      var current_timestamp = Math.round(new Date().getTime()/1000);
      if (ChkLocalStorage()) {
        localStorage.setItem('last_hit_timestamp', current_timestamp);
        localStorage.setItem('session_expire_timestamp', current_timestamp + parseInt(Drupal.settings.os.session_lifetime));
        localStorage.setItem('warning_display_timestamp', current_timestamp + parseInt(Drupal.settings.os.session_lifetime) - parseInt(Drupal.settings.os.warning_interval_before_timeout));
      } else {
        $.cookie('last_hit_timestamp', current_timestamp);
        $.cookie('session_expire_timestamp', current_timestamp + parseInt(Drupal.settings.os.session_lifetime));
        $.cookie('warning_display_timestamp', current_timestamp + parseInt(Drupal.settings.os.session_lifetime) - parseInt(Drupal.settings.os.warning_interval_before_timeout));
      }
    }
  });
}

// Check for localStorage
function ChkLocalStorage() {
  try {
    localStorage.setItem('testLocalStorage', 'test');
    localStorage.removeItem('testLocalStorage');
    return true;
  } catch(e) {
    return false;
  }
}