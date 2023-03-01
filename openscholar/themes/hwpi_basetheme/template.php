<?php

// Rebuild the theme data. Turn this off when in production.
//system_rebuild_theme_data();
//drupal_theme_rebuild();


/**
 * Preprocess variables for html.tpl.php
 */
function hwpi_basetheme_preprocess_html(&$vars) {

    $scripts = array(
        'matchMedia.js', // matchMedia polyfill for older browsers
        'stacktable.js', // Call stacktable jQuery plugins for responsive behavior for table markups.
        'scripts.js',    // General stuff, currently removes ui-helper-clearfix class from ui tabs
    );
    foreach ($scripts as $script) {
        // See load.inc in AT Core, load_subtheme_script() is a wrapper for drupal_add_js()
        load_subtheme_script('js/' . $script, 'hwpi_basetheme', 'header', $weight = NULL);
    }
}

/**
 * Implements template_preprocess_page() for page.
 */
function hwpi_basetheme_preprocess_page(&$vars) {
    if (_is_hwpi_theme()) {
        $vars['page']['branding_header']['hwpi'] = _hwpi_branding_header();
        $vars['page']['branding_footer']['hwpi'] = _hwpi_branding_footer();
    }
}

/**
 * Returns if the active theme uses hwpi_basetheme as one of it's base theme.
 *
 * @return bool
 */
function _is_hwpi_theme($theme_name = NULL) {
    if (is_null($theme_name)) {
        $theme_name = $GLOBALS['theme'];
    }
    $themes = list_themes();
    if (isset($themes[$theme_name])) {
        $t = $themes[$theme_name];
        if (isset($t->base_themes) && isset($t->base_themes['hwpi_basetheme'])) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Returns a build array for the HWPI branding header page region.
 *
 * @return array
 */
function _hwpi_branding_header() {
    $header = array();
    if (variable_get('logo_path')) {
        $logo_path = variable_get('logo_path');
        if (file_exists($logo_path)) {
            $imageinfo = getimagesize($logo_path);
        }
        else {
            $imageinfo = array(null, null);
        }
        $header['left_container'] = array(
            '#type' => 'container',
            '#attributes' => array(
                'class' => array(
                    'branding-left',
                ),
            ),
            'img' => array(
                '#theme' => 'link',
                '#path' => variable_get('university_base_url'),
                '#text' => theme('image', array('path' => $logo_path, 'width' => $imageinfo[0], 'height' => $imageinfo[1], 'alt' => 'University Logo')),
                '#options' => array(
                    'external' => TRUE,
                    'html' => TRUE,
                    'attributes' => array(),
                ),
                '#access' => file_exists($logo_path)
            ),
        );
    }
    $sites = _hwpi_get_ancestry();
    $links = array();
    foreach ($sites as $path => $title) {
        $links[] = l($title, $path);
    }
    $header['right_container'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array(
                'branding-right',
            ),
        ),
        'sitecrumbs' => array(
            '#type' => 'markup',
            '#markup' => implode(' | ', $links),
        ),
    );

    return $header;
}

/**
 * Returns website parents in an ordered, keyed array.
 *
 * Note: The returned array uses URLs as keys, and case-sensitive titles as
 * values. The top-most level of the array is hard-coded to be 'HARVARD.EDU',
 * site organization taxonomy terms are second-highest, and sub-site relations
 * will appear as the most-specific, lowest-level ancestor.
 *
 * @return array
 *   An array keyed by fully-qualified absolute URLs, values are link title text.
 */
function _hwpi_get_ancestry() {
    $sites = array();
    if ($vsite = spaces_get_space()) {
        // First, looks for parent vsites and adds them to hierarchy.
        $vsite_original = $vsite;
        $group = $vsite->group;
        while (isset($group->field_group_parent) && $group->field_group_parent) {
            $items = field_get_items('node', $group, 'field_group_parent');
            $vsite = vsite_get_vsite($items[0]['target_id']);
            if(!is_object($vsite) || !isset($vsite->group)) {
                break;
            }
            $group = $vsite->group;
            $sites[$vsite->get_absolute_url()] = $group->title;
        }

        // Then, looks for site organization terms and adds them to hierarchy.
        $items = field_get_items('node', $vsite_original->group, 'field_organization');
        if (is_array($items) && !empty($items)) {
            $tid = $items[0]['tid'];
            $items = field_get_items('taxonomy_term', taxonomy_term_load($tid), 'field_site_url');
            if (isset($items[0])) {
                $site_url = $items[0];
                while ($site_url) {
                    $sites[$site_url['url']] = $site_url['title'];
                    $parents = taxonomy_get_parents($tid);
                    if (empty($parents)) {
                        break;
                    }
                    $tid = array_shift(array_keys($parents));
                    $items = field_get_items('taxonomy_term', taxonomy_term_load($tid), 'field_site_url');
                    if (isset($items[0])) {
                        $site_url = $items[0];
                    }
                    else {
                        $site_url = FALSE;
                    }
                }
            }
        }
    }

    // Hard-codes "HARVARD.EDU" as the highest parent item.
    $sites[variable_get('university_base_url')] = variable_get('highest_parent_item');
    return $sites;
}

/**
 * Returns a build array for the standard branding footer region (copyright).
 *
 * @return array
 *   A build array ready to render footer info.
 */
function _hwpi_branding_footer() {
  $footer = array();
  $footer['hwpi_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'copyright',
      ),
    ),
    'copyright' => array(
      '#markup' => t('<span class="harvard-copyright">!copyright_text</span> !privacy !access !digital_access !copyinfring', array(
        '!copyright_text' => str_replace("@year", date('Y'), variable_get('copyright_text', '')),
        '!privacy' => variable_get('privacy_policy', '') && variable_get('privacy_policy_text', '') ? '| ' . l(variable_get('privacy_policy_text'), variable_get('privacy_policy')) : '',
        '!access' => variable_get('site_access_text', '') ? '| ' . l(variable_get('site_access_text'), variable_get('site_access')) : '',
        '!digital_access' => variable_get('site_digital_access_text', '') ? '| ' . l(variable_get('site_digital_access_text'), variable_get('site_digital_access')) : '',
        '!copyinfring' => variable_get('copyright_infring_text', '') ? '| ' . l(variable_get('copyright_infring_text'), variable_get('copyright_infring')) : '',
      )),
    ),
  );

  return $footer;
}

