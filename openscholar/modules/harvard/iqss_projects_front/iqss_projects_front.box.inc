<?php
/**
 * @file
 * iqss_projects_front.box.inc
 */

/**
 * Implements hook_default_box().
 */
function iqss_projects_front_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_projects_fp_features';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Features Carousel';
  $box->options = array(
    'body' => array(
      'value' => '<div class="os-row-features">
  <h2>Features</h2>
  <div class="os-projects-slider">
  <div class="os-jcarousel-skin-os_front">
    <ul id="jcarousel-features-carousel" class="jcarousel-features-carousel">
      <li id="mce-feature-academic-content">
        <h3>Create anything related to an academic site.</h3>
        <p>Your mission statement, bios, publications, a blog, image galleries, class materials and more.</p>
      </li>

      <li id="mce-word-out">
        <h3>Get the word out.</h3>
        <p>Create project announcements, resources, links, RSS feeds.</p>
      </li>

      <li id="mce-feature-look-feel">
        <h3>Control the look and feel of your site.</h3>
        <p>Move useful widgets in and out of your site; change menus, and the overall look of your site in seconds.</p>
      </li>

      <li id="mce-feature-events">
        <h3>Manage your events</h3>
        <p>Easily build calendars; highlight upcoming events.</p>
      </li>

      <li id="mce-smart-publishing">
        <h3>Smart Publishing</h3>
        <p>The built-in content forms and WYSIWYG editor makes it simple to publish. You can begin writing content from almost anywhere on your the site.</p>
      </li>

      <li id="mce-semantic-urls">
        <h3>Semantic URLs</h3>
        <p>All pages are given URLs which correspond to the page\'s <strong>title</strong>. No need to suffer with nonsensical URLs such as: "hdc.do?keyword=icomon&amp;tabgroupid=icb.tabgroup50611". Increase visibility and optimize the availability of your sites\' pages in search engines, such as <em>Google</em> and <em>Yahoo!</em> with semantic URLs.</p>
      </li>

      <li id="mce-categorize">
        <h3>Category Tagging</h3>
        <p>Organize content and enable visitors to find pages quickly with category tagging. The tools for multi-level category tagging come standard.</p>
      </li>

      <li id="mce-pluggable-features">
        <h3>Pluggable Features</h3>
        <p>"Out of the box ready" content features, which provide sections of your site. <strong>Features</strong> can also be set to "private", thus viewable to only members of the project.</p>
      </li>

      <li id="mce-content-aggregation">
        <h3>Content Aggregation</h3>
        <p>Keep the web sites fresh by pulling in content from relevant sources using RSS. <strong>Projects at Harvard</strong> provides built-in tools for RSS feeds making feeds easy to set up.</p>
      </li>

      <li id="mce-layout-tool">
        <h3>Interactive Layout Tool</h3>
        <p>Design page layouts using a visual "drop-and-drop" blueprint of the site. The Layout Tool provides an easy (and fun!) way to arrange the content presentation on any site. Each site owner can design site-wide default layouts or create unique layouts for different sections of their site.</p>
      </li>

      <li id="mce-sharing">
        <h3>Share With Others</h3>
        <p>Easily share web pages on other social networks such as <strong>Facebook</strong> and <strong>Twitter</strong>. Project Managers have the ability to distribute bookmark links to a variety of social network sites quickly and easily.</p>
      </li>

      <li id="mce-analytics">
        <h3>Analytics</h3>
        <p>Just provide your Google Analytics ID and data from your <strong>Projects at Harvard</strong> site will be sent to your Goggle analytics account automatically.</p>
      </li>
    </ul></div><!--CAROUSEL ENDS-->
  </div>
  </div>',
      'format' => 'full_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_projects_fp_features'] = $box;

  $image = theme('image', array('path' => drupal_get_path('theme', 'projects_harvard') . '/images/projects-at-harvard-logo.png'));

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_projects_fp_logoblock';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Projects Logo';
  $box->options = array(
    'body' => array(
      'value' => l($image, 'http://projects.iq.harvard.edu/', array('html' => TRUE)),
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_projects_fp_logoblock'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_projects_fp_copyright';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Copyright';
  $box->options = array(
    'body' => array(
      'value' =>  t('<span class="copyright">!copyright_text</span> !privacy !access !digital_access !copyinfring', array(
        '!copyright_text' => str_replace("@year", date('Y'), variable_get('copyright_text', '')),
        '!privacy' => variable_get('privacy_policy', '') && variable_get('privacy_policy_text', '') ? '| ' . l(variable_get('privacy_policy_text'), variable_get('privacy_policy')) : '',
        '!access' => variable_get('site_access_text', '') ? '| ' . l(variable_get('site_access_text'), variable_get('site_access')) : '',
        '!digital_access' => variable_get('site_digital_access_text', '') ? '| ' . l(variable_get('site_digital_access_text'), variable_get('site_digital_access')) : '',
        '!copyinfring' => variable_get('copyright_infring_text', '') ? '| ' . l(variable_get('copyright_infring_text'), variable_get('copyright_infring')) : '',
      )),
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_projects_fp_copyright'] = $box;

  return $export;
}
