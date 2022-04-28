<?php

/**
 * Themable function to generate message after user submits
 *  cite_distribute widget selections
 *
 * @param array $metadata
 *  	associative array of GS metadata
 * @return unknown
 *   HTML string of that metadata
 */
function theme_googlescholar($metadata){
	$output = "";
	foreach($metadata as $key => $value) {
		if ($value) {
			if (is_array($value)) {
				foreach($value as $subvalue) {
					$output .= '<meta name="' . $key . '" content="' . $subvalue . '" />' . "\n";
				}
			} else {
				$output .= '<meta name="' . $key . '" content="' . $value . '" />' . "\n";
			}
		}
	}
  return $output;
}