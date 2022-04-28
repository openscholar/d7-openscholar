<?php
function adaptivetheme_content_panels_layouts() {
  $items['content'] = array(
    'title'    => t('OS Content Regions'),
    'category' => t('OS Responsive Panels - 2 column'),
    'icon'     => 'content.png',
    'theme'    => 'content',
    'admin css' => 'content.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'content_top'    => t('Top (conditional)'),
      'content_first'  => t('Left'),
      'content_second' => t('Right'),
      'content_bottom' => t('Bottom (conditional)'),
    ),
    // AT
    'type' => 'two',
    'options' => array(
      'content' => 'default',
      'content-stack' => 'stack',
    ),
    'styles' => array(
      'content' => array(
        'css' => array(
          '50' => array('.content .region' => 'width:50%'),
        ),
      ),
      'content-stack' => array(
        'css' => array(
          'fdw' => array('.content .region' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
    ),
  );

  return $items;
}

function template_preprocess_content(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
