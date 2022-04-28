<?php

/*
 * @ file defines dash as a sword destination
*/

$plugin = array(
  'class' => 'citation_distribute_service_sword_dash',
  'file' => 'citation_distribute_service_sword_dash.inc',
  'parent' => 'citation_distribute_service_sword',
  'title' => t('DASH'),
  'href' => 'http://dash.harvard.edu',
  'name' => 'dash',
  'description' => 'Harvard\'s open access repository',
);


class citation_distribute_service_sword_dash extends citation_distribute_service_sword {
	public $name = 			'dash';
	public $title = 		'DASH';
	public $location = 	'https://dash.harvard.edu';
	public $username;
	public $password;
	//public $username = 	'swap@lists.iq.harvard.edu';
	//public $password =  'citation_distribute_dash_password';    //variable to fetch for this password
	public $root_in  =  '/tmp/sword';
	public $subdir_in = 'dash';
	public $root_out =  '/tmp/sword';
	public $file_out =  'dash_files.zip';
	public $obo  	   =  '';

	public function __construct() {
	  parent::__construct();
	  $this->username = variable_get('citation_distribute_service_dash_username');
	  $this->password = variable_get('citation_distribute_service_dash_password');
	}

	//variable or function for determining school.

	// optional validate function
	// note that this take's nodeapi's $a3 arg (which is the $form during validation)
	// OR it can take a nid, depending on when the validation takes place.
	public function validate($a3) {

	  if (is_array($a3)) {
	    $files = $a3['values']['field_upload'];
	  } elseif (is_int($a3) && $node = node_load($a3)) {
	    $files = $node->field_upload;
	    if (empty($files)) {
	      //during node insert, files may not be availabe.  if so, use staticly cached node object.
  	    $citation_distribute_node_object = &drupal_static('citation_distribute_node_object');
  	    $files = $citation_distribute_node_object->field_upload;
	    }
	  } else {
	    //not sure what a3 is anymore, so bail.
	    return FALSE;
	  }

		//if (count($files) > 0) { // or body-> full text?
		if ($files[LANGUAGE_NONE][0]['fid']) { //has a file been added?
			return TRUE;
		} else {
			form_set_error('files', $this->title . ' requires the full text document be uploaded as an attachment.  Please upload your document in the Attach Files section of this form.');
			return FALSE;

			//TODO: check if a user has set his affiliation.  if not, send them to where that setting can be adjusted.
			//TODO: add a harvard affiliation setting.  can plugins modify admin pages I wonder?
		}

	}

	//if present, this form will be used as an admin menu for CD:repec
	public function admin_form() {
	  $form = array();
	  $form['dash'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Configure DASH'),
	    '#collapsible' => FALSE,
	    '#collapsed' => FALSE,
	  );

	  $form['dash']['citation_distribute_service_dash_username'] = array(
	    '#type' => 'textfield',
	    '#title' => 'DASH username',
	    '#description' => 'Desposits to DASH will be made via this user account.',
      '#default_value' => variable_get('citation_distribute_service_dash_username'),
    );

	  $form['dash']['citation_distribute_service_dash_password'] = array(
	    '#type' => 'textfield',
	    '#title' => 'DASH password',
	    '#default_value' => variable_get('citation_distribute_service_dash_password'),
	  );

	  return $form;
	}

  /*
	 * get_deposit_url($workspace)
	 * returns deposit url
	 * given workspaces/collections, try to figure out the correct collection.
	 */
	public function get_deposit_url($workspaces, $nid) {
		//get email address
		$query = db_select('users', 'u');
		$query->join('node', 'n', 'n.uid = u.uid');
		$query->condition('n.nid', $nid);
		$query->fields('u', array('mail'));
		$email = $query->execute()->fetchColumn();
		//$email = db_result(db_query($query, $nid));

		//get department of email domain
		$domain = end(explode('@', $email));
		$department = (isset($this->department_domains[$domain])) ? $this->department_domains[$domain] : $this->department_domains['harvard.edu'];

		//
		foreach ($workspaces as $workspace) {
			foreach ($workspace->sac_collections as $collection) {
				if ($collection->sac_colltitle == $department) {
					//yes this is a simplexmlelement.  yes curl and/or sword can read it
					$deposit_url = $collection->sac_href;
				}
			}
		}

		if (isset($deposit_url)) {
			return $deposit_url;
		} else {
			return false;
		}
	}

	// DASH has no interest in metadata only submissions.  Other sword repos may feel otherwise.
	// $node->body is where biblio keeps full text of articles.  maybe that could be saved...
	// service_sword should check
	//public $document_required = true;

	/*******************
	 * r. engel's list of minimum data:
	 *
	 * submitter info:
	 * name, email, primary affiliation (which school)
	 *
	 * article info:
	 * authors, title, journal/proceedings/etc, year
	 * ****************/

	/* mapping of email domains to harvard departments by collection name */
	public $department_domains = array(
		'fas.harvard.edu' => 'FAS Scholarly Articles',
		'harvard.edu' =>  'FAS Scholarly Articles',
		'iq.harvard.edu' =>  'FAS Scholarly Articles',
		'lists.iq.harvard.edu' =>  'FAS Scholarly Articles',
		'hmdc.harvard.edu' =>  'FAS Scholarly Articles',

		'hsps.harvard.edu' => 'HSPH Scholarly Articles',

		'hms.harvard.edu' => 'HMS Scholarly Articles',

		'law.harvard.edu' => 'HLS Scholarly Articles',
		'mail.law.harvard.edu' => 'HLS Scholarly Articles',
		'llm11.law.harvard.edu' => 'HLS Scholarly Articles',
		'jd13.law.harvard.edu' => 'HLS Scholarly Articles',

		'hds.harvard.edu' => 'HDS Scholarly Articles',

		'gse.harvard.edu' => 'GSE Scholarly Articles',

		'gsd.harvard.edu' => 'GSD Scholarly Articles',
	);
	/* unknown domains:
	 * wjh
	 * post
	 * gov
	 * seas
	 * mail
	 * wcfia
	 * hks
	 * rwj
	 * mcb
	 * law
	 * hksphd
	 * harvardd.edu do we do typo correction?  (see also: fass.)
	 * schepens
	 * math
	 * ksg
	 * jbcc
	 * hulmail
	 * hbsp
	 * eecs
	 * college
	 * chemistry
	 * cga
	 * bwh
	 * bidmc
	 * mgh
	 * oeb
	 * cfa
	 *
	 * other schools/colleections:
	 *
'Harvard Business School'
'Harvard Central Administration'




'John F. Kennedy School of Government'

	 */
}
