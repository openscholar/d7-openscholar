<?php

/**
 * @file
 * Class for repec plugin
 */

$plugin = array(
  'class' => 'citation_distribute_service_repec',
  'file' => 'citation_distribute_service_repec.inc',
  'parent' => 'citation_distribute_service',
  'title' => t('RePEc'),
  'href' => 'http://repec.org',
  'name' => 'repec',
  'description' => 'Research Papers in Economics',
);

class citation_distribute_service_repec extends citation_distribute_service {

  public $title , $inst_name , $archive_code , $maintainer_name , $maintainer_email , $archive_path , $archive_url , $provider_institution , $provider_homepage;

  /**
   * Constructor
   * member variables are in flux for a little while until a control panel is created.
   * for now, we can still refer to them with $this, but they are coming from system vars.
   */
  public function __construct() {
    parent::__construct();

    $this->title = variable_get('citation_distribute_service_repec_title', 'RePEc');
    $this->inst_name = variable_get('citation_distribute_service_repec_inst_name');
    $this->archive_code = variable_get('citation_distribute_service_repec_archive_code');
    $this->maintainer_name = variable_get('citation_distribute_service_repec_maintainer_name');
    $this->maintainer_email = variable_get('citation_distribute_service_repec_maintainer_email');
    $this->archive_path = variable_get('citation_distribute_service_repec_archive_path');
    $this->archive_url = variable_get('citation_distribute_service_repec_archive_url');
    $this->provider_institution = variable_get('citation_distribute_service_repec_provider_institution');
    $this->provider_homepage = variable_get('citation_distribute_service_repec_provider_homepage');
  }

  /**
   * Distributes a node to chosen service(s)
   */
  public function save($nid, $plugin){
    //doublecheck the validation for this nid.
    if (!$this->validate((int)$nid)) {
      return false;
    }

    //rdf file for this node
    $metadata = $this->map_metadata($nid);
    $out = $this->render($metadata);
    $rdf_file = $this->_write($out, $nid);

    //index listing all nodes
    $index_file = $this->_index();

    //were index and rdf both successful
    return ($rdf_file && $index_file);
  }

  /**
   * Copies data from biblio and node data into array labeled for this service
   */
  public function map_metadata($nid){
    $node = node_load($nid);
    $type = $this->_nid_to_type($nid);
    $out = array();
    $out['Template-Type'] = $this->_get_template_type($this->_map_biblio_type($node->biblio_type_name));
    $out['Handle'] = 'RePEc:' . $this->archive_code . ':' . $type . ':' .  $nid;

    //get node's URL
    $path = 'node/'. $nid;  //using $node->nid notation here because pathauto changes URL when title changes
    if (module_exists('vsite') && (count($node->og_group_ref[LANGUAGE_NONE]) == 1)){
      $vsite = spaces_load('og', $node->og_group_ref[LANGUAGE_NONE][0]['target_id']);
      $file_url = $vsite->get_absolute_url() . $path;
    }
    elseif(module_exists('vsite') && (count($node->og_group_ref[LANGUAGE_NONE]) > 1)){
      //this should not happen
      error_log("Alert: citation_distribute_service_repec has detected that node {$node->nid} belongs to multiple groups");
      $vsite = vsite_get_vsite();
      $file_url = ( $vsite ) ? $vsite->get_absolute_url() . $path : '';
    }
    else{
      $file_url = variable_get('purl_base_domain') . '/' . $path;
    }
    $out['File-URL'] = $file_url;

    // recursive wrapper for utf8_decode
    $decoder = function($val) use (&$decoder) {
      if (is_string($val)) {
        return utf8_decode($val);
      }
      elseif (is_array($val)) {
        foreach ($val as &$v) {
          $v = $decoder($v);
        }
        return $val;
      }
      else {
        return $val;
      }
    };

    // add metadata to template array
    foreach ($this->_get_type_metadata($type) as $biblio => $repec) {
      if (isset($node->{$biblio})) {
        if (is_string($node->{$biblio}) && strlen($node->{$biblio})) {
          //scalar values
          $out[$repec] = $node->{$biblio};

        } else {
          //arrays are special cases because each of them has various levels of nesting and the data we're trying to extract is uniquely named
          switch($biblio) {

            case 'biblio_contributors':
              $out[$biblio] = array();
              foreach($node->{$biblio} as $contributor) {
                $out[$repec][] = $contributor['name'];
              }
            break;

            case 'biblio_keywords':
              $out[$repec] = $node->{$biblio};
            break;

          }
        }
      } else {
        //$node->{$biblio} is not set.  this must be another exception
        switch ($biblio) {
          //manipulate a string value before including it:
          case '{biblio_date}':
            if ($node->biblio_date) {
              $date = $node->biblio_date;
            } elseif ($node->biblio_year < 2200 && $node->biblio_year > 1900) {
              $date = $node->biblio_year;
            }

            if ($date) {
              //strtotime interprets '1957' as an hour not a year.  prepend 4 digit dates with a month and day.  bib_date is sometimes YYYY too, so we're doing this everywhere.
              $date = preg_replace( '/^(\d\d\d\d)$/', '01-01-$1', $date);
              $time = strtotime($date);
            }

            if (!isset($time) || !$time) {
              // Created is used in the case when it is a working paper, or non year.
              $time = $node->created;
            }

            $out[$repec] = date('Y-m', $time); //yyyy-mm
          break;
        }
      }
      // if we actually managed to set something, decode it
      // repec using single-byte encoding and we're currently using double-byte
      if (isset($out[$repec])) {
        $out[$repec] = $decoder($out[$repec]);
      }
    }

    return $out;

  }

