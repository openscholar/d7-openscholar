<?php

/**
 * @file
 * The class for "Book Table of Contents" widgets.
 */
class os_boxes_sectionnav extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    $block = parent::render();

    if ($node = menu_get_object()) {
      // only pages can have section nav
      if ($node->type == 'page' && isset($node->book['bid'])) {
        $block = os_books_build_toc($node) + $block;
      }
    }

    return $block;
  }
}
