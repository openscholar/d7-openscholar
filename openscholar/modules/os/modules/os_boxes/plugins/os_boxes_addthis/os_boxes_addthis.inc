<?php

/**
 * @file
 * The class for AddThis boxes.
 */

class os_boxes_addthis extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();
    $options['widget_type'] = 'buttons';
    $options['toolbox_services'] = 'facebook,twitter,email,linkedin,google_plusone';
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);

    global $base_url;
    $dir = $base_url . '/' . $mod = drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_addthis/images/';

    $form['widget_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display style'),
      '#default_value' => $this->options['widget_type'],
      '#options' => array(
        'buttons' => "<img src='$dir/addthis_button.png' />\n",
        'toolbox_small' => "<img src='$dir/addthis_sm.png' />\n",
        'toolbox_large' => "<img src='$dir/addthis_lg.png' />\n",
        'numeric' => "<img src='$dir/addthis_numbers.gif' />\n",
        'counter' =>  "<img src='$dir/addthis_counter.png' />\n",
      ),
    );

    $form['toolbox_services'] = array(
      '#type' => 'value',
      '#title' => 'Toolbox services',
      '#value' => $this->options['toolbox_services'],
      '#description' => 'A comma separated list of social media links service codes.  (ie: twitter, facebook, google).  For full listing of service codes, please see ' . l('AddThis', 'http://www.addthis.com/services/list#.TlvYUhiwe9E')
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }

    switch ($this->options['widget_type']) {

      case 'buttons' :
        $html = '<a class="addthis_button" href=" http://www.addthis.com/bookmark.php?v=250&amp;pubid=xa-4e9eeefa6983da55 "><img src="/'.drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_addthis/images/addthis_smallbar.png" width="125" height="16" alt="Bookmark and Share" style="border:0"/></a>';
        break;

      case 'toolbox_small' :
        $html  = '<div class="addthis_toolbox addthis_default_style "><a class="addthis_button_facebook"></a>';
        $html .= '<a class="addthis_button_twitter"></a>';
        $html .= '<a class="addthis_button_email"></a>';
        $html .= '<a class="addthis_button_linkedin"></a>';
        $html .= '<a class="addthis_button_google_plusone"></a>';
        $html .= '</div>';
        break;

      case 'toolbox_large' :
        $html  = '<div class="addthis_toolbox addthis_default_style addthis_32x32_style">';
        $html .= '<a class="addthis_button_facebook"></a>';
        $html .= '<a class="addthis_button_twitter"></a>';
        $html .= '<a class="addthis_button_email"></a>';
        $html .= '<a class="addthis_button_linkedin"></a>';
        $html .= '<a class="addthis_button_google_plusone"></a>';
        $html .= '</div>';
        break;

      case 'numeric' :
        $html  = '<div class="addthis_toolbox addthis_default_style ">';
        $html .= '<a class="addthis_button_facebook_like" fb:like:layout="button_count"></a>';
        $html .= '<a class="addthis_button_tweet"></a>';
        $html .= '<a class="addthis_button_google_plusone" g:plusone:size="medium"></a>';
        $html .= '<a class="addthis_counter addthis_pill_style"></a></div>';
        break;

      case 'counter' :
        $html = '<div class="addthis_toolbox addthis_default_style "><a class="addthis_counter"></a></div>';
        break;
    }

    $block = parent::render();
    $block['content'] = $html;
    drupal_add_css(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_addthis/os_boxes_addthis.css');
    drupal_add_js('//s7.addthis.com/js/250/addthis_widget.js#pubid=xa-4e9eeefa6983da55', 'external');

    //after removing and readding this widget, page needs a reload.  trigger it with js
    if (preg_match('/os\/widget\/boxes\/[^\/]*\/remove\/\w/', $_GET['q'])) {
      $block['content'] .= '<script type="text/javascript">
      	if (addthis) { 
      		setTimeout(function () {
      			addthis.toolbox(".addthis_toolbox");
      	  },1); 
    	} </script>';
    }
    return $block;
  }

}