  /*
   * themes data into format appropriate for this service
   */
  public function render($metadata){
    //Copy from array to string of 'key: value' format
    $out = '';
    foreach ($metadata as $key => $value) {
      if (is_string($value)) {
        $out .= $key . ': ' . trim(strip_tags($value)) . "\n";
      } elseif (is_array($value)) {
        foreach ($value as $subvalue) {
          $out .= $key . ': ' . trim(strip_tags($subvalue)) . "\n";
        }
      }
    }

    //Publications published by someone else need the publication-status field
    foreach (array('Publisher', 'Journal', 'Provider-Name') as $external_pub) {
      if (isset($metadata[$external_pub])) {
        $out .=  'Publication-Status: Published in ' . $metadata[$external_pub];
        break;
      }
    }

    return $out;
  }

  /* Given a biblio type, return a repec label for it */
  public function _map_biblio_type($biblio_type_name) {
    $types = array(
      'booktm' => array('Book'),
      'bookch' => array('Book Chapter'),
      'journl' => array('Journal Article','Newspaper Article','Magazine Article', 'Web Article', 'Film','Broadcast','Artwork','Manuscript',),
      'wpaper' => array('Conference Paper', 'Conference Proceedings','Thesis','Report','Hearing','Case','Bill','Statute', 'Patent','Personal','Map','Chart','Unpublished','Database','Government Report','Classical','Legal Ruling','Miscellaneous','Miscellaneous Section','Working Paper',),
      'sftwre' => array('Software','Audiovisual',),
    );

    foreach ($types as $name => $arr) {
      if (in_array($biblio_type_name, $arr)) {
        return $name;
      }
    }

    return false;
  }

  /* retrieves biblio -> repec metadata title mappings given a biblio type */
  public function _get_type_metadata($type) {
    $map = array(
      'booktm' => array(
        'biblio_contributors' => 'Editor-Name',
        'title' => 'Title',
        'biblio_publisher' => 'Provider-Name',
        'biblio_abst_e' => 'Abstract',
        'biblio_keywords' => 'Keywords',
        'biblio_volume' => 'Volume',
        'biblio_edition' => 'Edition',
        'biblio_year' => 'Year',
      ),
      'bookch' => array(
        'biblio_contributors' => 'Author-Name',
        'title' => 'Title',
        'biblio_abst_e' => 'Abstract',
        'biblio_keywords' => 'Keywords',
        'biblio_pages' => 'Pages',
        'biblio_volume' => 'Volume',
        'biblio_section' => 'Chapter',
        'biblio_edition' => 'Edition',
        'biblio_year' => 'Year',
        'biblio_publisher' => 'Publisher',
      ),
      'journl' => array(
        'biblio_contributors' => 'Author-Name',
        'title' => 'Title',
        'biblio_abst_e' => 'Abstract',
        'biblio_keywords' => 'Keywords',
        'biblio_secondary_title' => 'Journal',
        'biblio_pages' => 'Pages',
        'biblio_volume' => 'Volume',
        'biblio_issue' => 'Issue',
        'biblio_year' => 'Year',
      ),
      'sftwre' => array(
        'biblio_contributors' => 'Author-Name',
        'title' => 'Title',
        'biblio_tertiary_title' => 'Series',
        'biblio_pages' => 'Length',
        'biblio_publisher' => 'Publisher',
      ),
      'wpaper' => array(
        'biblio_contributors' => 'Author-Name',  //mandatory
        'title' => 'Title',                      //mandatory
        'biblio_abst_e' => 'Abstract',
        'biblio_keywords' => 'Keywords',
        'biblio_pages' => 'Length',
        '{biblio_date}' => 'Creation-Date',  //format the date before including it.
        'biblio_publisher' => 'Publisher',
      ),
    );

    if (isset($map[$type])) {
      return $map[$type];
    } else {
      return false;
    }
  }

