<?php

$plugin = array(
  'class' => 'citation_distribute_service',
  'file' => 'citation_distribute_service.inc',
);

/**
 * Abstract base class defining a server for citation distribution.
 */
abstract class citation_distribute_service {
//	public $name;
//	public $type; //push, file, metadata.  may not be really necessary.

	/**
	 * constructor function
	 *
	 * Doesn't do anything, but some plugins need this defined.
	 */
	function __construct(){
	}

	/**
	 * Distributes a node to chosen service
	 * @param int $nid
	 *   node id to distribute
	 * @param array plugin
	 *   CD's definition of this plugin
	 * @return bool
	 *   status of save/push
	 */
	abstract public function save($nid, $plugin);

	/**
	 * Copies data from biblio and node data into array labeled for this service
	 *
	 * @param int nid
	 *   node id
	 * @return array
	 *   mapping of metadata keys and values to distribute
	 */
	abstract public function map_metadata($nid);

	/**
	 * Themes data into format appropriate for this service
	 *
	 * @param array $metadata
	 *   associative array of metadata keys and values to map.
	 *
	 */
	abstract public function render($metadata);
}