/**
 * Adds mobile menu controls to menubar.
 */
function hwpi_basetheme_page_alter(&$page) {
    // Avoid adding responsive menu in case of an AJAX call.
    if (strstr($_GET['q'], 'ajax')) {
        return;
    }

    $page['responsive_menu']['#sorted'] = false;
    $page['responsive_menu']['mobile'] = array(
        '#theme' => 'links',
        '#attributes' => array(
            'class' => array('mobile-buttons'),
        ),
        '#weight' => 5000,
        '#links' => array(
            'mobi-main' => array(
                'href' => '#',
                'title' => '<span aria-hidden="true" class="icon-menu"></span><span class="move">Main Menu</span>',
                'external' => true,
                'html' => true,
                'attributes' => array(
                    'data-target' => '#block-os-primary-menu',
                ),
            ),
            'mobi-util' => array(
                'href' => '#',
                'title' => '<span aria-hidden="true" class="icon-plus"></span><span class="move">Utility Menu</span>',
                'external' => true,
                'html' => true,
                'attributes' => array(
                    'data-target' => '#block-os-quick-links, #block-os-secondary-menu, #header .os-custom-menu',
                ),
            ),
            'mobi-search' => array(
                'href' => '#',
                'title' => '<span aria-hidden="true" class="icon-search3"></span><span class="move">Search</span>',
                'external' => true,
                'html' => true,
                'attributes' => array(
                    'data-target' => '#block-os-search-db-site-search, #block-boxes-solr-search-box',
                )
            )
        )
    );

    if (context_isset('context', 'os_public') && variable_get('enable_responsive', true)) {
        $path = drupal_get_path('theme', 'hwpi_basetheme').'/css/';
        drupal_add_css($path.'responsive.base.css');
        drupal_add_css($path.'responsive.layout.css');
        drupal_add_css($path.'responsive.nav.css');
        drupal_add_css($path.'responsive.slideshow.css');
        drupal_add_css($path.'responsive.widgets.css');

        $theme = $GLOBALS['theme'];
        $theme_path = drupal_get_path('theme', $theme).'/css/';
        drupal_add_css($theme_path.'responsive.'.str_replace('hwpi_', '', $theme).'.css');
    }
}

