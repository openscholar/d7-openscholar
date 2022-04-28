<?php

/**
 * @file
 * Class for sword plugin
 */

//should there be a getlink function that can link a page to the corresponding dash article?

$plugin = array(
  'class' => 'citation_distribute_service_sword',
  'file' => 'citation_distribute_service_sword.inc',
  'parent' => 'citation_distribute_service',
);


abstract class citation_distribute_service_sword extends citation_distribute_service {
	public $packaging = 'http://purl.org/net/sword-types/METSDSpaceSIP';

	/*
	 * get_collection()
	 * given a workspace, choose the correct collection to use
	 */
	abstract public function get_deposit_url($workspaces, $nid);

	/**
	 * Consctructor
	 */
  public function __construct() {
  	parent::__construct();

  	/* ensure that $location has trailing slash.  sword server won't respond to requests with a double slash */
  	if (!preg_match('/\/$/', $this->location)) {
  		$this->location .= '/';
  	}
  }

	/*
	 * Implementation of citation_distribute_service::save()
	 */
	public function save($nid, $plugin) {
		//doublecheck the validation for this nid.
	  if (!$this->validate((int)$nid)) {
	    return false;
	  }

		$metadata = $this->map_metadata($nid);
		$out = $this->render($metadata, $nid);
		$saved = $this->_push($out, $nid);

		return $saved;
	}

	/*
	 * Implementation of citation_distribute_service::render()
	 */
	public function render($metadata) {
		$nid = func_get_arg(1);

		/* ensure we have a working directory or at least print some errors if we don't */
		$root_dir = $this->root_in . '/' . $this->subdir_in . '/' . $nid;
		if (!is_dir($root_dir)) {
			mkdir($root_dir, 0770, TRUE) || drupal_set_message('citation_distribute: could not create deposit directory ' . $root_dir, 'error');
		}

		/* include sword lib */
		require_once(drupal_get_path('module', 'citation_distribute') .	'/plugins/service/swordapp-php-library-1.0/packager_mets_swap.php');
		$zipfile = $nid . '_' . $this->name . '-' . $this->file_out;
		$packager = new PackagerMetsSwap($this->root_in, $this->subdir_in, $this->root_out, $zipfile);

		/* copy files from drupal to tmp dir */
		$full_dir = dirname(__FILE__) . '/';
		$mod_dir  = drupal_get_path('module', 'citation_distribute') . '/plugins';


    if (isset($metadata['File'])) {
  		foreach ($metadata['File'] as $delta => $file) {
  			$filename = end(explode('/', $file['filepath']));
  			$cp = copy(drupal_realpath($file['filepath']), $root_dir . '/'. $filename);
  			$metadata['File'][$delta]['filepath'] = $nid . '/' . $filename ;
  		}
    } else {
      return FALSE; //no file
    }

		/* add metadata to mets xml */
		foreach ($metadata as $name => $data) {
			// add to arrays
			if (is_array($data)) {
				$func = 'add' . $name;
				foreach ($data as $delta => $d) {
					if ($name == 'File') {
						$packager->$func($d['filepath'], $d['filemime']);  //	addFile takes mime as well
					} else {
						$packager->$func($d);
					}
				}
			} else {
				//set scalars if they have content
				if (strlen($data) > 0) {
	  			$func = 'set' . $name;
		  		$packager->$func(strip_tags($data));
				}
			}
		}

		/* attempt to write to xml file */
		try {
  		$packager->create();
		} catch (Exception $e) {
      watchdog('SWORD', 'Exception at: @file, @line: @message', array(
        '@file' => $e->getFile(),
        '@line' => $e->getLine(),
        '@message' => $e->getMessage()
      ));
			drupal_set_message('service_sword: ' . $e->getMessage(), 'error');
		return false;
	}

	/* returns path to saved file */
	return $this->root_out . '/' . $zipfile;

	}

