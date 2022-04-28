<?php

class OsRestfulRedirect extends \RestfulBase implements \RestfulDataProviderInterface {

  public function publicFieldsInfo() {
    // what is this even for
  }

  public function create() {
    $redirect = new stdClass();

    // Initiate the return message
    $redirect->msg = array();

    // Merge default values.
    redirect_object_prepare($redirect, array(
      'source' => isset($this->request['path']) ? urldecode($this->request['path']) : '',
      'source_options' => array(),
      'redirect' => isset($this->request['target']) ? urldecode($this->request['target']) : '',
      'redirect_options' => array(),
      'language' => LANGUAGE_NONE,
    ));

    if (!empty($_GET['vsite']) && $vsite = vsite_get_vsite($_GET['vsite'])) {
      // force verify_purl to be run
      $vsite->get_absolute_url();
      $redirect->redirect_options['purl'] = array(
        'add' => array(
          0 => array(
            'provider' => $vsite->purl_provider,
            'id' => $vsite->id,
          )
        ),
      );
    }

    // Check for empty source and redirect
    if (empty($redirect->source) || empty($redirect->redirect)) {
      $error->msg[] = t("The redirect from or to fields may not be submitted without a value.");
      $error->msg_type = 'error';
      return $this->renderRedirect($error);
    }

    // check that there there are no redirect loops
    if (url($redirect->source) == url($redirect->redirect)) {
      throw new RestfulBadRequestException(t('You are attempting to redirect the page to itself. This will result in an infinite loop.'));
    }

    redirect_hash($redirect);
    if ($existing = redirect_load_by_hash($redirect->hash)) {
      if ($redirect->rid != $existing->rid) {
        throw new RestfulBadRequestException(t('The source path %source is already being redirected. Do you want to <a href="@edit-page">edit the existing redirect</a>?', array('%source' => redirect_url($redirect->source, $redirect->source_options), '@edit-page' => url('admin/config/search/redirect/edit/'. $existing->rid))));
      }
    }

    // Removing leading slash or hash from the source
    if (substr($redirect->source, 0, 1) === '/' || substr($redirect->source, 0, 1) === '#') {
      $remove_character = substr($redirect->source, 0, 1);
      $redirect->source = ltrim($redirect->source, $remove_character);
      $redirect->msg[] = t("Leading ' %character ' is not allowed, it has been removed and saved.", array('%character' => $remove_character));
      $error->msg_type = 'warning';
    }
    // Saving to DB
    redirect_save($redirect);

    // Build full drupal system url
    if (isset($vsite->group->purl)) {
      $full_redirect_url = $vsite->group->purl . '/' .$redirect->source;
    } else {
      $full_redirect_url = $redirect->source;
    }

    // Check for exiting url and then add a message
    if (drupal_valid_path(drupal_get_normal_path($full_redirect_url))) {
      $redirect->msg[] = 'The url already exists. If this is unintended, the redirected can be deleted below.';
      $error->msg_type = 'warning';
    }

    return $this->renderRedirect($redirect);
  }

  public function index() {
    if (!empty($this->request['vsite']) && $vsite = vsite_get_vsite($this->request['vsite'])) {
      return array();
    }
  }

  public function view($id) {
    $id = trim($id, '/');
    $redirect = redirect_load($id);
    return $this->renderRedirect($redirect);
  }

  public function remove($id) {
    $id = trim($id, '/');
    redirect_delete($id);

    // Set the HTTP headers.
    $this->setHttpHeaders('Status', 204);
  }

  public function renderRedirect($redirect) {
    return array(
      'id' => $redirect->rid,
      'path' => $redirect->source,
      'target' => $redirect->redirect,
      'msg' => $redirect->msg,
      'msg_type' => $redirect->msg_type,
    );
  }
}