/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_node(&$vars) {
    if ($vars['type'] != 'person') {
        return;
    }

    if (!empty($vars['node']->field_person_photo)) {
        $vars['classes_array'][] = 'with-person-photo';
    }
    else {
        if (!in_array($vars['view_mode'], array('teaser', 'sidebar_teaser', 'full'))) {
            return;
        }

        if (in_array($vars['view_mode'], array('sidebar_teaser', 'full'))) {
            // On sidebar tease view mode the content in $vars['content']['pic_bio'].
            $key = &$vars['content']['pic_bio'];
            $vars['content']['pic_bio']['#access'] = TRUE;
        }
        else {
            $key = &$vars['content'];
        }

        // Set up the size of the picture.
        $size = (!empty($vars['os_sv_list_box']) && $vars['os_sv_list_box']) || $vars['view_mode'] == 'full' ? 'large' : 'small';

        $key['field_person_photo'][0] = array('#markup' => hwpi_basetheme_profile_default_image($size));
    }
}

/**
 * Helper function; Return the markup of the profile image by the next logic:
 * When there is no profile picture the node display the uploaded image.
 * When there is no uploaded image display the default image.
 *
 * @param string $size
 *  Determine the size of the profile picture. Optional values: small or big.
 *
 * @return string
 *  The markup of the image.
 */
function hwpi_basetheme_profile_default_image($size = 'small') {

    if (variable_get('os_profiles_disable_default_image', FALSE)) {
        return '<div class="no-default-image"></div>';
    }

    if ($custom_default_image = variable_get('os_profiles_default_image_file', 0)) {
        // Use custom default image.
        $image_file = file_load($custom_default_image);
        $path = $image_file->uri;
        $options = array(
            'path' => $path,
            'style_name' => $size == 'small' ? 'profile_thumbnail' : 'profile_full',
        );

        return '<div class="field-name-field-person-photo">' . theme('image_style',  $options) . '</div>';
    }

    // Use default image.
    $image = $size == 'small' ? 'person-default-image-small.png' : 'person-default-image-large.png';
    $install_default_image = variable_get('profile_default_photo_'.$size, drupal_get_path('theme', 'os_basetheme') . '/images/' . $image);
    $path = variable_get('os_person_default_image', $install_default_image);
    return '<div class="field-name-field-person-photo">' . theme('image',  array('path' => $path)) . '</div>';
}

/**
 * Process variables for comment.tpl.php
 */
function hwpi_basetheme_process_node(&$build) {
    // Event persons, change title markup to h1.
    if ($build['type'] == 'person') {
        if ($build['view_mode'] == 'title') {
            $build['title_prefix']['#suffix'] = '<h1 class="node-title">' . l($build['title'], 'node/' . $build['nid']) . '</h1>';
            $build['title'] = NULL;
        }
        elseif (!$build['teaser'] && $build['view_mode'] != 'sidebar_teaser') {
            $build['title_prefix']['#suffix'] = '<h1 class="node-title">' . $build['title'] . '</h1>';
            $build['title'] = NULL;

            if ($build['view_mode'] == 'slide_teaser') {
                $build['title_prefix']['#suffix'] = '<div class="toggle">' . $build['title_prefix']['#suffix'] . '</div>';
            }
        }
    }
}

/*
 * Implements hook_node_view_alter
 *
 */
