<?php
function adaptivetheme_footer_panels_layouts() {
  $items['footer'] = array(
    'title'    => t('OS footer'),
    'category' => t('OS Responsive Panels - 3 column'),
    'icon'     => 'footer.png',
    'theme'    => 'footer',
    'admin css' => 'footer.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'footer_top'    => t('Top (conditional)'),
      'footer_first'  => t('Left'),
      'footer' => t('Center'),
      'footer_third'  => t('Right'),
      'footer_bottom' => t('Bottom (conditional)'),
    ),
    // AT
    'type' => 'three',
    'options' => array(
      'footer' => 'default',
      'footer-stack-top' => 'stack bottom',
      'footer-stack-bottom' => 'stack top',
      'footer-stack' => 'stack',
    ),
    'styles' => array(
      'footer' => array(
        'css' => array(
          '33' => array('.footer .region' => 'width:33.333333%'),
        ),
      ),
       'footer-stack-top' => array(
        'css' => array(
          '50' => array('.footer .region' => 'width:50%'),
          'fdw' => array('.footer .region-footer-first' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'footer-stack-bottom' => array(
        'css' => array(
          '50' => array('.footer .region' => 'width:50%'),
          'fdw' => array('.footer .region-footer-third' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
      'footer-stack' => array(
        'css' => array(
          'fdw' => array('.footer .region' => 'float:none;display:block;width:100%;clear:both'),
        ),
      ),
    ),
  );

  return $items;
}

function template_preprocess_footer(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
