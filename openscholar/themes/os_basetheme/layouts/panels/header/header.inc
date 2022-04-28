<?php
function adaptivetheme_header_panels_layouts() {
  $items['header'] = array(
    'title'    => t('OS Header'),
    'category' => t('OS Responsive Panels - 3 column'),
    'icon'     => 'header.png',
    'theme'    => 'header',
    'admin css' => 'header.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'header_top'    => t('Top (conditional)'),
      'header_first'  => t('Left'),
      'header_second' => t('Center'),
      'header_third'  => t('Right'),
      'header_bottom' => t('Bottom (conditional)'),
    ),
    // AT
    'type' => 'three',
    'options' => array(
      'header' => 'default',
      'header-stack-top' => 'stack bottom',
      'header-stack-bottom' => 'stack top',
      'header-stack' => 'stack',
    ),
    'styles' => array(
      'header' => array(
        'css' => array(
          '33' => array('.header .region' => 'width:33.333333%'),
        ),
      ),
       'header-stack-top' => array(
        'css' => array(
          '50' => array('.header .region' => 'width:50%'),
          'fdw' => array('.header .region-header-first' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'header-stack-bottom' => array(
        'css' => array(
          '50' => array('.header .region' => 'width:50%'),
          'fdw' => array('.header .region-header-third' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'header-stack' => array(
        'css' => array(
          'fdw' => array('.header .region' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
    ),
  );

  return $items;
}

function template_preprocess_header(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