function hwpi_basetheme_node_view_alter(&$build) {

    // Persons, heavily modify the output to match the HC designs
    if ($build['#node']->type == 'person') {

        // Note that Contact and Website details will print wrappers and titles regardless of any field content.
        // This is kind of deliberate to avoid having to handle the complexity of dealing with the layout or
        // setting messages etc.

        $block_zebra = 0;

        // Contact Details
        if ($build['#view_mode'] != 'sidebar_teaser') {
            $build['contact_details']['#prefix'] = '<div class="block contact-details '.(($block_zebra++ % 2)?'even':'odd').'"><div class="block-inner"><h2 class="block-title">Contact Information</h2>';
            $build['contact_details']['#suffix'] = '</div></div>';
            $build['contact_details']['#weight'] = 51;

            // Contact Details > address
            if (isset($build['field_address'])) {
                $build['field_address']['#label_display'] = 'hidden';
                $build['contact_details']['field_address'] = $build['field_address'];
                $build['contact_details']['field_address'][0]['#markup'] = str_replace("\n", '<br>', $build['contact_details']['field_address'][0]['#markup']);
                unset($build['field_address']);
            }
            // Contact Details > email
            if (isset($build['field_email'])) {
                $build['field_email']['#label_display'] = 'hidden';
                $email_plain = mb_strtolower($build['field_email'][0]['#markup']);
                if ($email_plain) {

                    $build['field_email'][0]['#markup'] = l($email_plain, 'mailto:'.$email_plain, array('absolute'=>TRUE));
                }
                $build['contact_details']['field_email'] = $build['field_email'];
                $build['contact_details']['field_email']['#weight'] = 50;
                unset($build['field_email']);
            }
            // Contact Details > phone
            if (isset($build['field_phone'])) {
                $build['field_phone']['#label_display'] = 'hidden';
                $phone_plain = $build['field_phone'][0]['#markup'];
                if ($phone_plain) {
                    $build['field_phone'][0]['#markup'] = t('p: ') . $phone_plain;
                }
                $build['contact_details']['field_phone'] = $build['field_phone'];
                $build['contact_details']['field_phone']['#weight'] = 52;
                unset($build['field_phone']);
            }

            // Contact Details > office hours
            if (isset($build['field_office_hours'])) {
                $build['field_office_hours']['#label_display'] = 'hidden';
                $office_hours = trim($build['field_office_hours'][0]['#markup']);
                if ($phone_plain && !empty($office_hours)) {
                    $build['field_office_hours'][0]['#markup'] = t('Office Hours: ') . $office_hours;
                }
                $build['contact_details']['field_office_hours'] = $build['field_office_hours'];
                $build['contact_details']['field_office_hours']['#weight'] = 53;
                unset($build['field_office_hours']);
            }

            if ($build['#view_mode'] == 'sidebar_teaser') {
                $build['pic_bio']['#prefix'] = '<div class="pic-bio clearfix people-sidebar-teaser">';
            }
            else {
                $build['pic_bio']['#prefix'] = '<div class="pic-bio clearfix">';
            }
            $build['pic_bio']['#suffix'] = '</div>';
            $build['pic_bio']['#weight'] = -9;

            if (isset($build['body'])) {
                $build['body']['#label_display'] = 'hidden';
                $build['pic_bio']['body'] = $build['body'];
                unset($build['body']);
            }

            //join titles
            $title_field = &$build['field_professional_title'];
            if ($title_field) {
                $keys = array_filter(array_keys($title_field), 'is_numeric');
                foreach ($keys as $key) {
                    $titles[] = $title_field[$key]['#markup'];
                    unset($title_field[$key]);
                }
                $title_field[0] = array('#markup' => implode('<br />', $titles));
            }

            // We dont want the other fields on teasers
            if (in_array($build['#view_mode'], array('teaser', 'slide_teaser','no_image_teaser'))) {

                unset($build['contact_details']['#prefix'], $build['contact_details']['#suffix']);

                //move title, website. body
                if (!empty($build['pic_bio']['body'])) {
                    $build['pic_bio']['body']['#weight'] = 5;
                }
                foreach (array(0=>'field_professional_title', 15=>'field_website') as $weight => $field) {
                    if (isset($build[$field])) {
                        $build['pic_bio'][$field] = $build[$field];
                        $build['pic_bio'][$field]['#weight'] = $weight;
                        unset($build[$field]);
                    }
                }

                //hide the rest
                foreach (array('field_address') as $field) {
                    if (isset($build[$field])) {
                        unset($build[$field]);
                    }
                }

                if (isset($build['field_email'])) {
                    $email_plain = $build['field_email'][0]['#markup'];
                    $build['field_email'][0]['#markup'] = '<a href="mailto:' . $email_plain . '">' . $email_plain . '</a>';
                }

                // Newlines after website.
                if (isset($build['pic_bio']['field_website'])) {
                    foreach (array_filter(array_keys($build['pic_bio']['field_website']), 'is_numeric') as $delta) {
                        $item = $build['pic_bio']['field_website']['#items'][$delta];
                        $build['pic_bio']['field_website'][$delta]['#markup'] = l($item['title'], $item['url'], $item) . '<br />';
                    }
                }

                if (isset($build['links']['node']['#links']['node-readmore']) && !empty($build['pic_bio']['body'])) {
                    $link = $build['links']['node']['#links']['node-readmore'];
                    if (preg_match('!</?(?:p)[^>]*>\s*$!i', $build['pic_bio']['body'][0]['#markup'], $match, PREG_OFFSET_CAPTURE)) {
                        $insert_point = $match[0][1];
                        // Insert the link.
                        $build['pic_bio']['body'][0]['#markup'] = substr_replace($build['pic_bio']['body'][0]['#markup'], ' '.l($link['title'], $link['href'], $link), $insert_point, 0);
                    }
                }

                return;
            }

            // Professional titles
            if (isset($build['field_professional_title'])) {
                $build['field_professional_title']['#label_display'] = 'hidden';
                $build['field_professional_title']['#weight'] = -10;
            }

            if (isset($build['field_person_photo'])) {
                $build['field_person_photo']['#label_display'] = 'hidden';
                $build['pic_bio']['field_person_photo'] = $build['field_person_photo'];
                unset($build['field_person_photo']);
            }

            $children = element_children($build['pic_bio']);
            if (empty($children)) {
                $build['pic_bio']['#access'] = false;
            }


            // Websites
            if (isset($build['field_website'])) {
                $build['website_details']['#prefix'] = '<div class="block website-details '.(($block_zebra++ % 2)?'even':'odd').'"><div class="block-inner"><h2 class="block-title">Websites</h2>';
                $build['website_details']['#suffix'] = '</div></div>';
                $build['website_details']['#weight'] = -7;
                $build['field_website']['#label_display'] = 'hidden';
                $build['website_details']['field_website'] = $build['field_website'];
                unset($build['field_website']);
            }

            //Don't show an empty contact details section.
            if (!element_children($build['contact_details'])) {
                unset($build['contact_details']);
            }
            else {
                $build['contact_details']['#weight'] = -8;
            }
        }

        // Adding condition so that change in display in terms will show for full view mode
        if (isset($build['og_vocabulary']) && $build['#view_mode'] == 'full') {
            $terms = array();
            foreach ($build['og_vocabulary']['#items'] as $i) {
                if (isset($i['target_id'])) {
                    $terms[] = $i['target_id'];
                }
                else {
                    $terms[] = $i;
                }
            }

            $terms = vsite_vocab_sort_weight_alpha(taxonomy_term_load_multiple($terms));

            foreach ($terms as $info) {
                $v = taxonomy_vocabulary_load($info['vid']);
                if (!isset($build[$v->machine_name])) {
                    $m = $v->machine_name;
                    $build[$m] = array(
                        '#type' => 'container',
                        '#weight' => $block_zebra,
                        '#attributes' => array(
                            'class' => array(
                                'block',
                                $m,
                                (($block_zebra++ % 2)?'even':'odd')
                            )
                        ),
                        'inner' => array(
                            '#type' => 'container',
                            '#attributes' => array(
                                'class' => array('block-inner'),
                            ),
                            'title' => array(
                                '#markup' => '<h2 class="block-title">'.$v->name.'</h2>',
                            )
                        ),
                    );
                }
                $build[$v->machine_name]['inner'][$info['tid']] = array(
                    '#prefix' => '<div>',
                    '#suffix' => '</div>',
                    '#theme' => 'link',
                    '#path' => $info['uri'],
                    '#text' => $info['term'],
                    '#options' => array('attributes' => array(), 'html' => false),
                );
            }
            unset($build['og_vocabulary']);
        }
    }
}