  /* gets Template-Type for this repec type */
  public function _get_template_type($citation_distribute_service_repec_type) {
    $types = array(
      'wpaper' => 'ReDIF-Paper 1.0',
      'sftwre' => 'ReDIF-Software 1.0',
      'journl' => 'ReDIF-Article 1.0',
      'bookch' => 'ReDIF-Chapter 1.0',
      'booktm' => 'ReDIF-Book 1.0' ,
    );

    if (isset($types[$citation_distribute_service_repec_type])) {
      return $types[$citation_distribute_service_repec_type];
    } else {
      return false;
    }
  }

  /**
   *  Validate.
   *
   *  repec only takes working papers from us, so don't send any
   *  repec requires authors
   *
   * validate has two modes. it can check nodeapi's a3 form argument. or it
   * can check a nid.
   *
   * @return boolean
   *   success bool.  only relevant for nid mode
   **/
  public function validate($a3) {
    $return = TRUE;

    // Get the vars we're checking by any means.
    if (is_array($a3)) {
      $tid = $a3['values']['biblio_type'];
      $contributors = $a3['values']['biblio_contributors'];
    }
    elseif (is_int($a3)) {
      // Not really $a3 anymore, just a nid.
      if ($node = node_load($a3)) {
        $tid = $node-> biblio_type;
        $contributors = $node->biblio_contributors;
      }
      else {
        return FALSE;
      }
    }

    // Ensure we're submitting a working paper.
    // $type = db_result(db_query('SELECT name FROM {biblio_types} WHERE tid = %d ', $tid));
    $type = db_select('biblio_types', 'bt')->condition('tid', $tid)->fields('bt', array('name'))->execute()->fetchColumn();
    if ($this->_map_biblio_type($type) != 'wpaper') {
      $wpaper = array('Conference Paper', 'Conference Proceedings','Thesis','Report','Hearing','Case','Bill','Statute', 'Patent','Personal','Map','Chart','Unpublished','Database','Government Report','Classical','Legal Ruling','Miscellaneous','Miscellaneous Section','Working Paper',);
      form_set_error('biblio_type', t("The @type Publication type is not supported by @title. We've unselected it for you in the Distribute to Academic Repositories below on the right. Supported types are: @types", array('@title' => $this->title, '@types' => implode(', ', $wpaper), '@type' => $type)));
      drupal_add_js('jQuery(document).ready(function() {jQuery("#edit-citation-distribute").removeClass("collapsed"); });', array('type' => 'inline'));
      drupal_add_js('jQuery(document).ready(function() {jQuery("#edit-repec").attr("checked", false); });', array('type' => 'inline'));
      drupal_add_js('jQuery("div.form-item-repec").addClass("form-select error");', array('type' => 'inline'));
      $return = false;
    }

    // Repec needs at least one author.
    $has_author = false;
    foreach ($contributors as $author) {
      if ( isset($author['name']) && strlen($author['name'])>0 ) {
        $has_author = true;
        break;
      }
    }
    if (!$has_author) {
      form_set_error('biblio_contributors', 'At least one author is required for ' . $this->title . ' distribution');
      $return = false;
    }

    return $return;
  }

  // remove() when a node is unflagged, delete its rdf file.
  public function remove($nid) {
    $file = $this->_citation_distribute_service_repec_path() . $this->_nid_to_type($nid) . '/' . $this->_file_name($nid);
    return unlink($file);
  }

  /*
   * _citation_distribute_service_repec_path()
   * shortcut function for generating path to repec archive
   */
  public function _citation_distribute_service_repec_path() {
    return $this->archive_path . '/' . $this->archive_code . '/';
  }

  /*
   * _nid_to_type($nid)
   * shortcut function for deriving a repec type from a nid
   */
  public function _nid_to_type($nid) {
//     $query =  'SELECT type.name FROM {biblio} bib INNER JOIN {biblio_types} type ON bib.biblio_type = type.tid WHERE nid = %d ';
//     $bib_type = db_result(db_query($query, $nid));
    $query = db_select('biblio', 'b')
      ->condition('nid', $nid);
    $query->join('biblio_types', 'bt', 'b.biblio_type = bt.tid');
    $query->fields('bt', array('name'));
    $bib_type = $query->execute()->fetchColumn();

    return $this->_map_biblio_type($bib_type);
  }

  /*
   * _file_name($nid)
   * shortcut function for getting a file name from a nid
   */
  public function _file_name($nid) {
    return $this->_nid_to_type($nid) . '_' . $nid . '.rdf';
  }

  /* _write() saves rdf metadata to disk */
  public function _write($data, $nid) {
    $path = $this->_citation_distribute_service_repec_path() . $this->_nid_to_type($nid) . '/';

    //create the path if necessary.  otherwise fail.  mkdir goes to logs, but users get generic failure message
    if (!( is_dir($path) || mkdir($path, 0775, TRUE) )) {
      drupal_set_message(t('Could not save repec metadata to "%path".  Please contact support.', array('%path'=>$path)), 'error');
      return false;
    }

    $filename = $path . $this->_file_name($nid);
    return file_put_contents($filename, $data);
  }

