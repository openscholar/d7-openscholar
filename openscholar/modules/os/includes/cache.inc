<?php

/**
 * @file cache.inc
* Provides and CRUD functions needed for clearing and maintaining cache in an
* openscholar site.
*
* Provides cache management functions
*
* os_user_create
*  Remove the page cache for the given page or wildcard
*
*/

/**
 * Remove the page(s)/etc cache for the given URL or wildcard
 */
function os_remove_cache($urls, $wildcard = FALSE){
  global $base_url;

  if(!module_exists('expire')){
    return false;
  }

  if(!is_array($urls)){
    $urls = array($urls);
  }

  $absolute_urls = array();
  $internal_paths = array();
  foreach ($urls as &$url) {
    if (strpos($url, '://')) {
      $absolute_urls[] = trim($url);
    }
    else {
      $internal_paths[] = trim($url);
    }
  }

  if (!empty($internal_paths)) {
    ExpireAPI::executeExpiration($internal_paths);
  }

  if (!empty($absolute_urls)) {
    ExpireAPI::executeExpiration($absolute_urls, '', NULL, TRUE);
  }
}
