<?php

abstract class OsRestfulDataProvider extends \RestfulDataProviderDbQuery implements \RestfulDataProviderDbQueryInterface, \RestfulDataProviderInterface  {

  /**
   * @var string
   *
   * The string handler.
   */
  protected $validateHandler = '';

  /**
   * @var \stdClass
   *
   * The object the controller need to handle.
   */
  protected $object;

  /**
   * Get the request object.
   *
   * @return object|stdClass
   */
  public function getObject() {

    if (empty($this->object)) {
      // Get the clean request.
      $request = $this->getRequest();
      static::cleanRequest($request);
      $this->object = (object)$request;
    }

    return $this->object;
  }

  /**
   * Validate object.
   *
   * @throws RestfulBadRequestException
   *   Throws exception with the error per object.
   */
  public function validate() {
    if (!$handler = entity_validator_get_schema_validator($this->validateHandler)) {
      return;
    }

    $result = $handler->validate($this->getObject(), TRUE);

    $errors_output = array();
    if (!$result) {
      $e = new \RestfulBadRequestException("It's look that you sent a request with bad values.");
      $fields_errors = $handler->getErrors(FALSE);
      foreach ($fields_errors as $field => $errors) {

        foreach ($errors as $error) {
          $errors_output[$field][] = format_string($error['message'], $error['params']);
        }

        $e->addFieldError($field, implode(', ', $errors_output[$field]));
      }

      throw $e;
    }
  }

}
