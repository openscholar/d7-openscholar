<?php
/**
 * @file
 * os_activity.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_activity_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function os_activity_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_message_type().
 */
function os_activity_default_message_type() {
  $items = array();
  $items['os_create_node'] = entity_import('message_type', '{
    "name" : "os_create_node",
    "description" : "OS create node",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 0 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Ca href=\\u0022@{message:field-group:url}\\u0022\\u003E@{message:field-group:title}\\u003C\\/a\\u003E created @{message:field-node-reference:content-type:name}: \\u003Ca href=\\u0027@{message:field-node-reference:url}\\u0027\\u003E!title\\u003C\\/a\\u003E",
          "format" : "full_html"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['os_edit_node'] = entity_import('message_type', '{
    "name" : "os_edit_node",
    "description" : "OS edit node",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 0 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Ca href=\\u0022@{message:field-group:url}\\u0022\\u003E@{message:field-group:title}\\u003C\\/a\\u003E edited @{message:field-node-reference:content-type:name}: \\u003Ca href=\\u0027@{message:field-node-reference:url}\\u0027\\u003E!title\\u003C\\/a\\u003E",
          "format" : "full_html"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}
