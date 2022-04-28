<?php

/**
 * @file
 * Contains OS Importer MenuProcessor.
 */

/**
 * Creates menu items from feed items.
 */
class OsImporterMenuProcessor extends FeedsProcessor {

  /**
   * {@inheritdoc}
   */
  public function entityType() {
    return 'menu';
  }

  /**
   * {@inheritdoc}
   */
  public function bundleOptions() {
    return menu_get_menus();
  }

  /**
   * {@inheritdoc}
   */
  protected function entityInfo() {
    return array(
      'label' => t('Menu link'),
      'label plural' => t('Menu links'),
      'bundle name' => t('Menu'),
      'entity keys' => array('bundle' => 'menu_name'),
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function newEntity(FeedsSource $source) {
    // Use object to have reference by default.
    return (object) array(
      'link_path' => '',
      'link_title' => '',
      'menu_name' => '',
      'language' => LANGUAGE_NONE,
      'expanded' => TRUE,
      'weight' => 0,
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function entityLoad(FeedsSource $source, $entity_id) {
    return (object) menu_link_load($entity_id);
  }

  /**
   * {@inheritdoc}
   */
  protected function entityValidate($menu_link) {
    $this->cleanPath($menu_link->link_path);
    if (!$this->config['skip_link_path_validation']) {
      if (!drupal_valid_path($menu_link->link_path)) {
        throw new FeedsValidationException(t('Invalid link path.'));
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function entitySave($menu_link) {
    $item = (array) $menu_link;
    if (isset($item['og_group_ref'])) {
      if (!$item['menu_name'] || $item['menu_name'] == 'false') {
        vsite_menu_delete_menu_link ('primary-menu', $item['mlid']);
      }
      else {
        vsite_menu_menu_link_save ($item, $item['og_group_ref'][LANGUAGE_NONE][0]['target_id']);
      }
    }
    else {
      menu_link_save($item);
    }
    $menu_link = (object) $item;
  }

  /**
   * {@inheritdoc}
   */
  protected function entityDeleteMultiple($mlids) {
    foreach ($mlids as $mlid) {
      menu_link_delete($mlid);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function configDefaults() {
    $defaults = parent::configDefaults();
    $defaults['skip_link_path_validation'] = TRUE;
    $defaults['update_existing'] = FEEDS_UPDATE_EXISTING;  // needed so global links get into spaces. Inheritance doesn't happen
    return $defaults;
  }

  /**
   * {@inheritdoc}
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    unset($form['input_format']);
    $form['skip_link_path_validation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip link path validation'),
      '#default_value' => $this->config['skip_link_path_validation'],
      '#description' => t('Do not run link path validation for imported menu item.'),
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function setTargetElement(FeedsSource $source, $target_item, $target_element, $value) {
    switch ($target_element) {
      case 'parent_path':
        if (!empty($value)) {
          $this->cleanPath($value);
          $parent_item = os_menu_load_link_path($value);
          if ($parent_item) {
            $target_item->plid = $parent_item['mlid'];
          }
        }
        break;

      case 'description':
        $target_item->options['attributes']['title'] = $value;
        break;

      case 'weight':
        $target_item->weight = (int) $value;
        break;

      default:
        parent::setTargetElement($source, $target_item, $target_element, $value);
        break;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();

    $targets += array(
      'menu_name' => array(
        'name' => t('Menu name'),
        'description' => t('Menu name'),
      ),
      'link_path' => array(
        'name' => t('Link path'),
        'description' => t('The path of link.'),
      ),
      'link_title' => array(
        'name' => t('Link title'),
        'description' => t('The title of link.'),
      ),
      'weight' => array(
        'name' => t('Weight'),
        'description' => t('The weight of link.'),
      ),
      'description' => array(
        'name' => t('Description'),
        'description' => t('Description of link.'),
      ),
      'parent_path' => array(
        'name' => t('Parent link path'),
        'description' => t('Parent menu link path.'),
      ),
    );

    if (module_exists('i18n_menu')) {
      $targets['language'] = array(
        'name' => t('Menu link language'),
        'description' => t('Menu link language.'),
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    drupal_alter('feeds_processor_targets', $targets);

    return $targets;
  }

  /**
   * {@inheritdoc}
   */
  public function clear(FeedsSource $source) {

    $state = $source->state(FEEDS_PROCESS_CLEAR);
    $info = $this->entityInfo();

    // Build base select statement.
    $select = db_select('feeds_item', 'fi')
      ->condition('fi.id', $this->id)
      ->fields('fi', ['entity_id']);

    // If there is no total, query it.
    if (!$state->total) {
      $state->total = $select->countQuery()
        ->execute()
        ->fetchField();
    }

    // Delete a batch of entities.
    $ids = $select->range(0, $this->getLimit())->execute()->fetchCol();
    $this->entityDeleteMultiple($ids);

    // Report progress, take into account that we may not have deleted as
    // many items as we have counted at first.
    if (count($ids)) {
      $state->deleted += count($ids);
      $state->progress($state->total, $state->deleted);
    }
    else {
      $state->progress($state->total, $state->total);
    }

    // Report results when done.
    if ($source->progressClearing() == FEEDS_BATCH_COMPLETE) {
      if ($state->deleted) {
        $message = format_plural(
          $state->deleted,
          'Deleted @number @entity',
          'Deleted @number @entities',
          array(
            '@number' => $state->deleted,
            '@entity' => strtolower($info['label']),
            '@entities' => strtolower($info['label plural']),
          )
        );
        $source->log('clear', $message, [], WATCHDOG_INFO);
        drupal_set_message($message);
      }
      else {
        drupal_set_message(t('There are no @entities to be deleted.', ['@entities' => $info['label plural']]));
      }
    }
  }

  /**
   * Trim and normalize path.
   */
  protected function cleanPath(&$path) {
    $path = trim($path, " \t\n\r\0\x0B/");
    // first check that its not <front>
    // ignore it if it is
    if ($path == '<front>') {
      return;
    }

    // then we check if its an app path
    // if it is, we don't have to do anything
    foreach (os_app_info() as $app_name => $info) {
      if (!empty($info['path']) && $info['path'] == $path) {
        return;
      }
    }
    $purl = vsite_get_purl();
    $prefix = $purl ? $purl . '/' : '';
    $normal_path = drupal_get_normal_path($prefix . $path);
    if ($path != $normal_path) {
      $path = $normal_path;
    }
  }

  /**
   * @inheritdoc
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    $item = $result->currentItem();
    if ($item['path'] != '<front>' && $link = menu_link_get_preferred($item['path'])) {
      return $link['mlid'];
    }

    return 0;
  }
}
