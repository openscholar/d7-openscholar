<?php 

/**
 * @file infinite_scroll.inc
 * 
 * This plugin adds autopager's infinite scroll to a page.
 * 
 * ctools_include('infinite_scroll', 'os');
 * _os_add_infinite_scroll('mymodule', '.selector > .for > #pager-next-link,', '#content.selector');
 */

/**
 * @function _os_add_infinite_scroll
 * 
 * Attaches autopage javascript and provides selectors
 * 
 * @param $name
 *   Unique name for this infinite scroll
 * @param $next_link (optional)
 *   jQuery selector for the next link
 * @param $content (optional)
 *   jQuery selector for the content area
 * @param $options (optional)
 *   Associative array of additional autopager options. <http://code.google.com/p/jquery-autopager/>
 */
function _os_add_infinite_scroll($name, $next_link = 'li.pager-next a', $content = '#content', $options = array()) {
  if (($lib = libraries_get_path('autopager')) == FALSE) {
    watchdog('os', t('Please install autopager library'), WATCHDOG_WARNING);
    return FALSE;
  }
  
  $img = (module_exists('views_infinite_scroll')) ? '/' . drupal_get_path('module', 'views_infinite_scroll') . '/images/ajax-loader.gif' : '/' . drupal_get_path('theme', 'os_basetheme') . '/images/throbber.gif';
    
  $settings = array(
     $name => array(
       'link' => $next_link,
       'content' => $content,
       'autoLoad' => TRUE,
       'loading_image' => $img,
       'pager' => '#content ul.pager',
       'noAutoScroll' => 4,
       'link_prev' => '#content li.pager-previous a',
     ),
   );

  $options = array_diff($options, array('link', 'content')); //link and content could be supplied in two places.  always use function args, not opts array.
  $settings[$name] += $options;
  
  drupal_add_js(array('autopager'=>$settings), 'setting');
  drupal_add_js(drupal_get_path('module', 'os') . '/includes/os_infinite_scroll.js');
  drupal_add_css(libraries_get_path('autopager') .'/autopager.css');
  libraries_load('autopager');
  
}