/**
 * Implements hook_field_display_ENTITY_TYPE_alter().
 */
function hwpi_basetheme_field_display_node_alter(&$display, $context) {
    if ($context['entity']->type == 'event' && $context['instance']['field_name'] == 'field_date' && (!in_array($context['view_mode'], ['full', 'rss']) || (isset($context['entity']->os_sv_list_box) && $context['entity']->os_sv_list_box))) {

        if (isset($context['entity']->field_date[LANGUAGE_NONE][0]['value2']) &&
            (strtotime($context['entity']->field_date[LANGUAGE_NONE][0]['value2']) - strtotime($context['entity']->field_date[LANGUAGE_NONE][0]['value']) > 24*60*60)) {
            return; //event is more than one day long - keep both dates visible
        }

        //hide the date - it's already visible in the shield
        $display['settings']['format_type'] = 'os_time';
    }
}

/**
 * Preprocess variables for comment.tpl.php
 */
function hwpi_basetheme_preprocess_comment(&$vars) {
    if($vars['new']) {
        $vars['title'] = $vars['title'] . '<em class="new">' . $vars['new'] . '</em>';
        $vars['new'] = '';
    }
}


/**
 * Returns HTML for a menu link and submenu.
 *
 * Adaptivetheme overrides this to insert extra classes including a depth
 * class and a menu id class. It can also wrap menu items in span elements.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 */
