<?php
/**
 * @api {get} /api/files Request Site Files
 * @apiName GetFiles
 * @apiGroup File
 *
 * @apiParam {Number} vsite  Optional VSite to retrieve files from
 *
 * @apiSuccess {Object[]} files List of files in the site.
 *
 * @apiSuccessExample Success-Response:
 *     HTTP/1.1 200 OK
 *     {
 *       {
 *        "id":75061,
 *        "label":"Image One",
 *        "self":"http://staging.scholar.harvard.edu/api/v1.0/files/75061",
 *        "size":"37466",
 *         "mimetype":"image/jpeg",
 *        "url":"http://staging.scholar.harvard.edu/files/rbrandon/files/cafu1.jpg",
 *        "schema":"public",
 *        "filename":"cafu1.jpg",
 *        "type":"image",
 *        "name":"Image One",
 *        "timestamp":"1360044636",
 *        "description":null,
 *        "image_alt":'alt text',
 *        "image_title":null,
 *        "preview":"
 *        img1
 *        img1 (cafu1.jpg)
 *        ",
 *        "terms":null
 *      },...
 *     }
 */

/**
 * @api {post} /api/files Save File
 * @apiName SaveFile
 * @apiGroup File
 *
 * @apiParam {Number} vsite  VSite to save the file to
 * @apiParam {Object} data  File metadat
 * @apiParam {Object} files[upload]  File Data
 *
 * @apiParamExample {multipart/form-data} Request-Example:
 *     ------WebKitFormBoundaryXgmJRlIas3M22RWQ
 *         Content-Disposition: form-data; name="vsite"
 *         2664
 *     ------WebKitFormBoundaryXgmJRlIas3M22RWQ
 *         Content-Disposition: form-data; name="data"
 *         {"lastModified":1424292767000,"lastModifiedDate":"2015-02-18T20:52:47.000Z","name":"jassleep.jpg","type":"image/jpeg","size":1967014}
 *     ------WebKitFormBoundaryXgmJRlIas3M22RWQ
 *         Content-Disposition: form-data; name="files[upload]"; filename="jassleep.jpg"
 *         Content-Type: image/jpeg
 *     ------WebKitFormBoundaryXgmJRlIas3M22RWQ--
 *
 * @apiSuccess {Object} file The saved file
 *
 * @apiSuccessExample Success-Response:
 *     HTTP/1.1 200 OK
 *     {
 *       "data":[
 *          {
 *             "id":"330546",
 *             "label":"jassleep.jpg",
 *             "self":"http:\/\/staging.scholar.harvard.edu\/api\/v1.0\/files\/330546",
 *             "size":"1967014",
 *             "mimetype":"image\/jpeg",
 *             "url":"http:\/\/staging.scholar.harvard.edu\/files\/rbrandon\/files\/jassleep.jpg",
 *             "schema":"public",
 *             "filename":"jassleep.jpg",
 *             "type":"image",
 *             "name":"jassleep.jpg",
 *             "timestamp":"1431716541",
 *             "description":null,
 *             "image_alt":null,
 *             "image_title":null,
 *             "preview":"<div...preview markup",
 *             "terms":null
 *          }
 *       ],
 *       "self":{
 *          "title":"Self",
 *          "href":"http:\/\/staging.scholar.harvard.edu\/api\/v1.0\/files"
 *     }
 *}
 *
 */

/**
 * @api {patch} /api/files/:fid Update a File
 * @apiName UpdateFile
 * @apiGroup File
 *
 * @apiParam {Number} fid  A files unique ID
 * @apiParam {Object} file  File Object parameters to save
 *
 * @apiParamExample {json} Request-Example:
 *     {"name":"Jasper Sleeping","description":"My Images Description","image_alt":"Alternate TXT","image_title":"Mouseover"}
 *
 * @apiSuccess {Object} file The saved file
 *
 * @apiSuccessExample Success-Response:
 *     HTTP/1.1 200 OK
 *     {
 *       "data":[
 *          {
 *             "id":"330546",
 *             "label":"jassleep.jpg",
 *             "self":"http:\/\/staging.scholar.harvard.edu\/api\/v1.0\/files\/330546",
 *             "size":"1967014",
 *             "mimetype":"image\/jpeg",
 *             "url":"http:\/\/staging.scholar.harvard.edu\/files\/rbrandon\/files\/jassleep.jpg",
 *             "schema":"public",
 *             "filename":"jassleep.jpg",
 *             "type":"image",
 *             "name":"jassleep.jpg",
 *             "timestamp":"1431716541",
 *             "description":null,
 *             "image_alt":null,
 *             "image_title":null,
 *             "preview":"<div...preview markup",
 *             "terms":null
 *          }
 *       ],
 *       "self":{
 *          "title":"Self",
 *          "href":"http:\/\/staging.scholar.harvard.edu\/api\/v1.0\/files"
 *     }
 *}
 *
 */
