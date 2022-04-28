<?php

class SoftwareReleaseNodeRestful extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['software_project'] = array(
      'property' => 'field_software_project',
      'process_callbacks' => array(
        array($this, 'softwareProjectPreprocess'),
      ),
    );

    $public_fields['recommended'] = array(
      'property' => 'field_software_recommended',
      'process_callbacks' => array(
        array($this, 'softwareProjectRecommended'),
      ),
    );

    $public_fields['version'] = array(
      'property' => 'field_software_version',
    );

    $public_fields['package'] = array(
      'property' => 'field_software_package',
      'process_callbacks' => array(
        array($this, 'singleFileFieldDisplay'),
      ),
    );

    return $public_fields;
  }

  /**
   * Return the project name and id.
   */
  public function softwareProjectPreprocess($value) {
    return array(
      'id' => $value->nid,
      'label' => $value->title,
    );
  }

  public function softwareProjectRecommended($value) {
    return $value ? t('Recommended') : t('Not Recommended');
  }

}
