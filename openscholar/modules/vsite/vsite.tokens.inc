<?php

/**
 * @file
 * Builds placeholder replacement tokens for node-related data.
 */

/**
 * Implements hook_token_info().
 */
function vsite_token_info() {
  $type = array(
    'name' => t('Vsite'),
    'description' => t('Tokens related to a individual subsite, or "vsite".'),
  );

  // Core tokens for vsites.
  $vsite_tokens['site-purl'] = array(
    'name' => t("Site PURL"),
    'description' => t('The distinct purl associated with a "vsite".'),
  );
  
  return array(
    'types' => array('vsite' => $type),
    'tokens' => array('vsite' => $vsite_tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function vsite_tokens($type, $tokens, array $data = array(), array $options = array()) {
  
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  
  //Are we replacing a vsite token
  if ($type == 'vsite') {
  	
  	$group_node = false;
    /**
     * Try and get the vsite in one of three ways
     * 1.) The entity we are acting on is the vsite (group type)
     * 2.) The entity we are acting on is group content, extract the group from the og_grp_ref fields
     * 3.) We are in the space context but the entity does not hold the ref field
     */
  	if(!empty($data['node']) && og_is_group('node', $data['node'])){
    	$group_node = $data['node'];
    }elseif(!empty($data['node']) && og_is_group_content_type('node', $data['node']->type) && ($groups = og_get_entity_groups('node',$data['node']))){
    	
    	if(isset($groups['node'])){
	    	foreach ($groups['node'] as $group) {
	    		$grp = entity_load_single('node',$group);
	    		if(is_object($grp) && og_is_group('node', $grp)){
	    			$group_node = $grp;
	    			break;
	    		}
	    	}
    	}
    }
    
    if (!$group_node && ($site = spaces_get_space())){
    	$group_node = $site->group;
    }
    
    if (!$group_node){
      //Unable to load a vsite
      return $replacements;
    }
    

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'site-purl':
        	$purl = ($sanitize)? check_plain($group_node->purl):$group_node->purl;
          $replacements[$original] = $purl;
        break;
      }
    }
  }

  return $replacements;
}