	/*
	 * Implementation of citation_distribute_service::map_metadata()
	 */
	public function map_metadata($nid){
		$node = node_load($nid);

		/* copy static options and direct mappings first */
		$metadata = array(
			'Type' => 'http://purl.org/eprint/entityType/ScholarlyWork',
			'Title' => $node->title,
		  'Custodian' => $node->biblio_publisher,
	  	'Abstract' => $node->biblio_abst_e,
		);

		/* Add each author to Creator metadata */
		foreach($node->biblio_contributors as $delta => $contributor) {
			$metadata['Creator'][] = $contributor['name'];
		}

		/* Add each file as well */
		$files = $node->field_upload;
		if (empty($files)) {
		  $citation_distribute_node_object = &drupal_static('citation_distribute_node_object');
		  $files = $citation_distribute_node_object->field_upload;
		}
		foreach($files as $lang => $lang_files) {
		  foreach ($lang_files as $file) {

		    //$file = file_load($file->fid);
		    //file load won't work yet, but data is already in db.
        $query = db_select('file_managed', 'fm')
          ->condition('fid', $file['fid'])
          ->fields('fm', array('filemime', 'uri'));
        $f = $query->execute()->fetchAssoc();

        if ($f) {
  	      $metadata['File'][] = array(
  		  	 'filepath'=>$f['uri'],
  			   'filemime'=>$f['filemime'],
  			  );
        }
		  }
		}

		/* Create and add citation */
		//$citation = 	theme('biblio_style', $node, variable_get('biblio_base', 'biblio'), biblio_get_style());
		$citation = theme('biblio_style', array('node'=>$node, 'style_name'=>biblio_get_style()));
		$citation = strip_tags($citation);
	  $citation = preg_replace('/&nbsp;/', ' ', $citation);
		$metadata['Citation'] = $citation;

		/* Haven't found a mapping for these yet.
	  'Identifier' => '',
	  'DateAvailable' => '',
	  'StatusStatement' => '',
	  'CopyrightHolder' => '',
	  */

		return $metadata;
	}

	/* send data to sword service */
	private function _push($file, $nid) {
		require_once(drupal_get_path('module', 'citation_distribute') .	'/plugins/service/swordapp-php-library-1.0/swordappclient.php');
		$sac = new SWORDAPPClient();
		$sac_doc = $this->_get_service_doc($sac);
		$deposit_url = $this->get_deposit_url($sac_doc->sac_workspaces, $nid);

		/* attempt a deposit */
		if ($deposit_url) {
			try {
				$deposit = $sac->deposit($deposit_url, $this->username, $this->password, $this->obo, $file, $this->packaging, 'application/zip');
			} catch(Exception $e) {
				drupal_set_message('DASH deposit error', 'error');
        watchdog('SWORD', 'Exception at: @file, @line: @message', array(
          '@file' => $e->getFile(),
          '@line' => $e->getLine(),
          '@message' => $e->getMessage()
        ));
				return false;
			}
		} else {
			drupal_set_message('Unable to submit document to ' . $this->name . '.  No collection available for deposit.', 'error');
			return false;
		}


		/* DEBUG: check the deposit status */
//		drupal_set_message($deposit->sac_statusmessage);//(string:79) Unacceptable content type in deposit request: application/x-www-form-urlencoded

		if ($deposit->sac_status < 200 || $deposit->sac_status >= 300) {
			dsm($deposit);
			return false;
		} //TODO: make some less dev-ey error messages

		return true;
	}

	private function _get_service_doc(SWORDAPPClient $sac) {

		/* retrieve service document from sword server */
		$service_url = $this->location . 'sword/servicedocument';
		$sac_doc = $sac->servicedocument($service_url, $this->username, $this->password, $this->obo);

	  /* check status of service doc.  fix errors if possible, bail if not */
		switch($sac_doc->sac_status) {
			case 401:
				//auth problems are usually a bad on-behalf-of request.  drop the obo.
				$sac_doc = $sac->servicedocument($service_url, $this->username, $this->password, '');
				//only break if we're still unauthorized.  otherwise check other cases
				if ($sac_doc->sac_status == 401) {
					drupal_set_message('Couldn\'t log in to SWORD server.', 'error');
					return false;
				}
			case 200:
				//logged in!
				break;
			default:
				//other, nonspecific error
				drupal_set_message('Error '.$sac_doc->sac_status.' connecting to sword server', 'error');
        watchdog('SWORD', 'HTTP @code Error connecting to SWORD server', array(
          '@code' => $sac_doc->sac_status,
        ));
				return false;
		}

		return $sac_doc;
	}


}
