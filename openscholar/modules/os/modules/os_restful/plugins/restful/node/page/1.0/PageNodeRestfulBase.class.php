<?php

class PageNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['book'] = array(
      'callback' => array($this, 'getBook'),
    );

    $public_fields['body_classes'] = array(
      'property' => 'field_os_css_class',
    );

    return $public_fields;
  }

  /**
   * Callback for getting book details.
   */
  protected function getBook($wrapper) {
    $node = $wrapper->value();
    if ($node->book['plid'] != 0) {
      $query = db_select('book', 'b');
      $nid = $query->fields('b', array('nid'))
        ->condition('mlid', $node->book['plid'])
        ->execute()->fetchField();
        $node->book['plid'] = $nid;
    }

    $pages = array('p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9');
    foreach ($pages as $page) {
      if ($node->book[$page] != 0) {
        $query = db_select('book', 'b');
        $nid = $query->fields('b', array('nid'))
          ->condition('mlid', $node->book[$page])
          ->execute()->fetchField();
          $node->book[$page] = $nid;
      }
    }
    return $node->book;
  }

}
