Drupal.wysiwyg.plugins['colorbutton'] = CKEDITOR.plugins.get('colorbutton');
Drupal.wysiwyg.plugins['justify'] = CKEDITOR.plugins.get('justify');
Drupal.wysiwyg.plugins['font'] = CKEDITOR.plugins.get('font');
Drupal.wysiwyg.plugins['panelbutton'] = CKEDITOR.plugins.get('panelbutton');
Drupal.wysiwyg.plugins['dragresize'] = CKEDITOR.plugins.get('dragresize');
Drupal.wysiwyg.plugins['mathjax'] = CKEDITOR.plugins.get('mathjax');
Drupal.wysiwyg.plugins['lineutils'] = CKEDITOR.plugins.get('lineutils');
Drupal.wysiwyg.plugins['widget'] = CKEDITOR.plugins.get('widget');
Drupal.wysiwyg.plugins['colordialog'] = CKEDITOR.plugins.get('colordialog');
Drupal.wysiwyg.plugins['image2'] = CKEDITOR.plugins.get('image2');
Drupal.wysiwyg.plugins['indentblock'] = CKEDITOR.plugins.get('indentblock');
Drupal.wysiwyg.plugins['bidi'] = CKEDITOR.plugins.get('bidi');

// Override the normal link functionality.
CKEDITOR.plugins.original_link = CKEDITOR.plugins.link;
CKEDITOR.plugins.link = Drupal.wysiwyg.plugins['os_link'];

// Remove unsupported fonts.
CKEDITOR.config.font_names =
  'Andale Mono;' +
  'Arial/Arial, Helvetica, sans-serif;' +
  'Ariel Black;' +
  'Book Antiqua;' +
  'Comic Sans MS/Comic Sans MS, cursive;' +
  'Courier New/Courier New, Courier, monospace;' +
  'Georgia/Georgia, serif;' +
  'Helvetica Neue Lt Std;' +
  'Impact;' +
  'Lucida Sans Unicode/Lucida Sans Unicode, Lucida Grande, sans-serif;' +
  'Symbol;' +
  'Tahoma/Tahoma, Geneva, sans-serif;' +
  'Terminal;' +
  'Times New Roman/Times New Roman, Times, serif;' +
  'Trebuchet MS/Trebuchet MS, Helvetica, sans-serif;' +
  'Verdana/Verdana, Geneva, sans-serif;' +
  'Wingdings;';

CKEDITOR.config.autoParagraph = false;