class OsFilesResource extends OsRestfulEntityCacheableBase {

  protected $errors = array();

  public static function controllersInfo() {
    return array(
      '\d\/image_style\/\w*' => array(
        RestfulInterface::GET => 'getImageStyle',
      ),
      'filename\/[^\/]+$' => array(
        RestfulInterface::GET => 'checkFilename',
        RestfulInterface::HEAD => 'checkFilename'
      )
    ) + parent::controllersInfo();
  }

  public function initSpace() {
    static $hasRun = false;
    $vid = 0;
    if (!empty($GET['vsite'])) {
      $vid = $_GET['vsite'];
    }
    else if (!empty($this->request['vsite'])) {
      $vid = $this->request['vsite'];
    }
    if (!$hasRun && $vsite = vsite_get_vsite($vid)) {
      spaces_set_space($vsite);
      $vsite->activate_user_roles();
      $hasRun = true;
    }
  }

  /**
   * Overrides RestfulEntityBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $info = parent::publicFieldsInfo();

    $info['vsite'] = array(
      'property' => OG_AUDIENCE_FIELD,
      'process_callbacks' => array(
        array($this, 'vsiteFieldDisplay'),
      ),
    );

    $info['size'] = array(
      'property' => 'size',
      'discovery' => array(
        'data' => array(
          'type' => 'int',
          'read_only' => TRUE,
        )
      )
    );

    $info['mimetype'] = array(
      'property' => 'mime',
      'discovery' => array(
        'data' => array(
          'type' => 'string',
          'read_only' => TRUE,
        )
      )
    );

    $info['icon'] = array(
      'callback' => array($this, 'getIconUrl')
    );

    $info['url'] = array(
      'property' => 'url',
    );

    $info['schema'] = array(
      'callback' => array($this, 'getSchema'),
    );

    $info['filename'] = array(
      'callback' => array($this, 'getFilename'),
      'saveCallback' => array($this, 'updateFileLocation')
    );

    $info['type'] = array(
      'property' => 'type',
      'discovery' => array(
        'data' => array(
          'type' => 'string',
          'read_only' => TRUE,
        )
      )
    );

    $info['name'] = array(
      'property' => 'name',
    );

    $info['timestamp'] = array(
      'property' => 'timestamp',
    );

    $info['changed'] = array(
      'callback' => array($this, 'getChanged'),
    );

    $info['description'] = array(
      'property' => 'os_file_description',
      'sub_property' => 'value',
      'saveCallback' => array($this, 'setDescription')
    );

    $info['image_alt'] = array(
      'property' => 'field_file_image_alt_text',
      'sub_property' => 'value',
      'callback' => array($this, 'getImageAltText'),
      'saveCallback' => array($this, 'setImageAltText'),
    );

    $info['is_decorative'] = array(
      'property' => 'field_file_is_decorative',
      'sub_property' => 'value',
      'callback' => array($this, 'getIsDecorative'),
      'saveCallback' => array($this, 'setIsDecorative'),
    );

    $info['image_title'] = array(
      'property' => 'field_file_image_title_text',
      'sub_property' => 'value',
      'callback' => array($this, 'getImageTitleText'),
      'saveCallback' => array($this, 'setImageTitleText'),
    );

    $info['embed_code'] = array(
      'property' => 'field_html_code',
      'sub_property' => 'value',
      'callback' => array($this, 'getHtmlCode'),
      'saveCallback' => array($this, 'setHtmlCode'),
    );

    $info['preview'] = array(
      'callback' => array($this, 'getFilePreview'),
      'discovery' => array(
        'data' => array(
          'type' => 'string',
          'read_only' => TRUE,
        )
      )
    );

    $info['terms'] = array(
      'property' => OG_VOCAB_FIELD,
      'process_callbacks' => array(
        array($this, 'processTermsField'),
      ),
      'saveCallback' => array($this, 'setTerms'),
    );

    unset($info['label']['property']);

    return $info;
  }

  /**
   * Helper function for rendering a field.
   */
  private function getBundleProperty($wrapper, $field) {
    $properties = $wrapper->getPropertyInfo();

    if (isset($properties[$field])) {
      $property = $wrapper->get($field);
      return $property->value();
    }

    return null;
  }

