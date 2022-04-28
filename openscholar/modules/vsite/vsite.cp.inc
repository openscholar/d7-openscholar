<?php

/**
 * Implements hook_cp_settings().
 *
 * Note: Site favicon settings are provided in a hook_form_FORM_ID_alter().
 *
 * @see vsite_form_cp_settings_form_alter()
 */
function vsite_cp_settings() {
  $vsite = vsite_get_vsite();
  if (!$vsite || !isset($vsite->group->type)) {
    return;
  }
  $settings = array();

  // Builds the "Site favicon" file field widget.
  // Renders a fieldset with a managed file widget.
  $subsite_parents = variable_get('subsite_parents', array('department', 'project'));
  $inherit_theme = variable_get('vsite_subsite_inherit_theme', 0);
  if (in_array($vsite->group->type, $subsite_parents)) {
    $settings['vsite_subsite_inherit_theme'] = array(
      'form' => array(
        '#type' => 'checkbox',
        '#title' => t('Inherit parent site theme'),
        '#default_value' => $inherit_theme,
        '#description' => t('Check to have newly created subsites inherit this site\'s currently selected theme. This does not apply to custom themes.'),
      ),
      'group' => array(
        '#id' => 'vsite-subsite',
        '#title' => t('Subsite Themes'),
        '#weight' => 31,
        '#menu_parent' => 'appearance',
        '#help_link' => l(t('Learn more about subsite themes'), 'https://help.theopenscholar.com/subsite-themes', array('attributes' => array('target' => '_blank')))
      ),
    );
  }

  $settings['vsite_title'] = array(
    'form' => array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Site title'),
      '#default_value' => $vsite->group->title,
      '#description' => t('Your site\'s official title, as displayed in your Site Info widget, in search results, and in browser tabs.'),
    ),
    'group' => array(
      '#id' => 'vsite-seo',
      '#title' => t('Search Engine Optimization (SEO)'),
      '#weight' => 30,
      '#help_link' => l(t('Learn more about Search Engine Optimization'), 'https://help.theopenscholar.com/global-site-settings#search-engine-optimization', array('attributes' => array('target' => '_blank')))
    ),
    'rest_submit' => 'vsite_rest_cp_settings_submit',
    'submit' => 'vsite_cp_settings_form_submit'
  );

  $meta_description = '';
  $items = field_get_items('node', $vsite->group, 'field_meta_description');
  if (isset($items[0]['value'])) {
    $meta_description = $items[0]['value'];
  }
  $settings['field_meta_description'] = array(
    'form' => array(
      '#type' => 'textarea',
      '#rows' => 2,
      '#title' => t('Meta description'),
      '#resizable' => FALSE,
      '#default_value' => $meta_description,
      '#description' => t('A brief summary to appear in search results for your site\'s home page. Ideally 155 characters or less.'),
    ),
    'group' => array(
      '#id' => 'vsite-seo',
      '#title' => t('Search Engine Optimization (SEO)'),
      '#weight' => 30,
      '#help_link' => l(t('Learn more about Search Engine Optimization'), 'https://help.theopenscholar.com/global-site-settings#search-engine-optimization', array('attributes' => array('target' => '_blank')))
    ),
    'rest_submit' => 'vsite_rest_cp_settings_submit',
    'submit' => 'vsite_cp_settings_form_submit'
  );

  $purl = vsite_get_purl();
  $settings['site_favicon_fid'] = array(
    'form' => array(
      '#type' => 'managed_file',
      '#custom_directive' => 'media-browser-field',
      '#custom_directive_parameters' => array(
        'cardinality' => 1,
        'panes' => array('upload', 'library'),
        'hide_helpicon' => true,
        'droppable_text' => t('Drag your file here'),
        'upload_text' => t('Upload file'),
        'max-filesize' => '100kb',
        'types' => array('png'),
      ),
      '#title' => t('Favicon'),
      '#description' => t('A 16x16 .png file to be displayed in browser shortcut icons and tabs for any page on your site. For help generating a favicon file, try <a href="http://www.favicon.cc/" target="_blank">http://www.favicon.cc/</a>.'),
      '#default_value' => variable_get('site_favicon_fid', ''),
      '#upload_location' => $purl ? "public://{$purl}/files" : "public://",
      '#upload_validators' => array(
        'file_validate_extensions' => array('png'),
        'file_validate_size' => array(2*1024*1024),
      ),
    ),
    'group' => array(
      '#id' => 'Favicon',
      '#title' => t('Favicon'),
      '#weight' => 30,
      '#menu_parent' => 'appearance',
      '#description' => t('A favicon is a 16x16 .ico file to be displayed in browser shortcut iocns and tabs for any page on your site. For help generatiing a favison, try <a href="http://www.favicon.cc/">http://www.favicon.cc/</a>.'),
      '#help_link' => l(t('Learn more about favicon'), 'https://help.theopenscholar.com/adding-site-favicon', array('attributes' => array('target' => '_blank')))
    ),
    'submit' => 'vsite_cp_settings_form_submit',
  );

  $settings['vsite_head_link_rel_publisher'] = array(
    'form' => array(
      '#type' => 'textfield',
      '#title' => t('Publisher URL'),
      '#default_value' => variable_get('vsite_head_link_rel_publisher', ''),
      '#description' => t('Enter the URL to your publishing entity\'s permanent web address. Use a Google+ page URL to be eligible for a branded knowledge panel in Google search results.'),
      '#column' => 'right' //TODO: Remove this when Dan is done with it.
    ),
    'group' => array(
      '#id' => 'vsite-seo',
      '#title' => t('Search Engine Optimization (SEO)'),
      '#weight' => 30,
      '#help_link' => l(t('Learn more about Search Engine Optimization'), 'https://help.theopenscholar.com/global-site-settings#search-engine-optimization', array('attributes' => array('target' => '_blank')))
    ),
    'submit' => 'vsite_cp_settings_form_submit'
  );

  $settings['vsite_head_link_rel_author'] = array(
    'form' => array(
      '#type' => 'textfield',
      '#title' => t('Author URL'),
      '#default_value' => variable_get('vsite_head_link_rel_author', ''),
      '#access' => $vsite->group->type == 'personal' && spaces_access_admin(),
      '#description' => t('Enter the URL to your permanent web address.'),
      '#column' => 'right,' // TODO: Remove this when Dan is done with it.
    ),
    'group' => array(
      '#id' => 'vsite-seo',
      '#title' => t('Search Engine Optimization (SEO)'),
      '#weight' => 30,
      '#help_link' => l(t('Learn more about Search Engine Optimization'), 'https://help.theopenscholar.com/global-site-settings#search-engine-optimization', array('attributes' => array('target' => '_blank')))
    ),
    'submit' => 'vsite_cp_settings_form_submit',
  );

  return $settings;
}

function vsite_rest_cp_settings_submit($value, $var) {
  $values = &drupal_static(__FUNCTION__, array());

  switch ($var) {
    case 'vsite_title':
    case 'field_meta_description':
      $values[$var] = $value;
      if (array_diff_key(array('vsite_title' => false, 'field_meta_description' => false), $values)) {
        return;
      }

      $vsite = vsite_get_vsite();
      $vsite->group->title = $values['vsite_title'];
      $vsite->group->field_meta_description = $vsite->group->metatags = array(
        LANGUAGE_NONE => array(
          0 => array(
            'value' => $values['field_meta_description'],
            'format' => NULL,
          )
        )
      );

      node_save($vsite->group);

      $cid_parts = _vsite_metatag_get_cid_parts($vsite);
      $cid = metatag_cache_default_cid_parts($cid_parts);
      cache_clear_all($cid, 'cache_metatag', TRUE);

      break;
  }
}
