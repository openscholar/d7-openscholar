<?php

/**
 * Implementation of cite_distribute_admin()
 *
 * @return array returns system setting form
 */
function citation_distribute_admin() {
  $form = array();

  $unconfigured = array();
  foreach ( _citation_distribute_plugins() as $plugin ) {
    if (isset($plugin['title']) && !_citation_distribute_is_service_configured($plugin)) {
      $unconfigured[] = $plugin['title'];
    }
  }

  if (count($unconfigured)) {
    $form['citation_distribute_config_message'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages warning">' . t('The following plugin(s) need to be configured:') . implode(', ', $unconfigured) . '</div>',
    );
  }

  $form['citation_distribute'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Citation Distribute'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  // module mode
  $form['citation_distribute']['citation_distribute_module_mode'] = array(
    '#type' => 'select',
    '#title' => t('Select the mode for this module'),
    '#options' => array(
      'api' => 'API Mode',
      'batch' => 'Batch Process Mode',
      'per_submission' => 'Per Submission Mode'
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('citation_distribute_module_mode', 'per_submission'),
    '#description' => t('<strong>API mode</strong> does nothing by default, but allows developers to call Citation Distribute manually.
	      <br><strong>Batch mode</strong> is intended to be run by cron will update all meta files at once in a batch process.
	      <br><strong>Per Submission mode</strong> (<em>default</em>) will update or create a meta file whenever content submitted or updated.')
  );

  // cron limit
  $form['citation_distribute']['citation_distribute_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => 'Batch Size Limit',
    '#description' => t('(Batch mode only) Limit how many publications can be submitted per cron run.'),
    '#required' => FALSE,
    '#default_value' => variable_get('citation_distribute_cron_limit', 10)
  );

  // list all our plugins, include autoflag checkboxes
  $form['citation_distribute']['autoflag'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic Node Flagging'),
    '#description' => 'Publication nodes should automatically select the following services for distribution:<br /><br />',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  foreach ( _citation_distribute_plugins() as $plugin ) {
    if (isset($plugin['title'])) {
      $name = $plugin['class'];
      $form['citation_distribute']['autoflag'][$name . '_auto_flag'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get($name . '_auto_flag', false),
        '#title' => $plugin['title'] . '  (' . $name . ')'
      );
    }
  }

  return system_settings_form($form);
}

/**
 * implementation of form_validate
 *
 * Ensures that cron limit is a positive integer
 */
function citation_distribute_admin_validate($form, &$form_state) {
  if ((int)$form_state['values']['citation_distribute_cron_limit'] <= 0) {
    form_set_error('', t('Batch size limit must be a positive integer.'));
  }
}

/*
 * Fetches and returns admin form from a plugin.
 */
function citation_distribute_plugin_admin_form($form, &$state, $service) {

	$plugins = _citation_distribute_plugins();
	foreach ($plugins as $plugin => $info) {
		if ($service == $info['name']) {
			break;
		}
	}

	if ($instance = _citation_distribute_load_plugin($plugins[$plugin])) {
		if (method_exists($instance, 'admin_form')) {
      $form = $instance->admin_form();
      return system_settings_form($form);
    }
	}

	return array();
}

