<?php

/**
 * Extension of Drupal private (private://) stream wrapper class.
 *
 * Provides support for loading private files via pinserver.
 */
class OSPinserverPrivateStreamWrapper extends DrupalPrivateStreamWrapper {

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a private file.
   */
  function getExternalUrl() {
    global $base_url;

    $purl_base_domain = variable_get('purl_base_domain');
    $custom_pin_domain = variable_get('os_pinserver_custom_domain', FALSE);
    $private_pin_files = variable_get('os_files_private_pinserver', 0);

    $options = array('absolute' => TRUE, 'purl' => array('disabled' => TRUE));
    if ($custom_pin_domain && ($private_pin_files || $custom_pin_domain != $purl_base_domain)){
      $options['base_url'] = variable_get('os_pinserver_custom_domain', FALSE);
    }

    $path = str_replace('\\', '/', $this->getTarget());

    //Should we serve files via the pinserver URL or the standard private files URL?
    if (!$private_pin_files){
      if(!isset($options['base_url'])){
        //Default to the current baseurl if they don't have a custom one and they are not using pin
        $options['base_url'] = $base_url;
      }

	    return url('system/files/' . $path, $options);
    }

    return url(variable_get('os_pinserver_auth_fast_path','os_fast').'/files/' . $path, $options);
  }
}
