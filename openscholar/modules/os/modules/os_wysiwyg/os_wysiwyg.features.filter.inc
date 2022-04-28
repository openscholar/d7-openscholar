<?php
/**
 * @file
 * os_wysiwyg.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function os_wysiwyg_filter_default_formats() {
  $formats = array();

  // Exported format: Filtered HTML.
  $formats['filtered_html'] = array(
    'format' => 'filtered_html',
    'name' => 'Filtered HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => -10,
    'filters' => array(
      'owl_filter' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(),
      ),
      'wysiwyg' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(
          'valid_elements' => '@[id|style|class],
    a[href|target<_blank|title|name|rel|download],
    div[align<center?justify?left?right],
    p[dir|align<center?justify?left?right],
    br,span,cite,code,blockquote,ul,ol,li,dl,dt,dd,img[!src|width|height|alt|title],
    sup,sub,
    table[scope|id|headers|style|border|align<center?left?right|cellpadding|cellspacing|bgcolor],
    tr[scope|id|headers|align<center?justify?left?right|valign],
    td[scope|id|headers|colspan|rowspan|align<center?justify?left?right|valign],
    th[scope|id|headers|colspan|rowspan|align<center?justify?left?right|valign],
    thead[scope|id|headers],
    caption,figcaption,
    address,h2,h3,h4,h5,h6,pre,
    strong/b,em/i,strike,s,u,abbr[title]',
          'allow_comments' => 1,
          'nofollow_policy' => 'disabled',
          'nofollow_domains' => array(),
          'style_color' => array(
            'color' => 'color',
            'background-color' => 'background-color',
            'background' => 0,
            'background-image' => 0,
            'background-repeat' => 0,
            'background-attachment' => 0,
            'background-position' => 0,
          ),
          'style_font' => array(
            'font' => 'font',
            'font-family' => 'font-family',
            'font-size' => 'font-size',
            'font-style' => 'font-style',
            'font-weight' => 'font-weight',
            'font-size-adjust' => 0,
            'font-stretch' => 0,
            'font-variant' => 0,
          ),
          'style_text' => array(
            'text-align' => 'text-align',
            'text-decoration' => 'text-decoration',
            'text-indent' => 'text-indent',
            'text-transform' => 'text-transform',
            'direction' => 'direction',
            'letter-spacing' => 0,
            'word-spacing' => 0,
            'white-space' => 0,
            'unicode-bidi' => 0,
          ),
          'style_box' => array(
            'margin' => 'margin',
            'margin-top' => 'margin-top',
            'margin-right' => 'margin-right',
            'margin-bottom' => 'margin-bottom',
            'margin-left' => 'margin-left',
            'padding' => 'padding',
            'padding-top' => 'padding-top',
            'padding-right' => 'padding-right',
            'padding-bottom' => 'padding-bottom',
            'padding-left' => 'padding-left',
          ),
          'style_border-1' => array(
            'border' => 'border',
            'border-top' => 'border-top',
            'border-right' => 'border-right',
            'border-bottom' => 'border-bottom',
            'border-left' => 'border-left',
            'border-width' => 'border-width',
            'border-top-width' => 'border-top-width',
            'border-right-width' => 'border-right-width',
            'border-bottom-width' => 'border-bottom-width',
            'border-left-width' => 'border-left-width',
          ),
          'style_border-2' => array(
            'border-color' => 'border-color',
            'border-top-color' => 'border-top-color',
            'border-right-color' => 'border-right-color',
            'border-bottom-color' => 'border-bottom-color',
            'border-left-color' => 'border-left-color',
            'border-style' => 'border-style',
            'border-top-style' => 'border-top-style',
            'border-right-style' => 'border-right-style',
            'border-bottom-style' => 'border-bottom-style',
            'border-left-style' => 'border-left-style',
            'border-radius' => 'border-radius',
          ),
          'style_dimension' => array(
            'height' => 'height',
            'width' => 'width',
            'line-height' => 0,
            'max-height' => 0,
            'max-width' => 0,
            'min-height' => 0,
            'min-width' => 0,
          ),
          'style_positioning' => array(
            'vertical-align' => 'vertical-align',
            'bottom' => 0,
            'clip' => 0,
            'left' => 0,
            'overflow' => 0,
            'right' => 0,
            'top' => 0,
            'z-index' => 0,
          ),
          'style_layout' => array(
            'clear' => 'clear',
            'display' => 'display',
            'float' => 'float',
            'position' => 'position',
            'visibility' => 0,
          ),
          'style_list' => array(
            'list-style' => 0,
            'list-style-image' => 0,
            'list-style-position' => 0,
            'list-style-type' => 0,
          ),
          'style_table' => array(
            'border-collapse' => 'border-collapse',
            'border-spacing' => 'border-spacing',
            'table-layout' => 'table-layout',
            'caption-side' => 0,
            'empty-cells' => 0,
          ),
          'style_user' => array(
            'cursor' => 0,
            'outline' => 0,
            'outline-width' => 0,
            'outline-style' => 0,
            'outline-color' => 0,
            'zoom' => 0,
          ),
          'rule_valid_classes' => array(
            0 => '*',
          ),
          'rule_valid_ids' => array(
            0 => 'os-*',
          ),
          'rule_style_urls' => array(),
        ),
      ),
      'filter_url' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'media_filter' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: Full HTML.
  $formats['full_html'] = array(
    'format' => 'full_html',
    'name' => 'Full HTML',
    'cache' => 0,
    'status' => 1,
    'weight' => -8,
    'filters' => array(
      'filter_autop' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'oembed' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'options' => '',
          'view_mode' => 'full',
          'autoembed' => 1,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: Plain text.
  $formats['plain_text'] = array(
    'format' => 'plain_text',
    'name' => 'Plain text',
    'cache' => 1,
    'status' => 1,
    'weight' => -7,
    'filters' => array(
      'filter_html_escape' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 1,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'filter_autop' => array(
        'weight' => 2,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
