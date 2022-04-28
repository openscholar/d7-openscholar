<?php
class MediaInternetEmbedHandler extends MediaInternetBaseHandler {

  // Throws an exception if $this->embedCode is invalid or untrusted.
  public function validate() {
    $valid = (bool) media_embed_check_src($this->embedCode);
    if (!$valid) {
      throw new MediaInternetValidationException(t('Invalid src attribute or @url is from an untrusted domain.', array('@url' => $this->embedCode)));
    }
  }

  public function getFileObject() {
    $uri = 'embed://'. md5($this->embedCode . time());
    $file = file_uri_to_object($uri);
    $file->type = 'html';

    $file->field_html_code[LANGUAGE_NONE][0]['value'] = $this->embedCode;

    return $file;
  }
/*
 * @see MediaInternetBaseHandler::claim()
 */
  public function claim($embed_code) {
    // remove any tags, then see if the string is still the same
    $stripped = strip_tags($embed_code);
    return $stripped != $embed_code;
  }

}