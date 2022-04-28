/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($, w) {

  w.osTour = {
		  /**
		   * Determines if the feed item is new enough to display to this user.
		   *
		   * @param {object} entry
		   * @returns {bool}
		   */
		  notifications_is_new: function (entry) {
			    var pub_date = new Date(entry.publishedDate);
			    var pub_date_unix = pub_date.getTime() / 1000;
			    var last_read = Drupal.settings.os_notifications.last_read;
			    if (pub_date_unix > last_read) {
			      return true;
			    }

			    return false;
			  },
	     /**
		  * Converts a Google FeedAPI Integration feed item into a hopscotch step.
		  *
		  * @param {object} entry
		  * @returns {string} output
		  */
		  notifications_item: function (entry, num_remaining, hideTeaser, handler, target) {
			    // Prepare the output to display inside the tour's content region.
			    var output = "<div class='feed_item'>";

			    // Adds a date like "5 days ago", or blank if no valid date found.
			    var date = "";
			    /** @FIXME parse entry.contentSnippet to see if it starts with a date first.
			    if (typeof entry.publishedDate != 'undefined' && entry.publishedDate != '') {
			      date = os_tour_notifications_fuzzy_date(entry.publishedDate);
			      if (typeof date === 'undefined') {
			        date = "";
			      } else {
			        date = "<span class='date'>" + date + "</span>";
			      }
			    }
			    */
			    output += date;

			    // Builds the remainder of the content, with a "Read more" link.
			    output += "<span class='description'>";
			    var content = entry.content;
			    if (typeof entry.contentSnippet != 'undefined') {
			      content = entry.contentSnippet;
			    }
			    output += content + "</span>";

          if (! hideTeaser) {
            output += '<div class="os-tour-notifications-readmore"><a target="_blank" href="' + entry.link + '">Read more &raquo;</a></div></div>';
          }

			    // Returns the item to be added to the tour's (array) `items` property .
			    var item = {
			      title: entry.title,
			      content:output,
			      target: target,
			      placement: "bottom",
			      yOffset: -3,
			      xOffset: -10,
			      onShow: function() {
              if (typeof handler == 'function') {
                handler(num_remaining);
              }
			      }
			    };
			    return item;
			  },
	     /**
		  * Takes an ISO time and returns a string with "time ago" version.
		  *
		  * @param time
		  * @returns {string}
		  */
		  notifications_fuzzy_date: function (time) {
			    var date = new Date(time),
			      diff = (((new Date()).getTime() - date.getTime()) / 1000),
			      day_diff = Math.floor(diff / 86400);

			    if (isNaN(day_diff) || day_diff < 0 || day_diff >= 31) {
			      return;
			    }

			    return day_diff == 0 && (
			      diff < 60 && "just now" ||
			        diff < 120 && "1 minute ago" ||
			        diff < 3600 && Math.floor(diff / 60) + " minutes ago" ||
			        diff < 7200 && "1 hour ago" ||
			        diff < 86400 && Math.floor(diff / 3600) + " hours ago") ||
			      day_diff == 1 && "Yesterday" ||
			      day_diff < 7 && day_diff + " days ago" ||
			      day_diff < 31 && Math.ceil(day_diff / 7) + " weeks ago";
			  },
	     /**
		  * Updates the notifications count of remaining notifications.
		  */
		  notifications_count: function (count_element, num_remaining) {
				count_element = $(count_element);
        var value = parseInt(count_element.text());
        if (arguments.length === 0) {
          return value;
        }
        if (parseInt(num_remaining) === -1) {
          count_element.text('0');
          count_element.hide();
          $("#os-tour-notifications-menu-link").slideUp('slow');
          return;
        }
        if (parseInt(num_remaining) > -1) {
          count_element.text(num_remaining);
          if (!isNaN(parseFloat(value)) && isFinite(value)) {
            count_element.show();
            if (num_remaining > value) {
              count_element.text(value);
            }
          }
        }
      },
		 /**
		  * Sets the current user's "notifications_read" to the current time.
		  *
    	  * Invoked when a user clicks "Done" on the final tour step.
	      */
		  notifications_read_update: function () {

			    var settings = Drupal.settings.os_notifications;
			    var url = window.location.origin + Drupal.settings.basePath + '/os/tour/user/' + settings.uid + '/notifications_read';
   			    $.get(url, function(data) {
  			      console.log(data);
  			    });
			  }
  };
  

})(jQuery, window);
