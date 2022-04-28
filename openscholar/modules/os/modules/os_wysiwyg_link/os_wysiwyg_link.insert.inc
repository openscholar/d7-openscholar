<?php

/**
* Provides the insert link overlay including forms for each link URL type.
*/
function os_wysiwyg_link_insert() {
  module_invoke_all('suppress', TRUE);

  module_load_include('inc', 'file_entity', 'file_entity.pages');
  $tabs = array(
    'external' => array(
      'title' => 'Web Address',
    	'form' => '_os_link_external_form',
    ),
    'email' => array(
      'title' => 'E-mail',
      'form' => '_os_link_email_form',
    ),
    'file' => array(
      'title' => 'Files',
      'page' => '_os_link_get_view',
      'params' => array(),
    ),/*
    'page' => array(
      'title' => 'Page',
      'form' => '_os_link_internal_form',
    ),*/
  );

  if (isset($_GET['active']) && isset($tabs[$_GET['active']])) {
     $tabs[$_GET['active']]['default'] = TRUE;
     $tabs[$_GET['active']]['params'] = array('url' => $_GET['url']);
  }

  $output = array(
    'text' => array(
      '#type' => 'textfield',
      '#title' => t('Text To Display'),
      '#description' => t('The text of the link. Leave blank to use the url of the link itself.'),
      '#name' => 'link-text',
    ),
    'title' => array(
      '#type' => 'textfield',
      '#id' => 'edit-link-title',
      '#title' => t('Title Text'),
      '#description' => t('This text will appear on mouse hover and is used by screen readers, however, it will not appear for keyboard-only users or touch-only users.'),
      '#name' => 'link-title',
    ),
    'tabs' => array(
      '#title' => t('Link To'),
      '#type' => 'vertical_tabs',
      'tabs__active_tab' => array(
        '#type' => 'hidden',
        '#default_value' => '',
        '#attributes' => array('class' => array('vertical-tabs-active-tab')),
      )
    )
  );
  foreach ($tabs as $k => $t) {
    $output['tabs'][$k] = array(
      '#type' => 'fieldset',
      '#title' => $t['title'],
      '#id' => 'edit-'.$k,
    );
    if (isset($t['form'])) {
      $output['tabs'][$k]['form'] = drupal_get_form($t['form'], (!empty($t['params'])?$t['params']:array()));
    }
    elseif (isset($t['page'])) {
      $output['tabs'][$k]['page'] = $t['page']((!empty($t['params'])?$t['params']:array()));
    }
    if (!empty($t['default'])) {
      $output['tabs']['#default_tab'] = 'edit-'.$k;
      $output['tabs']['tabs__active_tab']['#value'] = 'edit-'.$k;
    }
  }

  // handle file uploads
  if ($form = $output['tabs']['file']['page']['upload']) {
    if (isset($form['upload']['#validated'])) {
      $commands = array();
      if ($form['upload']['#file']) {
        $entity = $form['upload']['#file'];
        if (isset($entity->fid)) {
          media_browser_build_media_item($entity);
          drupal_add_js(array('media' => array('files' => array($entity))), 'setting');

          $commands[] = ajax_command_replace('.view-media-default .view-empty', '<div class="view-content"><div class="clearfix"><ul id="media-browser-library-list" class="media-list-thumbnails"></ul></div>');
          $commands[] = ajax_command_prepend('#media-browser-library-list', '<li id="media-item-'.$entity->fid.'" class="media-item-new">'.$entity->preview.'</li>');
          // clear out status messages
          theme('status_messages');

          $commands[] = array(
            'command' => 'clickOn',
            'target' => '#media-item-'.$entity->fid.' div.media-item',
          );
        }
      }
      else {
        $commands[] = ajax_command_prepend('.views-exposed-form', theme('status_messages'));
      }
      // return results to ajax script
      $output = array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
      ajax_deliver($output);
      exit;
    }
  }

  $output['actions'] = array(
    '#type' => 'container',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
    ),
    '#attributes' => array(
      'class' => array('insert-buttons'),
    ),
  );
  $path = drupal_get_path('module', 'os_wysiwyg_link');
  $output['#attached'] = array(
    'library' => array(
      array('media', 'media_browser_page'),
    ),
    'css' => array($path.'/theme/os_wysiwyg_link_form.css'),
    'js' => array($path.'/js/os_wysiwyg_link_form.js'),
  );

  return $output;
}

/**
* Form builder; provides form for "Web address" tab.
*/
function _os_link_external_form($form, $form_state, $params) {
  $form = array();
  $args = func_get_args();

  $form['external'] = array(
    '#type' => 'textfield',
    '#title' => t('Website URL'),
    '#description' => t('The URL of the web page. Can be a link to one of your own pages.'),
    '#maxlength' => 500,
    '#default_value' => isset($params['url'])?$params['url']:'',
  );

  $form['target_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open this link in a new tab'),
    '#description' => t('Note: depending on the usage, may cause accessibility concerns. <a href="@link" target="_blank">Learn more</a>', array('@link' => 'https://www.w3.org/TR/WCAG20-TECHS/G200.html')),
    '#default_value' => 0,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    )
  );

  return $form;
}

/**
* Form builder; provides form for "Page" tab.
*/
function _os_link_internal_form($form, $form_state, $params) {
  $form = array();

  $form['coming_soon'] = array(
    '#markup' => t('Coming soon!'),
  );
  /*$form['internal'] = array(
    '#type' => 'textfield',
    '#title' => t('Page'),
    '#description' => t('The page of your site to link to.'),
    '#default_value' => isset($params['url'])?$params['url']:'',
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    ),
  );*/

  return $form;
}

/**
* Form builder; provides form for "Files" tab.
*/
function _os_link_get_view($params) {
  $name = 'media_default';
  $display = 'media_browser_1';
  if (isset($params['url'])) {
    $_GET['fid'] = $params['url'];
  }

  if ($view = views_get_view($name)) {
    $view->set_items_per_page(12);
    if ($view->set_display($display)) {
      $view->display_handler->options['items_per_page'] = 12;
      $build['#markup'] = $view->preview();

      // Allow the View title to override the plugin title.
      if ($title = $view->get_title()) {
        $build['#title'] = $title;
      }

      $output = array(
        'upload' => drupal_get_form('file_entity_add_upload', array(
          'file_directory' => os_files_get_save_path(),
          'no_redirect' => TRUE,
        )),
      	'library' => $build,
        'actions' => array(
          '#type' => 'actions',
          '#attributes' => array(
            'class' => array('form-actions')
          ),
          'insert' => array(
            '#type' => 'submit',
            '#value' => t('Insert'),
          )
        ),
        '#attached' => array(
          'library' => array(
            array('media', 'media_browser_page'),
          ),
          'js' => array(
            drupal_get_path('module', 'media') . '/js/plugins/media.views.js',
            array(
              'type' => 'setting',
              'data' => array('media' =>
                array(
                  'browser' => array(
                    'params' => media_get_browser_params(),
                  )
                )
              )
            )
          )
        )
      );

      return $output;
    }
  }

  return array();
}

/**
* Form builder; provides form for "E-mail" tab.
*/
function _os_link_email_form($form, $form_state, $params) {
  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => isset($params['url'])?$params['url']:'',
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'insert' => array(
      '#type' => 'submit',
      '#value' => 'Insert',
    ),
  );

  return $form;
}