function hwpi_basetheme_menu_link(array $vars) {
    $element = $vars['element'];
    $sub_menu = '';

    if ($element['#below']) {
        $sub_menu = drupal_render($element['#below']);
    }

    if (!empty($element['#original_link'])) {
        if (!empty($element['#original_link']['depth'])) {
            $element['#attributes']['class'][] = 'menu-depth-' . $element['#original_link']['depth'];
        }
        if (!empty($element['#original_link']['mlid'])) {
            $element['#attributes']['class'][] = 'menu-item-' . $element['#original_link']['mlid'];
        }
    }

    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";
}


/**
 * Returns HTML for a set of links.
 *
 * @param $vars
 *   An associative array containing:
 *   - links: An associative array of links to be themed. The key for each link
 *     is used as its CSS class. Each link should be itself an array, with the
 *     following elements:
 *     - title: The link text.
 *     - href: The link URL. If omitted, the 'title' is shown as a plain text
 *       item in the links list.
 *     - html: (optional) Whether or not 'title' is HTML. If set, the title
 *       will not be passed through check_plain().
 *     - attributes: (optional) Attributes for the anchor, or for the <span> tag
 *       used in its place if no 'href' is supplied. If element 'class' is
 *       included, it must be an array of one or more class names.
 *     If the 'href' element is supplied, the entire link array is passed to l()
 *     as its $options parameter.
 *   - attributes: A keyed array of attributes for the UL containing the
 *     list of links.
 *   - heading: (optional) A heading to precede the links. May be an associative
 *     array or a string. If it's an array, it can have the following elements:
 *     - text: The heading text.
 *     - level: The heading level (e.g. 'h2', 'h3').
 *     - class: (optional) An array of the CSS classes for the heading.
 *     When using a string it will be used as the text of the heading and the
 *     level will default to 'h2'. Headings should be used on navigation menus
 *     and any list of links that consistently appears on multiple pages. To
 *     make the heading invisible use the 'element-invisible' CSS class. Do not
 *     use 'display:none', which removes it from screen-readers and assistive
 *     technology. Headings allow screen-reader and keyboard only users to
 *     navigate to or skip the links. See
 *     http://juicystudio.com/article/screen-readers-display-none.php and
 *     http://www.w3.org/TR/WCAG-TECHS/H42.html for more information.
 */
