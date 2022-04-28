<?php

/**
 * OS Site context condition, to deterimine if you are in a OS Site.
 */
class os_context_condition_os_site extends context_condition {

  function condition_values() {
    return array(
      1 => t('In a OS Site'),
    );
  }

  function editor_form($context = NULL) {
    $form = parent::editor_form($context);
    $form[1]['#title'] = t('In a OS Site');
    $form['#weight'] = - 10;
    return $form;
  }

  function execute() {
    if ($this->condition_used()) {

      // Default: NON Multi Tenant Always in a site
      $value = 1;
      if (module_exists('vsite')) {
        $vsite = spaces_get_space();
        if (!$vsite) {
          $value = 0;
        }
      }

      foreach ($this->get_contexts($value) as $context) {
        $this->condition_met($context, $value);
      }
    }
  }
}
