<?php 

class os_sv_list_file extends os_sv_list {

  public function __construct() {
    parent::__construct();
    $this->entity_type = 'file';
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $types = array('all' => t('All')) + 
      array_reduce(file_type_get_all_types(), function ($ret, $o) {$ret[$o->type] = $o->label; return $ret;});
    
    if (isset($types['html'])) {
      $types['html'] = t('Embedded Media');
    }
    $form['content']['content_type']['#title'] = t('File Type');
    $form['content']['content_type']['#options'] = $types;
    return $form;
  }
 
  /**
   * @function query()
   * 
   * Remove html files from ever showing up in sv list.
   */
  public function query() {
    $efq = parent::query();
    return $efq;
  }
  
  
  protected function _sort_options() {
    return array(
      'sort_newest' => t('Newest'),
      'sort_oldest' => t('Oldest'),
      'sort_alpha' => t('Alphabetical'),
      'sort_filesize' => t('Filesize'),
      'sort_random' => t('Random'),
    );
  }

  /**
   * sorting functions
   */
  function sort_newest(&$efq) {
    $efq->propertyOrderBy('timestamp', 'DESC');
  }

  function sort_oldest(&$efq) {
    $efq->propertyOrderBy('timestamp', 'ASC');
  }

  function sort_alpha(&$efq) {
    $efq->propertyOrderBy('filename', 'ASC');
  }

  function sort_filesize(&$efq) {
    $efq->propertyOrderBy('filesize', 'DESC');
  }

  /**
   * Check access to the the box.
   *
   * We return here TRUE because in the case of file, a public file attached to
   * a private bundle would still be considered public.
   */
  public function checkWidgetAccess($bundle) {
    return TRUE;
  }

}