function hwpi_basetheme_links($vars) {
    $links = $vars['links'];
    $attributes = $vars['attributes'];
    $heading = $vars['heading'];
    global $language_url;
    $output = '';

    if (count($links) > 0) {
        $output = '';

        if (!empty($heading)) {
            if (is_string($heading)) {
                $heading = array(
                    'text' => $heading,
                    'level' => 'h2',
                );
            }
            $output .= '<' . $heading['level'];
            if (!empty($heading['class'])) {
                $output .= drupal_attributes(array('class' => $heading['class']));
            }
            $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
        }

        // Count links to use later for setting classes on the ul wrapper and on
        // each link
        $num_links = count($links);
        $i = 1;

        // Add a class telling us how many links there are, we need to check if
        // $attributes['class'] is an array because toolbar is converting this to
        // a string, if we don't check we get a fatal error. This class is added
        // to aid in potential cross browser issues with the full width ui.tabs
        if (isset($attributes['class']) && is_array($attributes['class'])) {
            $attributes['class'][] = 'num-links-' . $num_links;
        }
        $output .= '<ul' . drupal_attributes($attributes) . '>';

        foreach ($links as $key => $link) {
            // Add classes to make theming the ui.tabs much easier/possible
            $class = array();
            $class[] = 'link-count-' . $key;
            if ($i == 1) {
                $class[] = 'first';
            }
            if ($i == $num_links) {
                $class[] = 'last';
            }
            if (!empty($class)) {
                $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';
            }
            else {
                $output .= '<li>';
            }
            if (isset($link['href'])) {
                $output .= l($link['title'], $link['href'], $link);
            }
            elseif (!empty($link['title'])) {
                if (empty($link['html'])) {
                    $link['title'] = check_plain($link['title']);
                }
                $span_attributes = '';
                if (isset($link['attributes'])) {
                    $span_attributes = drupal_attributes($link['attributes']);
                }
                $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
            }

            $i++;
            $output .= "</li>";
        }

        $output .= '</ul>';
    }

    return $output;
}

/**
 * Returns HTML for status and/or error messages, grouped by type.
 *
 * Adaptivetheme adds a div wrapper with CSS id.
 *
 * An invisible heading identifies the messages for assistive technology.
 * Sighted users see a colored box. See http://www.w3.org/TR/WCAG-TECHS/H69.html
 * for info.
 *
 * @param $vars
 *   An associative array containing:
 *   - display: (optional) Set to 'status' or 'error' to display only messages
 *     of that type.
 */
function hwpi_basetheme_status_messages($vars) {
    $display = $vars['display'];
    $output = '';
    $allowed_html_elements = '<'. implode('><', variable_get('html_title_allowed_elements', array('em', 'sub', 'sup'))) . '>';

    $status_heading = array(
        'status' => t('Status update'),
        'error' => t('Error'),
        'warning' => t('Warning'),
    );
    foreach (drupal_get_messages($display) as $type => $messages) {
        $output .= '<div class="messages ' . $type . '"><div ng-non-bindable class="message-inner"><div class="message-wrapper">';
        if (!empty($status_heading[$type])) {
            $output .= '<h2>' . $status_heading[$type] . "</h2>";
        }
        if (count($messages) > 1) {
            $output .= " <ul>";
            foreach ($messages as $message) {
                if (strpos($message, 'Biblio') === 0 || strpos($message, 'Publication') === 0) {
                    // Allow some tags in messages about a Biblio.
                    $output .= '  <li ng-non-bindable>' . strip_tags(html_entity_decode($message), $allowed_html_elements) . "</li>";
                }
                else {
                    $output .= '  <li ng-non-bindable>' . $message . "</li>";
                }
            }
            $output .= " </ul>";
        }
        elseif (strpos($messages[0], 'Biblio') === 0 || strpos($messages[0], 'Publication') === 0) {
            // Allow some tags in messages about a Biblio.
            $output .= strip_tags(html_entity_decode($messages[0]), $allowed_html_elements);
        }
        else {
            $output .= $messages[0];
        }
        $output .= "</div></div></div>";
    }
    return $output;
}

