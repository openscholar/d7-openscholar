<?php

/**
 * @file
 * Provides mapping for contextual ids to documentation URLs.
 */

/**
 * The mode for popup icon links to behave when clicked.
 *
 * @todo Expose setting for sitewide admins?
 */
define('OS_HELP_ICON_LINK_MODE', 'new_tab');

/**
 * Returns a docs URL for the given path or form element key.
 *
 * @param bool $is_path
 *   TRUE if the $id param being passed is a page path, FALSE otherwise.
 * @param string id
 *   The current path or form element.
 */
function os_help_get_variables_from_id($type, $id) {
  $variables = FALSE;

  switch ($type) {
    case 'path':
      $links = array(
        'cp/appearance' => array('url' => 'https://help.theopenscholar.com/managing-look-your-site', 'title' => 'Manage Appearance'),
        'cp/build' => array('url' => 'https://help.theopenscholar.com/content-apps-and-widgets', 'title' => 'Apps'),
        'cp/build/apps' => array('url' => 'https://help.theopenscholar.com/content-apps-and-widgets', 'title' => 'Apps'),
        'cp/build/layout' => array('url' => 'https://help.theopenscholar.com/customizing-your-site-layout', 'title' => 'Manage Layout'),
        'cp/build/menu' => array('url' => 'https://help.theopenscholar.com/menus', 'title' => 'Manage Menus'),
        'cp/build/taxonomy' => array('url' => 'https://help.theopenscholar.com/categorizing-your-content-taxonomy', 'title' => 'Manage Taxonomy'),
        'cp/content' => array('url' => '', 'title' => ''),
        'cp/content/content' => array('url' => '', 'title' => ''),
        'cp/content/files' => array('url' => 'https://help.theopenscholar.com/file-management', 'title' => 'Manage Files'),
        'cp/settings' => array('url' => 'https://help.theopenscholar.com/site-settings', 'title' => 'Manage Site Settings'),
        'cp/support' => array('url' => 'http://hwp.harvard.edu/pages/support', 'title' => 'Get Support'),
        'cp/users' => array('url' => 'https://help.theopenscholar.com/managing-users', 'title' => 'Manage Site Members'),
      );
      break;
    case 'app':
      $links = array(
        'harvard_courses' => array('url' => '', 'title' => ''),
        'os_news' => array('url' => 'https://help.theopenscholar.com/import-news-rss', 'title' => 'News App'),
        'os_biocv' => array('url' => 'https://help.theopenscholar.com/biocv', 'title' => 'Bio/CV app'),
        'os_blog' => array('url' => 'https://help.theopenscholar.com/blog', 'title' => 'Blog App'),
        'os_booklets' => array('url' => 'https://help.theopenscholar.com/booklet', 'title' => 'Booklets App'),
        'os_classes' => array('url' => 'https://help.theopenscholar.com/classes', 'title' => 'Classes App'),
        'os_events' => array('url' => 'https://help.theopenscholar.com/events-app', 'title' => 'Events App'),
        'os_faq' => array('url' => 'https://help.theopenscholar.com/faq-app', 'title' => 'FAQ App'),
        'os_files_private' => array('url' => 'https://help.theopenscholar.com/private-files', 'title' => 'Private Files App'),
        'os_pages' => array('url' => 'https://help.theopenscholar.com/pages', 'title' => 'Basic Pages App'),
        'os_image_gallery' => array('url' => 'https://help.theopenscholar.com/image-management', 'title' => 'Media Gallery App'),
        'os_links' => array('url' => 'https://help.theopenscholar.com/links', 'title' => 'Links App'),
        'os_profiles' => array('url' => 'https://help.theopenscholar.com/people-profiles', 'title' => 'Profiles App'),
        'os_publications' => array('url' => 'https://help.theopenscholar.com/publications', 'title' => 'Publications App'),
        'os_presentations' => array('url' => 'https://help.theopenscholar.com/presentations', 'title' => 'Presentations App'),
        'os_reader' => array('url' => 'https://help.theopenscholar.com/rss-feed', 'title' => 'Reader App'),
        'os_software' => array('url' => 'https://help.theopenscholar.com/software', 'title' => 'Software App'),
      );
      break;
    case 'widget':
      $links = array(
        'os_boxes_html' => array('url' => 'https://help.theopenscholar.com/custom-texthtml-widget', 'title' => 'Custom text widget'),
        'os_taxonomy_fbt' => array('url' => 'https://help.theopenscholar.com/taxonomy-widget', 'title' => 'Taxonomy Widget'),
        'os_boxes_manual_list' => array('url' => 'https://help.theopenscholar.com/featured-posts-widget', 'title' => 'Featured Posts Widget'),
        'os_search_solr_more_like_this' => array('url' => 'https://help.theopenscholar.com/more', 'title' => 'More like this Widget'),
        'os_sv_list_box' => array('url' => 'https://help.theopenscholar.com/list-of-posts-widget', 'title' => 'List of Posts Widget'),
        'os_sv_list_file' => array('url' => 'https://help.theopenscholar.com/list-files', 'title' => 'List of Files Widget'),
        'os_boxes_feedreader' => array('url' => 'https://help.theopenscholar.com/feed-reader-widget', 'title' => 'Feed Reader Widget'),
        'os_boxes_pub_year' => array('url' => 'https://help.theopenscholar.com/publications-year-widget', 'title' => 'Publications by Year Widget'),
        'os_boxes_booktoc' => array('url' => 'https://help.theopenscholar.com/booklet-table-contents-widget', 'title' => 'Booklet Table of Content Widget'),
        'os_boxes_pub_ca' => array('url' => 'https://help.theopenscholar.com/publications-authors-widget', 'title' => 'Publications by Authors Widget'),
        'os_boxes_pub_type' => array('url' => 'https://help.theopenscholar.com/publications-types-widget', 'title' => 'Publications: Types Widget'),
        'os_slideshow_box' => array('url' => 'https://help.theopenscholar.com/photo-slideshow-widget', 'title' => 'Slideshow Widget'),
        'os_boxes_media' => array('url' => 'https://help.theopenscholar.com/embed-media-widget', 'title' => 'Embed Media Widget'),
        'os_boxes_rss' => array('url' => 'https://help.theopenscholar.com/rss-feed-widget', 'title' => 'Rss Feed Widget'),
        'os_boxes_mailchimp' => array('url' => 'https://help.theopenscholar.com/embedding-mailchimp-newsletter-sign', 'title' => 'MailChimp Widget'),
        'os_boxes_follow' => array('url' => 'https://help.theopenscholar.com/follow-me-widget', 'title' => 'Follow Me Widget'),
        'os_boxes_twitterfeed' => array('url' => 'https://help.theopenscholar.com/twitter-widget', 'title' => 'Twitter Widget'),
        'os_boxes_bio' => array('url' => 'https://help.theopenscholar.com/bio-widget', 'title' => 'Bio Widget'),
        'os_boxes_addthis' => array('url' => 'https://help.theopenscholar.com/addthis-widget', 'title' => 'Add This Widget'),
        'os_boxes_accordion' => array('url' => 'https://help.theopenscholar.com/accordion-widget', 'title' => 'Accordian Widget'),
        'os_boxes_tabs' => array('url' => 'https://help.theopenscholar.com/tabs-widget', 'title' => 'Tabs Widget'),
        'os_boxes_rand_widget' => array('url' => 'https://help.theopenscholar.com/random-widget', 'title' => 'Random Widget'),
        'os_boxes_widget_slider' => array('url' => 'https://help.theopenscholar.com/widget-slider', 'title' => 'Widget Slider Widget'),
        'os_boxes_columns' => array('url' => 'https://help.theopenscholar.com/columns-widget', 'title' => 'Columns Widget'),
        'os_boxes_facetapi_vocabulary' => array('url' => 'https://help.theopenscholar.com/filtering-content-faceted-taxonomy-widget', 'title' => 'Faceted Taxonomy Widget'),
        'os_boxes_dataverse_dataset' => array('url' => 'https://help.theopenscholar.com/dataverse-dataset-widget', 'title' => 'Dataverse Dataset'),
        'os_boxes_dataverse_dataset_citation' => array('url' => 'https://help.theopenscholar.com/dataverse-dataset-citation', 'title' => 'Dataverse Dataset Citation'),
        'os_boxes_dataverse_list' => array('url' => 'https://help.theopenscholar.com/dataverse-list-widget', 'title' => 'Dataverse List'),
        'os_boxes_dataverse_search' => array('url' => 'https://help.theopenscholar.com/dataverse-search-box', 'title' => 'Dataverse Search'),
        'os_search_solr_search_box' => array('url' => 'https://help.theopenscholar.com/filtered-search-widget', 'title' => 'Filtered Search Widget'),
      );
      break;
  }

  if (isset($links[$id]) && !empty($links[$id]['url']) && !empty($links[$id]['title'])) {
    $variables = array(
      'path' => $links[$id]['url'],
      'text' => '<span>' . t('Help') . '</span>',
      'options' => array(
        'fragment' => 'main-content',
        'attributes' => array(
          'class' => array(
            'os-help-link',
          ),
          'title' => $links[$id]['title'],
        ),
        'html' => TRUE,
      ),
    );
    $mode = OS_HELP_ICON_LINK_MODE;
    switch ($mode) {
      case 'popup':
        $variables['options']['attributes']['onclick'] = 'var w=window.open(this.href, \'os_help_link\', \'width=480,height=600,scrollbars,resizable\'); w.focus(); return false;';
        break;
      case 'new_tab':
        $variables['options']['attributes']['target'] = '_blank';
        break;
    }
  }

  return $variables;
}
