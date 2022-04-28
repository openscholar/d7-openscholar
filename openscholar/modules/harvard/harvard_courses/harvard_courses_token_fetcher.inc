<?php
/**
 * @file
 *
 * Provides the Class for Feeds Token Fetcher.
 */
/**
 * Base class for the token fetcher.
 */
class HarvardFetcher extends FeedsFetcher {
  var $baseUrl = '';
  var $data = '';
  var $url_param = '';

  public function fetch(FeedsSource $source) {
    $wrapper = entity_metadata_wrapper('node', $source->feed_nid);
    $import_type = $wrapper->field_import_type->value();
    $nids = haravard_courses_sites_nid_by_import_type($import_type);

    if (empty($nids)) {
      drupal_set_message(t('There were no sites for this import.'));
      return new FeedsFetcherResult('<courses></courses>');
    }

    // Define the query parameter and the field that holds the query values.
    if ($import_type == 'cat_num') {
      $field = 'field_catalog_number';
      $this->url_param = 'q=';
    }
    else {
      $field = 'field_department_school';
      $this->url_param = array();
    }

    // Loop over the nodes and build the query parameter value.
    $schools = array();
    $departments = array();
    $import_value = array();

    foreach ($nids as $nid) {
      $import_wrapper = entity_metadata_wrapper('node', $nid);

      if ($field == 'field_department_school') {
        // Get all the schools and all the departments from the field
        // collection.
        $field_collections = $import_wrapper->{$field}->value();
        for ($i = 0; $i <= count($field_collections); $i++) {
          if (!$import_wrapper->{$field}->get($i)->value()) {
            continue;
          }

          if ($import_wrapper->{$field}->get($i)->field_school_name->label()) {
            $schools[] = $import_wrapper->{$field}->get($i)->field_school_name->label();
          }
          if ($import_wrapper->{$field}->get($i)->field_department_id->value()) {
            $departments[] = $import_wrapper->{$field}->get($i)->field_department_id->value();
          }
        }
      }
      else {
        foreach ($import_wrapper->{$field}->value() as $import_values) {
          $import_value[] = $import_values;
        }
      }
    }

    // Build the URL for import and verify for response code 200 from the URL.
    $this->baseUrl = $wrapper->field_base_url->value();
    $rows = 10;
    $start = 0;
    if ($import_type == 'cat_num') {
      $url = $this->buildUrl($import_value, $rows, $start);
    }
    else {
      $this->url_param = array(
        'fq_dept_area_category=dept_area_category:"' . implode('" OR "', $departments) . '"',
        'school_nm=school_nm:"' . implode('" OR "', $schools) . '"',
      );
      $url = $this->buildUrl(array(), $rows, $start);
    }

    $result = drupal_http_request($url);

    preg_match_all('|<courses*.numFound="(.*)">(.*)<\/courses>|U', $result->data, $matches, PREG_PATTERN_ORDER);

    if ($result->code != 200) {
      // oops... error!
      $params = array(
        '@url' => $this->buildUrl($import_value, $rows, $start),
        '@error' => $result->error,
        '@request' => $result->request,
      );
      $text = t('Unable to get feeds from: @url due to @error. The request information is @request', $params);
      drupal_set_message($text);
      watchdog('harvard_courses', $text);
      return new FeedsFetcherResult('<courses></courses>');
    }

    $found = $matches[1][0];
    if ($found < $rows) {
      return new FeedsFetcherResult($result->data);
    }
    else {
      // Number of batches we need to iterate.
      $batches = floor(intval($found) / 10) + 1;
      $courses = '<courses>';
      for ($i = 0; $i <= $batches; $i++) {
        $start = $i * $rows;
        $result = drupal_http_request($this->buildUrl($import_value, $rows, $start));
        if (!property_exists($result, 'data')) {
          continue;
        }
        preg_match_all('|<courses*.numFound="(.*)">(.*)<\/courses>|U', $result->data, $matches, PREG_PATTERN_ORDER);
        if (!isset($matches[2][0])) {
          continue;
        }
        $courses .= $matches[2][0];
      }
      $courses .= '</courses>';
      return new FeedsFetcherResult($courses);
    }
  }

  /**
   * Building the URL for the XML file.
   */
  private function buildUrl($values, $rows, $start) {
    if ($this->config['debug_mode']) {
      if ($start != 0) {
        return;
      }
      return variable_get('purl_base_domain') . '/' . drupal_get_path('module', 'harvard_courses') . '/dummy_courses.xml';
    }
    else {
      if (is_array($this->url_param)) {
        $param = implode('&', $this->url_param);
      }
      else {
        $param = $this->url_param . '"' . implode('" OR "', $values);
      }
      return $this->baseUrl . '?' . urlencode( $param . '&rows=' . $rows . '&start=' . $start);
    }
  }

  public function configDefaults() {
    return array('debug_mode' => '');
  }

  public function sourceDefaults() {

  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['debug_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug mode'),
      '#description' => t('When this is checked, the importer will import courses from a dummy file.'),
      '#default_value' => $this->config['debug_mode'],
    );
    return $form;
  }
}