/**
 * Implements template_process_HOOK() for theme_pager_link().
 */
function hwpi_basetheme_process_pager_link($variables) {
    // Adds an HTML head link for rel='prev' or rel='next' for pager links.
    module_load_include('inc', 'os', 'includes/pager');
    _os_pager_add_html_head_link($variables);
}

// Overriding theme_nice_menus_build to add open-submenu span for menuparent's
function hwpi_basetheme_nice_menus_build($variables) {
    $menu = $variables['menu'];
    $depth = $variables['depth'];
    $trail = $variables['trail'];
    $output = '';
    // Prepare to count the links so we can mark first, last, odd and even.
    $index = 0;
    $count = 0;
    foreach ($menu as $menu_count) {
        if ($menu_count['link']['hidden'] == 0) {
            $count++;
        }
    }
    // Get to building the menu.
    foreach ($menu as $menu_item) {
        $mlid = $menu_item['link']['mlid'];
        // Check to see if it is a visible menu item.
        if (!isset($menu_item['link']['hidden']) || $menu_item['link']['hidden'] == 0) {
            // Check our count and build first, last, odd/even classes.
            $index++;
            $first_class = $index == 1 ? ' first ' : '';
            $oddeven_class = $index % 2 == 0 ? ' even ' : ' odd ';
            $last_class = $index == $count ? ' last ' : '';
            // Build class name based on menu path
            // e.g. to give each menu item individual style.
            // Strip funny symbols.
            $clean_path = str_replace(array('http://', 'www', '<', '>', '&', '=', '?', ':', '.'), '', $menu_item['link']['href']);
            // Convert slashes to dashes.
            $clean_path = str_replace('/', '-', $clean_path);
            $class = 'menu-path-' . $clean_path;
            if ($trail && in_array($mlid, $trail)) {
                $class .= ' active-trail';
            }
            // If it has children build a nice little tree under it.
            if ((!empty($menu_item['link']['has_children'])) && (!empty($menu_item['below'])) && $depth != 0) {
                // Keep passing children into the function 'til we get them all.
                if ($menu_item['link']['depth'] <= $depth || $depth == -1) {
                    $children = array(
                        '#theme' => 'nice_menus_build',
                        '#prefix' => '<span class="open-submenu"></span><ul>',
                        '#suffix' => '</ul>',
                        '#menu' => $menu_item['below'],
                        '#depth' => $depth,
                        '#trail' => $trail,
                    );
                } else {
                    $children = '';
                }
                // Set the class to parent only of children are displayed.
                $parent_class = ($children && ($menu_item['link']['depth'] <= $depth || $depth == -1)) ? 'menuparent ' : '';
                $element = array(
                    '#below' => $children,
                    '#title' => $menu_item['link']['title'],
                    '#href' => $menu_item['link']['href'],
                    '#localized_options' => $menu_item['link']['localized_options'],
                    '#attributes' => array(
                        'class' => array('menu-' . $mlid, $parent_class, $class, $first_class, $oddeven_class, $last_class),
                    ),
                );
                $variables['element'] = $element;
                $output .= theme('menu_link', $variables);
            } else {
                $element = array(
                    '#below' => '',
                    '#title' => $menu_item['link']['title'],
                    '#href' => $menu_item['link']['href'],
                    '#localized_options' => isset($menu_item['link']['localized_options']) ? $menu_item['link']['localized_options'] : array(),
                    '#attributes' => array(
                        'class' => array('menu-' . $mlid, $class, $first_class, $oddeven_class, $last_class),
                    ),
                );
                $variables['element'] = $element;
                $output .= theme('menu_link', $variables);
            }
        }
    }
    return $output;
}
