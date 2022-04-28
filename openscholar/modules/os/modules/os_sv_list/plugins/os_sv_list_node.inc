<?php 

class os_sv_list_box extends os_sv_list {

  public function __construct() {
    parent::__construct();
    $this->entity_type = 'node';
  }
  
  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $defaults = $this->options_defaults();

    $content_types = array('all' => t('All')) + os_get_bundles(array(1));
    // Removing Feed Importer and Slideshow Image from dropdown.
    foreach ($content_types as $key => $val) {
      if ($key == 'feed_importer' || $key == 'slideshow_slide') {
        unset($content_types[$key]);
      }
    }
    $form['content']['content_type']['#options'] = $content_types;
    return $form;
  }

  public function query() {
    $efq = parent::query();
    $efq->propertyCondition('status', 1); 
    return $efq;
  }

  /**
   * Add our node specific sorts.
   */
  protected function _sort_options() {
    // Standard sorts.
    $sorts = array(
      'sort_newest' => t('Newest Post'),
      'sort_oldest' => t('Oldest Post'),
      'sort_alpha' => t('Alphabetical'),
      'sort_random' => t('Random'),
    );

    return parent::_sort_options() + $sorts;
  }

  /**
   * sorting functions.
   */
  function sort_newest(EntityFieldQuery &$efq) {
    //could thiese be made generic by using entity_get_info to figure out $created and $title per type?
    $efq->propertyOrderBy('created', 'DESC');
  }

  function sort_oldest(EntityFieldQuery &$efq) {
    $efq->propertyOrderBy('created', 'ASC');
  }

  function sort_alpha(EntityFieldQuery &$efq) {
    $efq->propertyOrderBy('title', 'ASC');
  }
}