  /* writes repec index file */
  public function _index() {
    //series file first
    $series_name = array(
      'sftwre' => 'Software',
      'journl' => 'Journal Article',
      'bookch' => 'Book Chapter',
      'booktm' => 'Book',
      'wpaper' => 'Working Paper',
    );

    //only create a listing for series that have their own folder
    //note that this means index should be run AFTER adding new rdfs.
    $path = $this->_citation_distribute_service_repec_path();
    $all_series = array_intersect(scandir($path), array_keys($series_name));

    //generate series listing
    $seri = '';
    foreach ($all_series as $series) {
      $sn = $series_name[$series];

      $seri .= 'Template-Type: ReDIF-Series 1.0' . "\n";
      $seri .= 'Name: ' . $sn . "\n";
      $seri .= 'Provider-Name: '. $this->inst_name . "\n";
      $seri .= 'Provider-Homepage: ' . $this->provider_homepage . "\n";
      $seri .= 'Provider-Institution:  ' . $this->provider_institution . "\n";
      $seri .= 'Maintainer-Name: ' . $this->maintainer_name . "\n";
      $seri .= 'Maintainer-Email: ' . $this->maintainer_email . "\n";
      $seri .= 'Type: ReDIF-' . array_pop(explode(' ', $sn)) . "\n";    //only takes last word of series name...
      $seri .= 'Handle: RePEc:' . $this->archive_code . ':' . $series . "\n";
      $seri .= "\n";
    }

    //write to series file
    $file = $this->archive_code . 'seri.rdf';
    $filename = $path . '/' . $file;
    $series_out = file_put_contents($filename, $seri);

    //now for the arch file
    $arch  = 'Template-Type: ReDIF-Archive 1.0' . "\n";
    $arch .= 'Handle: RePEc:' . $this->archive_code . "\n";
    $arch .= 'Name: ' . $this->inst_name . "\n";
    $arch .= 'Maintainer-Email: ' . $this->maintainer_email . "\n";
    $arch .= 'Description: ' . $this->inst_name . "\n";
    $arch .= 'URL: ' . $this->archive_url . "\n";

    //write
    $file = $this->archive_code . 'arch.rdf';
    $filename = $path . '/' . $file;
    $arch_out = file_put_contents($filename, $arch);

    //and done
    return ($series_out && $arch_out);
  }


  //admin form
  //if present, this form will be used as an admin menu for CD:repec
  public function admin_form() {
    $form = array();
    $form['repec'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure RePEc'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

  $form['repec'] = array(
      '#type' => 'fieldset',
      '#title' => t('Enter RePEc Information'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      );

  $form['repec']['citation_distribute_service_repec_inst_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Institution Name'),
      '#default_value' => variable_get('citation_distribute_service_repec_inst_name',''),
      '#description' => t('Enter your institution\'s Name'),
      );


  $form['repec']['citation_distribute_service_repec_archive_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Archive Code'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_archive_code',''),
      '#description' => t('Enter your institution\'s 3 letter archive code' )

    );

  $form['repec']['citation_distribute_service_repec_provider_institution'] = array(
      '#type' => 'textfield',
      '#title' => t('Provider Institution Code'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_provider_institution',''),
      '#description' => t('Enter your institution\'s provider code.  e.g. RePEc:edi:adsflkj' )

    );
  $form['repec']['citation_distribute_service_repec_archive_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Archive Root Path'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_archive_path',''),
      '#description' => t('Enter the absolute path to the root of the RePEc archive. Do not include the RePEc directory.
       <br> For example: /nfs/test/archive' )

    );

  $form['repec']['citation_distribute_service_repec_archive_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Archive URL'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_archive_url',''),
      '#description' => t("Enter your institution's archive base URL.
      Do not include any directories in the URL. <br> For example: http://test.repec.com" )
    );

    $form['repec']['citation_distribute_service_repec_maintainer_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Maintainer\'s Email Address'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_maintainer_email',''),
      '#description' => t("Email address for contacting the maintainer of this archive." )
    );

    $form['repec']['citation_distribute_service_repec_maintainer_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Maintainer\'s Name'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_maintainer_name',''),
      '#description' => t("Name of the maintainer of this RePEc archive." )
    );

    $form['repec']['citation_distribute_service_repec_provider_homepage'] = array(
      '#type' => 'textfield',
      '#title' => t('Provider\'s Homepage'),
      '#required' => TRUE,
      '#default_value' => variable_get('citation_distribute_service_repec_provider_homepage',''),
      '#description' => t("Homepage for this archive." )
    );
    return $form;
  }
}