  /**
   * Callback function to get the name of the file on disk
   * We need this to inform the user of what the new filename will be.
   */
  public function getFilename($wrapper) {
    $uri = $wrapper->value()->uri;
    return basename($uri);
  }

  /**
   * Display the id and the title of the group.
   */
  public function vsiteFieldDisplay($value) {
    return array('title' => $value[0]->title, 'id' => $value[0]->nid);
  }

  /**
   * Callback function to get the schema of the file.
   * We use this to prevent user from changing the filename
   */
  public function getSchema($wrapper) {
    $uri = str_replace('///', '//', $wrapper->value()->uri);  // band aid fix
    return parse_url($uri, PHP_URL_SCHEME);
  }

  /**
   * Callback function for the decorative bool of the image.
   */
  public function getIsDecorative($wrapper) {
    $is_decorative = $this->getBundleProperty($wrapper, 'field_file_is_decorative');
    return !empty($is_decorative);
  }

  /**
   * Callback function for the alt text of the image.
   */
  public function getImageAltText($wrapper) {
    return $this->getBundleProperty($wrapper, 'field_file_image_alt_text');
  }

  /**
   * Callback function for the title text.
   */
  public function getImageTitleText($wrapper) {
    return $this->getBundleProperty($wrapper, 'field_file_image_title_text');
  }

  /**
   * Callback function for the file preview.
   */
  public function getFilePreview($wrapper) {
    $output = media_get_thumbnail_preview($wrapper->value());
    return drupal_render($output);
  }

  /**
   * Callback for icon url
   */
  public function getIconUrl($wrapper) {
    $file = $wrapper->value();
    // Setting icon directory for svg files
    $icon_directory = variable_get('file_icon_directory', drupal_get_path('module', 'os_files') . '/icons');
    $icon_url = file_icon_url($file, $icon_directory);
    // Replacing png icons with svg
    $svg_url = str_replace('.png', '.svg', $icon_url);
    return $svg_url;
  }

  /**
   * Callback for embed codes
   */
  public function getHtmlCode($wrapper) {
    return $this->getBundleProperty($wrapper, 'field_html_code');
  }

  /**
   * Callback for file last changed timestamp
   */
  public function getChanged($wrapper) {
    $file = $wrapper->value();

    return $file->changed;
  }

  /**
   * Override checkEntityAccess()
   */
  public function checkEntityAccess($op, $entity_type, $entity) {
    return parent::checkEntityAccess($op, $entity_type, $entity) || $this->checkGroupAccess($op, $entity);
  }

  /**
   * Override checkPropertyAccess()
   *
  public function checkPropertyAccess($op, $public_field_name, EntityMetadataWrapper $property_wrapper, EntityMetadataWrapper $wrapper) {
    return $access = parent::checkPropertyAccess($op, $public_field_name, $property_wrapper, $wrapper);
    if (1 || $access) {
      return $access;
    }

    $file = $wrapper->value();
    return $this->checkGroupAccess($op, $file);
  }*/

  /**
   * Check for group access
   */
  public function checkGroupAccess($op, $file = null) {
    $account = $this->getAccount();

    $vsite = null;
    if (!empty($this->request['vsite'])) {
      $vsite = $this->request['vsite'];
    }
    elseif ($file == null) {
      return FALSE;
    }
    elseif ($file instanceof EntityDrupalWrapper) {
      $value = $file->{OG_AUDIENCE_FIELD}->value();
      $vsite = $value['target_id'];
    }
    elseif (is_object($file)) {
      $vsite = $file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
    }

    $permission = '';
    switch ($op) {
      case 'create':
        $permission = 'create files';
        break;
      case 'update':
      case 'edit':
        $permission = 'edit any files';
        break;
      case 'delete':
        $permission = 'delete any files';
        break;
    }

    if ($permission && $vsite) {
      return og_user_access('node', $vsite, $permission, $account);
    }

    return false;
  }

