<?php
/**
 * @file
 * contains OsImporterEntityValidateBase
 */

/**
 * Extending the entity validator class for plugins that just need to validate
 * the title and nothing more.
 */
class OsImporterEntityValidateBase extends EntityValidateBase {

  /**
   * @var bool
   * Determine if the validation invoked during a rest call. This is useful when
   * we have different validation for rest call and other validation when
   * importing CSV.
   */
  public $rest = TRUE;

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    // Add all fields.
    foreach (field_info_instances($this->entityType, $this->bundle) as $field_name => $info) {
      if ($info['required']) {
        // Check if the 'isNotEmpty' validator is already assigned to the field
        // in the base class. Don't re-add it to avoid duplicated messages.
        if (!in_array('isNotEmpty', $fields[$field_name]['validators'])) {
          $fields[$field_name]['validators'][] = array($this, 'isNotEmpty');
        }
      }

      $field = field_info_field($field_name);
      if ($field['type'] == 'text_with_summary') {
        $fields[$field_name]['validators'][] = array($this, 'validateTextAreaLength');
      }
    }

    // Remove all validators for entityreference fields.
    foreach ($fields as $field_name => $info) {
      $field_info = field_info_field($field_name);
      $type = $field_info['type'];
      if ($type == 'entityreference') {
        unset($fields[$field_name]);
      }
    }

    // Remove the required field which not being imported via the importer or
    // taken care by the os_importer_entity_property_info_alter().
    $field_names = array(
      'field_news_date',
      'field_url',
      'field_links_link',
      'media_gallery_format',
      'media_gallery_image_info_where',
      'media_gallery_block_columns',
      'media_gallery_block_rows',
    );

    foreach ($field_names as $field) {
      unset($fields[$field]);
    }

    // Add path validator.
    $fields['path_alias']['validators'][] = array($this, 'validatorPath');

    if ($this->rest) {
      $fields[OG_AUDIENCE_FIELD] = array(
        'required' => TRUE,
      );
    }

    return $fields;
  }

  public function validateTextAreaLength($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    $length = variable_get('os_wysiwyg_maximum_length_' . $field_name, 5 * 1000000);

    if (strlen($value['value']) > $length) {
      $this->setError($field_name, "The value of the '$field_name' field can not be longer than '$length' characters.");
    }
  }

  /**
   * Verify the start is occurring before the end date.
   */
  public function validateOsDate($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    // Empty values are checked for in 'isNotEmpty'.
    if (empty($value) || $this->rest) {
      return;
    }

    $value = reset($value);
    // Validate the date format for the start and end date.
    $this->validateDateFormats(array('M j Y', 'M d Y'), $value, $field_name);
  }

  /**
   * Verify the value is in a given format.
   *
   * @param $formats
   *   Array of formats.
   * @param $value
   *   The value of the field.
   * @param $field_name
   *   The field name.
   */
  protected function validateDateFormats($formats, $value, $field_name) {
    // Validate the date format for the start and end date.
    $validationSuccess = false;
    $formatLabels = array();
    foreach ($formats as $format) {
      if ($date = DateTime::createFromFormat($format, $value)) {
        if ($date->format($format) == $value) {
          $validationSuccess = true;
        }
      }
      $formatLabels[] = format_date(REQUEST_TIME, 'custom', $format);
    }

    if (! $validationSuccess) {
      $params = array(
          '@date' => $value,
          '@format' => join(" or ", $formatLabels),
      );
      $this->setError($field_name, 'The value of the date field (@date) is not valid. The date should be in a format similar to one of the following formats: @format', $params);
    }

    return TRUE;
  }

  /**
   * Validating the url field is not empty. We can't use the isNotEmpty since
   * the field is array with keys.
   */
  public function validatorUrlNotEmpty($field_name, $value) {
    $value = reset($value);
    if (empty($value)) {
      $this->setError($field_name, t('The field URL cannot be empty.'));
    }
  }

  /**
   * Validating the image minimum size.
   */
  public function validatorPhoto($field_name, $value, $min_width, $min_height) {
    if (empty($value)) {
      return;

    }

    list($width, $height) = getimagesize($value);

    if ($width < $min_width || $height < $min_height) {
      $params = array(
        '@width' => $width,
        '@height' => $height,
        '@min_width' => $min_width,
        '@min_height' => $min_height,
      );
      $this->setError($field_name, 'The size of the image needs to be at least @min_widthX@min_height. The given image is @widthX@height', $params);
    }
  }

  /**
   * Validating the path alias.
   */
  public function validatorPath($field_name, $value) {
    if (empty($value)) {
      return;
    }
    if (!module_exists('pathauto')) {
      $this->setError($field_name, t('Please enable pathauto module, to validate the path.'));
    }
    module_load_include('inc', 'pathauto');

    $vsite = vsite_get_vsite();
    $purl = $vsite->group->purl;
    $alias_without_purl = pathauto_cleanstring($value[0]);
    $alias_with_purl = $purl . '/' . $alias_without_purl;

    // Ensure that the alias does not exist yet.
    $query = db_select('url_alias')
      ->fields('url_alias')
      ->condition('alias', $alias_with_purl)
      ->condition('language', LANGUAGE_NONE)
      ->range(0, 1);

    if ($query->execute()->fetchField()) {
      // Alias already in use.
      $params = array(
        '@path' => $alias_without_purl,
      );
      $this->setError($field_name, t('The path "@path" is already in use. Please supply a different path.', $params));
    }

    // Check if the given alias is not taken by something more global.
    // @see _vsite_form_alias_validate().
    $items = array(
      'alias_with_purl' => menu_get_item($alias_with_purl),
      'alias_without_purl' => menu_get_item($alias_without_purl),
    );
    if ($items['alias_with_purl']['href'] == $alias_with_purl || $items['alias_without_purl']['href'] == $alias_without_purl) {
      $params = array(
        '@path' => $alias_without_purl,
      );
      $this->setError($field_name, t('The path "@path" is already in use. Please supply a different path.', $params));
    }
  }

  /**
   * Display the id and the title of the group.
   */
  public function vsiteFieldDisplay($value) {
    return array('title' => $value[0]->title, 'id' => $value[0]->nid);
  }
}
