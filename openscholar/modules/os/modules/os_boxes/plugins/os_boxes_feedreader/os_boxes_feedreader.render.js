
(function ($) {

/**
 * This script reads through feed settings and fetches feed data using the Google FeedAPI
 */
Drupal.behaviors.osBoxesFeedReader = {
  attach: function (context, settings) {
    //Loop through the feeds that are on this page
    $.each(settings.osBoxesFeedReader, function(div_id, feed_settings) {
      if (feed_settings.rss2json_api_key == '') {
        return false;
      }
      // run only once for each feed
      $('div#' + div_id, context).once('osBoxesFeedReader', function () {
        //Run the feed processing only once per feed
        $.ajax({
          url: feed_settings.rss2json_api_url,
          method: 'GET',
          dataType: 'json',
          data: {
            rss_url: feed_settings.url,
            api_key: feed_settings.rss2json_api_key,
            count: feed_settings.num_feeds,
            order_by: 'pubDate'
          }
        }).done(function (response) {
          if(response.status != 'ok'){ throw response.message; }

          if (response.items != null) {
            for(var i in response.items){
              var entry = response.items[i];
              var date = "";
              var dateToFormat = getDateFromEntry(entry);

              if (dateToFormat != null) {
                if (feed_settings.time_display == 'relative') {
                  //@todo find a good way to do FuzzyTime in js
                  date = fuzzyDate(dateToFormat);
                }

                if (feed_settings.time_display == 'formal') {
                  date = formalDate(dateToFormat);
                }

                if (typeof date == 'undefined') {
                  date = "";
                } else {
                  date = "<span class='date'>" + date + "</span>";
                }
              }
              var content = '';
              if (feed_settings.show_content) {
                content = getStringValue(entry.description);
              }
              var feed_markup = "<div class='feed_item'>";

              // Put time before title if there is no content
              if (!feed_settings.show_content) {
                feed_markup = feed_markup + date;
              }

              //Add Title
              feed_markup = feed_markup + "<a class='title' href='" + getStringValue(entry.link) + "' target='_blank'>" + getStringValue(entry.title) + "</a>";
              if (feed_settings.show_content) {
                feed_markup = feed_markup + "<br />" + date + "<span class='description'>" + content + "<span/>";
              }
              feed_markup = feed_markup + "</div>";
              var div = $(feed_markup);

              $('div#' + div_id).append(div);
            }
          }
          else {
            $('div#' + div_id).append('<p class="feed-message">There are currently no items in this feed. Please check again soon.</p>');
          }
        });
      });
    });
  }
};

})(jQuery);


//Takes an ISO time and returns a string representing how
//long ago the date represents.
function fuzzyDate(time){
  var date = new Date(time.split(' ').join('T')),
    diff = (((new Date()).getTime() - date.getTime()) / 1000),
    day_diff = Math.floor(diff / 86400);
      
  if ( isNaN(day_diff) || day_diff < 0 || day_diff > 365) {
    return;
  }

  return day_diff == 0 && (
    diff < 60 && "just now" ||
    diff < 120 && "1 minute ago" ||
    diff < 3600 && Math.floor( diff / 60 ) + " minutes ago" ||
    diff < 7200 && "1 hour ago" ||
    diff < 86400 && Math.floor( diff / 3600 ) + " hours ago") ||
    day_diff == 1 && "Yesterday" ||
    day_diff < 7 && day_diff + " days ago" ||
    Math.ceil( day_diff / 7 ) + " weeks ago";
}

//Takes an ISO time and returns a string representing how
//long ago the date represents.
function formalDate(time){
  var date = new Date(time.split(' ').join('T'));
  var month = date.getMonth();
  var day = date.getDate();
  var year = date.getFullYear();
  var montharray=new Array("January","February","March","April","May","June", "July","August","September","October","November","December");
  return montharray[month]+" "+day+", "+year;
}

function getDateFromEntry(entry){
  if (entry.pubDate != null) {
    if (typeof entry.pubDate === 'string' && entry.pubDate != 'Thu, 01 Jan 1970 00:00:00 +0000') {
      return entry.pubDate;
    }
    else if (typeof entry.pubDate === 'object') {
      if (entry.pubDate.content != null && typeof entry.pubDate.content == 'string') {
        return entry.pubDate.content;
      }
      else if (entry.pubDate.time != null && typeof entry.pubDate.time == 'object') {
        if (entry.pubDate.time.content != null && typeof entry.pubDate.time.content == 'string' && entry.pubDate.time.content != 'Thu, 01 Jan 1970 00:00:00 +0000') {
          return entry.pubDate.time.content;
        }
      }
    }
  }
  else if (entry.publicationDate != null) {
    if (typeof entry.publicationDate === 'string' && entry.publicationDate != 'Thu, 01 Jan 1970 00:00:00 +0000') {
      return entry.publicationDate;
    }
    else if (typeof entry.publicationDate === 'object') {
      if (entry.publicationDate.content != null && typeof entry.publicationDate.content == 'string') {
        return entry.publicationDate.content;
      }
      else if (entry.publicationDate.time != null && typeof entry.publicationDate.time == 'object') {
        if (entry.publicationDate.time.content != null && typeof entry.publicationDate.time.content == 'string' && entry.publicationDate.time.content != 'Thu, 01 Jan 1970 00:00:00 +0000') {
          return entry.publicationDate.time.content;
        }
      }
    }
  }

  return null;
}

function getStringValue(variable){
  if (typeof variable === 'string') {
    return variable;
  }
  else if (typeof variable == 'object' && 'length' in variable && typeof variable.length === 'number') {
    return variable[0];
  }
  else if (typeof variable == 'number') {
    return variable.toString();
  }
  return '';
}