  /**
   * Filter files by vsite
   */
  protected function queryForListFilter(EntityFieldQuery $query) {
    $query->propertyCondition('status', 1);
    if (!empty($this->request['vsite'])) {
      if ($vsite = vsite_get_vsite($this->request['vsite'])) {
        $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $this->request['vsite']);
      }
      else {
        throw new RestfulBadRequestException(t('No vsite with the id @id', array('@id' => $this->request['vsite'])));
      }
    }
    elseif (module_exists('vsite')) {
      //$query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', 0, '<>');
    }
    // Make getting private files explicit
    // Private files currently require PIN authentication before they can even be access checked
    if (!isset($this->request['private'])) {
      $query->propertyCondition('uri', 'private://%', 'NOT LIKE');
    }
    // Only get private files. Nothing else.
    elseif ($this->request['private'] == 'only') {
      $query->propertyCondition('uri', 'private://%', 'LIKE');
    }

    // Add filter to get updated content only if parameter is set.
    if (!empty($this->request['changed'])) {
      $query->propertyCondition('changed', $this->request['changed'], '>=');
    }
    
    // Filter with multiple Vsite ids.
    if (!empty($this->request['vsiteid'])) {
      $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $this->request['vsiteid'], "IN");
    }
  }

  /**
   * Override. Handle the file upload process before creating an actual entity.
   * The file could be a straight replacement, and this is where we handle that.
   */
  public function createEntity() {
    $this->initSpace();
    if ($this->checkEntityAccess('create', 'file', NULL) === FALSE && $this->checkGroupAccess('create') === FALSE) {
      // User does not have access to create entity.
      $params = array('@resource' => $this->getPluginKey('label'));
      throw new RestfulForbiddenException(format_string('You do not have access to create a new @resource resource.', $params));
    }

    $destination = 'public://';
    // Public files are put inside of a files directory within the vsite folder
    // This keeps user uploaded files seperate from other vsite resources.
    $vsite_directory = '/files';

    // do spaces/private file stuff here
    if (isset($this->request['private'])) {
      $destination = 'private://';
      $vsite_directory = '';
    }

    if (isset($this->request['vsite'])) {
      $path = db_select('purl', 'p')->fields('p', array('value'))->condition('id', $this->request['vsite'])->execute()->fetchField();
      $destination .= $path . $vsite_directory;
    }

    $writable = file_prepare_directory($destination, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);

    if ($entity = file_save_upload('upload', $this->getValidators(), $destination, FILE_EXISTS_REPLACE)) {

      if (isset($this->request['vsite'])) {
        og_group('node', $this->request['vsite'], array('entity_type' => 'file', 'entity' => $entity));
        $entity = file_load($entity->fid);
      }

      if ($entity->status != FILE_STATUS_PERMANENT) {
        $entity->status = FILE_STATUS_PERMANENT;
        $entity = file_save($entity);
      }

      // Handle cropped photos
      $this->handleCrops($entity);

      $wrapper = entity_metadata_wrapper($this->entityType, $entity);

      return array($this->viewEntity($wrapper->getIdentifier()));
    }
    elseif (isset($_FILES['files']) && $_FILES['files']['errors']['upload']) {
      throw new RestfulUnprocessableEntityException('Error uploading new file to server.');
    }
    elseif ($errors = form_get_errors()) {
      throw new RestfulUnprocessableEntityException($errors['upload']);
    }
    elseif (isset($this->request['embed']) && module_exists('media_internet')) {

      $provider = media_internet_get_provider($this->request['embed']);
      $provider->validate();

      $validators = array();  // TODO: How do we populate this?
      $file = $provider->getFileObject();
      if ($validators) {
        $file = $provider->getFileObject();

        // Check for errors. @see media_add_upload_validate calls file_save_upload().
        // this code is ripped from file_save_upload because we just want the validation part.
        // Call the validation functions specified by this function's caller.
        $errors = array_merge($errors, file_validate($file, $validators));
      }

      if (!empty($errors)) {
        throw new MediaInternetValidationException(implode("\n", $errors));
      }
      else {
        // Providers decide if they need to save locally or somewhere else.
        // This method returns a file object
        $entity = $provider->save();

        if ($entity->status != FILE_STATUS_PERMANENT) {
          $entity->status = FILE_STATUS_PERMANENT;
          $entity = file_save($entity);
        }

        if ($this->request['vsite']) {
          og_group('node', $this->request['vsite'], array('entity_type' => 'file', 'entity' => $entity));
          $entity = file_load($entity->fid);
        }

        $wrapper = entity_metadata_wrapper($this->entityType, $entity);

        return array($this->viewEntity($wrapper->getIdentifier()));
      }
    }
    else {
      if (!$writable) {
        throw new RestfulServerConfigurationException('Unable to create directory for target file.');
      }
      else {
        // we failed for some other reason. What?
        throw new RestfulBadRequestException('Unable to process request.');
      }
    }
  }

  protected function getValidators() {
    $extensions = array();
    $types = file_type_get_enabled_types();
    foreach ($types as $t => $type) {
      $extensions = array_merge($extensions, _os_files_extensions_from_type($t));
    }

    $validators = array(
      'file_validate_extensions' => array(
        implode(' ', $extensions)
      ),
      'file_validate_size' => array(
        parse_size(file_upload_max_size())
      ),
      'os_files_upload_validate_image_dimensions' => array()
    );

    return $validators;
  }

  public function processTermsField($terms) {
    $return = array();

    foreach ($terms as $term) {
      $return[] = array(
        'id' => (int)$term->tid,
        'label' => $term->name,
        'vid' => $term->vid,
      );
    }

    return $return;
  }

  /**
   * Override. We need to handle files being replaced through this method.
   */
  public function putEntity($entity_id) {

    // this request is only a file
    // no other data is addeed
    if ($this->request['file']) {
      $oldFile = file_load($entity_id);
      $validators = $this->getValidators();
      preg_match('|\.([a-zA-Z0-3]*)$|', $oldFile->uri, $match);
      if ($match[1]) {
        unset($validators['file_validate_extensions']);
        $validators['file_validate_extension_from_mimetype'] = array($match[1]);
      }
      if ($errors = file_validate($this->request['file'], $validators)) {
        throw new RestfulUnprocessableEntityException(implode("\n", $errors));
      };

      $this->request['file']->filename = $oldFile->filename;
      if ($file = file_move($this->request['file'], $oldFile->uri, FILE_EXISTS_REPLACE)) {
        if ($oldFile->{OG_AUDIENCE_FIELD}) {
          og_group('node', $oldFile->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'], array('entity_type' => 'file', 'entity' => $file));
        }

        $this->handleCrops($file);

        return array($this->viewEntity($entity_id));
      }
      else {
        throw new RestfulBadRequestException('Error moving file. Please contact your server administrator.');
      }
    }

    return parent::putEntity($entity_id);
  }

  protected function handleCrops($entity) {
    if (module_exists('imagefield_crop') && $original = _imagefield_crop_file_to_crop($entity->fid)) {
      if ($original->fid != $entity->fid) {
        // this is a cropped image
        $fields = field_read_fields(array('type' => 'imagefield_crop'));
        foreach ($fields as $name => $info) {
          $q = db_select("field_data_$name", 'f')
            ->condition("{$name}_fid", $entity->fid)
            ->fields('f')
            ->execute();

          foreach ($q as $r) {
            $input = array(
              'cropbox_x' => $r->{$name.'_cropbox_x'},
              'cropbox_y' => $r->{$name.'_cropbox_y'},
              'cropbox_width' => $r->{$name.'_cropbox_width'},
              'cropbox_height' => $r->{$name.'_cropbox_height'}
            );
            file_copy($entity, $original->uri, FILE_EXISTS_REPLACE);
            // long drawn out process to get the $scale value for this crop
            $owner = entity_load($r->entity_type, array($r->entity_id));
            list(,,$bundle) = entity_extract_ids($r->entity_type, $owner[$r->entity_id]);
            $instance = field_info_instance($r->entity_type, $name, $bundle);
            $resolution = $instance['widget']['settings']['resolution'];
            list($scale['width'], $scale['height']) = explode('x', $resolution);
            // got everything we need. crop the image
            _imagefield_crop_resize(drupal_realpath($original->uri), $input, $scale, $entity);
            file_save($entity);
          }
        }
      }
    }
  }

  protected function setPropertyValues(EntityMetadataWrapper $wrapper, $null_missing_fields = FALSE) {
    $request = $this->getRequest();

    static::cleanRequest($request);
    $save = FALSE;
    $original_request = $request;

    if ($space = $wrapper->{OG_AUDIENCE_FIELD}->value()) {
      $vsite = vsite_get_vsite($space[0]->nid);
      $vsite->activate_user_roles();
    }

    foreach ($this->getPublicFields() as $public_field_name => $info) {
      if (empty($info['property']) && empty($info['saveCallback'])) {
        // We may have for example an entity with no label property, but with a
        // label callback. In that case the $info['property'] won't exist, so
        // we skip this field.
        continue;
      }

      if (isset($info['saveCallback'])) {
        $save = call_user_func($info['saveCallback'], $wrapper) || $save;

        if ($save) {
          unset($original_request[$public_field_name]);
        }
      }
      elseif ($info['property']) {
        $property_name = $info['property'];

        if (!isset($request[$public_field_name])) {
          // No property to set in the request.
          if ($null_missing_fields && $this->checkPropertyAccess('edit', $public_field_name, $wrapper->{$property_name}, $wrapper)) {
            // We need to set the value to NULL.
            $wrapper->{$property_name}->set(NULL);
          }
          continue;
        }

        if (!$this->checkPropertyAccess('edit', $public_field_name, $wrapper->{$property_name}, $wrapper)) {
          throw new RestfulBadRequestException(format_string('Property @name cannot be set.', array('@name' => $public_field_name)));
        }

        $field_value = $this->propertyValuesPreprocess($property_name, $request[$public_field_name], $public_field_name);

        $wrapper->{$property_name}->set($field_value);
        unset($original_request[$public_field_name]);
        $save = TRUE;
      }
    }


    if ($this->getErrors()) {
      $e = new RestfulBadRequestException("The following errors occured when attempting to save this file.\n".
        implode("\n", $this->getErrors()));
      throw $e;
    }
    if (!$save) {
      // No request was sent.
      throw new RestfulBadRequestException('No values were sent with the request');
    }

    if ($original_request) {
      // Request had illegal values.
      $error_message = format_plural(count($original_request), 'Property @names is invalid.', 'Property @names are invalid.', array('@names' => implode(', ', array_keys($original_request))));
      throw new RestfulBadRequestException($error_message);
    }

    // Allow changing the entity just before it's saved. For example, setting
    // the author of the node entity.
    $this->entityPreSave($wrapper);

    $this->entityValidate($wrapper);

    $wrapper->save();
  }


  public function propertyValuesPreprocess($property_name, $value, $public_field_name) {
    $request = $this->getRequest();
    self::cleanRequest($request);

    if ($public_field_name == 'terms') {
      $new_value = array();
      foreach ($request['terms'] as $term) {
        $new_value[] = is_array($term) ? $term['tid'] : $term;
      }

      return $new_value;
    }

    return parent::propertyValuesPreprocess($property_name, $value, $public_field_name);
  }

  /**
   * Add an error to be returned to the client.
   *
   * @param $field - the field that errored
   * @param $error - the string message describing the error
   */
  public function addError($field, $error) {
    $this->errors[] = "$field: $error";
  }

  /**
   * Get the list of errors displayed so far
   * @return array
   */
  public function getErrors() {
    return $this->errors;
  }

  protected function updateFileLocation($wrapper) {
    if ($this->request['filename']) {
      $file = file_load($wrapper->getIdentifier());
      $label = $wrapper->name->value();
      $destination = drupal_dirname($file->uri) . '/' . $this->request['filename'];

      if ($file = file_move($file, $destination)) {
        $wrapper->set($file);
        $wrapper->name->set($label);
        return true;
      }
    }
    return false;
  }

  protected function setDescription($wrapper) {
    if (isset($this->request['description'])) {
      $data = array(
        'value' => $this->request['description'],
        'format' => 'filtered_html'
      );
      $wrapper->os_file_description->set($data);

      return true;
    }
    return false;
  }

  protected function setIsDecorative($wrapper) {
    if (isset($this->request['is_decorative'])) {
      $wrapper->field_file_is_decorative->set($this->request['is_decorative']);

      return true;
    }
    return false;
  }

  protected function setImageAltText($wrapper) {
    if (isset($this->request['image_alt'])) {
      $wrapper->field_file_image_alt_text->set($this->request['image_alt']);

      return true;
    }
    return false;
  }

  protected function setImageTitleText($wrapper) {
    if (isset($this->request['image_title'])) {
      $wrapper->field_file_image_title_text->set($this->request['image_title']);

      return true;
    }
    return false;
  }

  protected function setHtmlCode($wrapper) {
    if (isset($this->request['embed_code'])) {
      if (media_embed_check_src($this->request['embed_code'])) {
        $wrapper->field_html_code->set($this->request['embed_code']);

        return true;
      }
      if ($GLOBALS['is_https']) {
        $this->addError('embed_code', 'This embed code failed validation. Please check that all urls are with https and from accepted domains');
      }else {
        $this->addError('embed_code', 'This embed code failed validation. Please check that all urls are from accepted domains');
      }
    }
    return false;
  }

  protected function setTerms($wrapper) {
    if (isset($this->request['terms'])) {
      $values = $this->request['terms'];
      $terms = array();
      foreach ($values as $term) {
        $terms[] = $term['id'];
      }

      $wrapper->{OG_VOCAB_FIELD}->set($terms);

      return true;
    }
    return false;
  }

  protected function getLastModified($id) {
    $q = db_select('file_managed', 'fm')
      ->fields('fm', array('changed'))
      ->condition('fid', $id)
      ->execute();

    foreach ($q as $r) {
      return $r->changed;
    }

    return FALSE;
  }

  protected function checkFilename($filename) {
    list (, $filename) = explode('/', $filename);
    $dir = 'public://';
    if (isset($this->request['private'])) {
      $dir = 'private://';
    }

    if (isset($this->request['vsite'])) {
      $vsite = db_select('purl', 'p')
        ->fields('p', array('value'))
        ->condition('provider', 'spaces_og')
        ->condition('id', $this->request['vsite'])
        ->execute()
        ->fetchField();

      if ($vsite) {
        $dir .= $vsite . '/files/';
      }
    }

    $new_filename = strtolower($filename);
    $new_filename = preg_replace('|[^a-z0-9\-_\.]|', '_', $new_filename);
    $new_filename = preg_replace(':__:', '_', $new_filename);
    $new_filename = preg_replace('|_\.|', '.', $new_filename);

    $fullname = $dir . $new_filename;
    $counter = 0;
    $collision = false;
    while (file_exists($fullname)) {
      $collision = true;
      $pos = strrpos($new_filename, '.');
      if ($pos !== FALSE) {
        $name = substr($new_filename, 0, $pos);
        $ext = substr($new_filename, $pos);
      } else {
        $name = $basename;
        $ext = '';
      }

      $fullname = sprintf("%s%s_%02d%s", $dir, $name, ++$counter, $ext);
    }

    return array(
      'collision' => $collision,
      'invalidChars' => basename($new_filename) != $filename,
      'expectedFileName' => basename($fullname)
    );
  }

  /**
   * Return the URL of the image style of a given file.
   */
  public function getImageStyle() {
    $path = explode("/", $this->getPath());
    $style_name = $path[2];

    if (!in_array($style_name, array_keys(image_styles()))) {
      throw new RestfulBadRequestException(format_string('There is no image style with the name @name', array('@name' => $style_name)));
    }

    $fid = $path[0];

    if (!$file = file_load($fid)) {
      throw new RestfulBadRequestException(format_string('There is no file with the id @id', array('@id' => $fid)));
    }

    if ($file->type != 'image') {
      throw new RestfulBadRequestException(format_string('The file @name is not an image.', array('@name' => $file->filename)));
    }

    return array(
      'url' => image_style_url($style_name, $file->uri),
    );
  }
}

/**
 * Replaces the core file_validate_extensions function when the file in question
 * has a temporary extension.
 */
function file_validate_extension_from_mimetype(stdClass $file, $extensions) {
  include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
  $maps = file_mimetype_mapping();
  $ext_arr = explode(' ', $extensions);
  $index = array_search($file->filemime, $maps['mimetypes']);
  $exts = array_keys($maps['extensions'], $index);
  $passes = array_intersect($ext_arr, $exts);

  $errors = array();
  if (!count($passes)) {
    $errors[] = t('Only files with the following extensions are allowed: %files-allowed.', array('%files-allowed' => $extensions));
  }

  return $errors;
}
