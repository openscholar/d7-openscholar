<?php
/**
 * @file
 * iqss_scholars_front.box.inc
 */

/**
 * Implements hook_default_box().
 */
function iqss_scholars_front_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fixed_header';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Scholars fixed-position header.';
  $box->options = array(
    'body' => array(
      'value' => '<div id="os-reasons_header" class="os-scholar-header">
        <div class="os-scholar-header-wrap">
            <h2><a href="/" title="Home" rel="home">Scholars at Harvard</a></h2>
            <a href="/site/register" class="os-big-button">Get Your Site Now</a>
            </div>
        </div>
      ',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fixed_header'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_headertext';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Front page header text';
  $box->options = array(
    'body' => array(
      'value' => '<div class="os-topheadings">Personal websites created by Harvard scholars using <a href="http://openscholar.harvard.edu/">OpenScholar@Harvard</a>,<br> a web publishing service provided by <a href="http://hwp.harvard.edu/">Harvard Web Publishing</a>. <br><a href="http://openscholar.harvard.edu/">Not a Harvard Scholar? Find out how to create a similar site at your institution.</a></div>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fp_headertext'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_infoblock';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Scholars Info text with video link';
  $box->options = array(
    'body' => array(
      'value' => '<div class="os-leftcol">
  Hosted by Harvard Web Publishing, and powered by OpenScholar software, OpenScholar @ Harvard offers you the next generation scholarly web site. With it, you can easily create and manage a highly dynamic, powerful, and customizable site, with an easy-to-use interface and a look and feel that is uniquely yours.
<ul><li><a href="profiles/openscholar/themes/custom/scholars_harvard/Info-sheet-Scholars-at-Harvard-H-2011.03.17.pdf">Fact Sheet (PDF)</a></li>
<li><a href="http://player.vimeo.com/video/9887585?title=0&amp;byline=0&amp;portrait=0">Video Intro</a></li>
</ul></div>
<div class="os-featuredSites"><a href="/websites">Featured Sites</a></div>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fp_infoblock'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_learnmore';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Scholars Learn More Box';
  $box->options = array(
    'body' => array(
      'value' => '<div class="os-rightcol">
<p class="os-leadin">OpenScholar @ Harvard is a web site building tool available to faculty, graduate students and visiting scholars at Harvard.</p>
<ul>
<li><span>Create a beautiful academic web site in seconds.</span></li>
<li><span>Receive more web visibility.</span></li>
<li><span>Build higher citation counts.</span></li>
<li><span>Customize your site\'s layout and structure, and choose (or design) a unique look and feel</span></li>
<li><span>Watch as your site is automatically updated with the latest web technology.</span></li>
</ul>
</div>
<div class="started-oneyour-own">Get started on your own today</div>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fp_learnmore'] = $box;
  

   $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_hwp_option';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'HWP Option Info text';
  $box->options = array(
    'body' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fp_hwp_option'] = $box;


  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_logoblock';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Scholars Logo';
  $box->options = array(
    'body' => array(
      'value' => '<h1>Scholars at Harvard</h1>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_fp_logoblock'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_learnmore_toggle';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'Scholars Learn More Toggle Page';
  $box->options = array(
    'body' => array(
      'value' => '<div id="os-reasons" class="os-page">
       <div id="os-reasons_header" class="os-scholar-header">
        <div class="os-scholar-header-wrap">
            <h2><a href="/" title="Home" rel="home">Scholars at Harvard</a></h2>
            <a href="/site/register" class="os-big-button">Get Your Site Now</a>
            </div>
        </div>
    <a class="os-columns-home" href="#">Home</a>
    <ul class="os-features os-wrap os-dark os-clearfix">
        <li class="os-feature-academic-content">
          <h3>Create Anything Related to an Academic Site</h3>
          <p>CV, bio, publications, blogs, announcements, links, events, image galleries, class materials, presentations. Easy as using a word processor.</p>
        </li>
        <li class="os-feature-biblio">
          <h3>Get Cited More</h3>
          <p>Automatically have your publications submitted to indices such as RePEc, Computer Science Repository, and Google Scholar.</p>
        </li>
        <li class="os-no-tech">
          <h3>No Technical Knowledge Needed</h3>
          <p>The user-friendly interface for adding and editing content requires no technical knowledge, special software, or HTML. You can edit your site from any computer with an internet connection.</p>
        </li>
		<li class="os-feature-look-feel">
          <h3>Control the Look and Feel of Your Site</h3>
          <p>Move useful widgets in and out of your site; change menus and the overall look of your site in seconds.</p>
        </li>
        <li class="os-categorize">
          <h3>Category Tagging</h3>
          <p>Organize content and enable visitors to find pages quickly with category tagging. The tools for multi-level category tagging come standard.</p>
        </li>
        <li class="os-pluggable-features">
          <h3>Pluggable Features</h3>
          <p>Out-of-the-box-ready, content "features," which provide sections of your site. <strong>Features</strong> can also be set to "private," thus viewable to only members of the site.</p>
        </li>
        <li class="os-feature-collaborate">
          <h3>Collaborate with Your Colleagues</h3>
          <p>Allow commentary on your work by whomever you select. Keep track of colleagues\' work.</p>
        </li>
        <li class="os-custom-domains">
          <h3>Custom Domains</h3>
          <p>Use your custom domain for your website. Departments and institutes can use their domain for all sites of affiliated scholars.</p>
        </li>
        <li class="os-smart-publishing">
          <h3>Smart Publishing</h3>
          <p>The built-in content forms and WYSIWYG editor makes it simple to publish. You can begin writing content from almost anywhere on your site.</p>
        </li>
        <li class="os-feature-events">
          <h3>Manage Your Events</h3>
          <p>Build calendars easily. Highlight upcoming events.</p>
        </li>
        <li class="os-content-aggregation">
          <h3>Content Aggregation</h3>
          <p>Keep the web site fresh by pulling in content from relevant sources using RSS. <strong>Scholars at Harvard</strong> provides built-in tools for RSS feeds making feeds easy to set up.</p>
        </li>
        <li class="os-word-out">
          <h3>Get the Word Out</h3>
          <p>Create project announcements, resources, links, RSS feeds.</p>
        </li>
        <li class="os-semantic-urls">
          <h3>Semantic URLs</h3>
          <p>Increase visibility and optimize the availability of your site\'s pages in search engines, such as <em>Google</em> and <em>Yahoo!</em> with semantic URLs.</p><p>All pages are given URLs which correspond to the page\'s <strong>title</strong>. No need to suffer with nonsensical web addresses such as: "hdc.do?keyword=something&tabgroupid=icb.tabgroup50611". </p>
        </li>
        <li class="os-layout-tool">
          <h3>Interactive Layout Tool</h3>
          <p>Design page layouts using a visual "drag-and-drop" blueprint of the site. The Layout Tool provides an easy (and fun!) way to arrange the content presentation on any site. You can design site-wide default layouts or create unique layouts for different sections of your site.</p>
        </li>
        <li class="os-sharing">
          <h3>Share with Others</h3>
          <p>Allow sharing of your content on other social networks such as <strong>Facebook</strong> and <strong>Twitter</strong>.</p>
        </li>
        <li class="os-analytics">
          <h3>Analytics</h3>
          <p>Just provide your Google Analytics ID, and data from your <strong>Scholars at Harvard</strong> site will be sent to your Goggle analytics account automatically.</p>
        </li>
      </ul>
  <ul class="os-more-info os-wrap os-clearfix">
    <li class="os-docs"><span><strong>Scholars at Harvard</strong> includes complete <strong>documentation</strong>.  <a href="http://scholar.harvard.edu/help/os_help/User-Documentation" target="_blank">Go there now</a></span></li>
    <li class="os-link"><span><strong>Scholars at Harvard</strong> is built on <strong>OpenScholar</strong>, an open-source web-creation tool developed right here at Harvard. <a href="http://openscholar.harvard.edu" target="_blank">Learn More</a></span></li>
  </ul>
  </div>
      ',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['iqss_scholars_learnmore_toggle'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'iqss_scholars_fp_copyright';
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
  $export['iqss_scholars_fp_copyright'] = $box;

  return $export;
}
