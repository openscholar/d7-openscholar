<?php

/**
 * Searches through textareas and file fields for references to this file
 * We need the full file for its fid and uri
 *
 * @param $file   Fully loaded file entity
 * @return array  All entities that use this file, grouped by entity type
 */
function _os_files_install_find_nodes($file) {
  $fields = &drupal_static(__FUNCTION__, array());
  $file_field_types = array(
    'file',
    'image',
    'imagefield_crop',
  );
  $text_field_types = array(
    'text_with_summary',
    'text_long',
  );

  if (empty($fields)) {
    $q = db_select('field_config', 'fc')
      ->fields('fc', array('field_name', 'type'))
      ->condition('active', 1)
      ->condition('deleted', 0)
      ->condition('type', array_merge($file_field_types, $text_field_types))
      ->execute();

    foreach ($q as $r) {
      $fields[$r->field_name] = $r->type;
    }
  }

  $filename = basename($file->uri);
  $pattern = '~("fid":"'.$file->fid.'"|data-fid="'.$file->fid.'"|'.$filename.'")~';

  $eids = array();
  foreach ($fields as $f => $t) {
    $table = 'field_data_'.$f;
    $module = 'file';
    $count = 0;

    $q = db_select($table, 'ft')
      ->fields('ft', array('entity_type', 'entity_id'));
    if (in_array($t, $file_field_types)) {
      $q->condition($f.'_fid', $file->fid);
    }
    elseif (in_array($t, $text_field_types)) {
      $q->condition($f.'_format', 'filtered_html');
      $q->fields('ft', array($f.'_value'));
      $or = db_or()
        ->condition($f.'_value', '%"fid":"'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%data-fid="'.$file->fid.'"%', 'LIKE')
        ->condition($f.'_value', '%'.$filename.'%', 'LIKE');
      $q->condition($or);
      $module = 'media';
    }
    $q = $q->execute();

    foreach ($q as $r) {
      // account for multiple usages of a file
      $count = 1;
      if ($module == 'media') {
        $count = preg_match_all($pattern, $r->{$f.'_value'}, $arr, PREG_PATTERN_ORDER);
      }

      $eids[$r->entity_type][] = array(
        'id' => $r->entity_id,
        'module' => $module,
        'count' => $count
      );
    }
  }

  $biblio_fields = array(
    'biblio_notes',
    'biblio_full_text',
    'biblio_abst_e'
  );

  $q = db_select('biblio', 'b')
    ->fields('b', array_merge(array('nid'), $biblio_fields));
  $bib_or = db_or();
  foreach ($biblio_fields as $bf) {
    $bib_or
      ->condition($bf, '%"fid":"'.$file->fid.'"%', 'LIKE')
      ->condition($bf, '%data-fid="'.$file->fid.'"%', 'LIKE')
      ->condition($bf, '%'.$filename.'%', 'LIKE');
  }
  $q->condition($bib_or);
  $q = $q->execute();

  foreach ($q as $r) {
    $count = 0;
    foreach ($biblio_fields as $b) {
      $count = preg_match_all($pattern, $r->{$b}, $arr, PREG_PATTERN_ORDER);
    }
    $eids['node'][] = array(
      'id' => $r->nid,
      'module' => 'media',
      'count' => $count
    );
  }


  return $eids;
}

function _os_files_install_find_files($node) {
  $start = microtime(true);
  $fields = &drupal_static(__FUNCTION__, array());
  $file_field_types = array(
    'file',
    'image',
    'imagefield_crop',
  );
  $text_field_types = array(
    'text_with_summary',
    'text_long',
  );

  if (empty($fields)) {
    $q = db_select('field_config', 'fc')
      ->fields('fc', array('field_name', 'type'))
      ->condition('active', 1)
      ->condition('deleted', 0)
      ->condition('type', array_merge($file_field_types, $text_field_types))
      ->execute();

    foreach ($q as $r) {
      $fields[$r->field_name] = $r->type;
    }
  }
  $space = spaces_og_spaces_get_space_from_object('node', $node);
  $path_prefix = $space ? $space->group->purl : NULL;

  $files = array();
  foreach ($fields as $name => $type) {
    if (empty($node->{$name})) continue;
    $field = $node->{$name};
    if (in_array($type, $file_field_types)) {
      foreach ($field[LANGUAGE_NONE] as $f) {
        $files['file'][$f['fid']]++;
      }
    }
    elseif (in_array($type, $text_field_types)) {
      foreach ($field[LANGUAGE_NONE] as $b) {
        $usage =  _os_files_install_get_files_from_string($b['value'], $path_prefix);
        foreach ($usage as $fid => $count) {
          $files['media'][$fid] += $count;
        }
      }
    }
  }

  if ($node->type == 'biblio') {
    $biblio_fields = array(
      'biblio_notes',
      'biblio_full_text',
      'biblio_abst_e'
    );

    foreach ($biblio_fields as $bf) {
      $usage =  _os_files_install_get_files_from_string($node->{$bf}, $path_prefix);
      foreach ($usage as $fid => $count) {
        $files['media'][$fid] += $count;
      }
    }
  }

  return $files;
}

function _os_files_install_get_files_from_string($string, $space = null) {
  $fid_pattern = '~"fid":"([\d]+)"|<a[^>]+data\-fid="([\d]+)"~';
  $image_pattern = '~<img[^>]+src="([^"]*)"[^>]*>~';
  $link_pattern = '~<a[^>]+href="([^"]*)"[^>]*>[^<]*<\/a>~';
  $files = array();


  $matches = array();
  preg_match_all($fid_pattern, $string, $matches, PREG_SET_ORDER);
  foreach ($matches as $m) {
    if (empty($m[1])) {
      $files[$m[2]]++;
    }
    else {
      $files[$m[1]]++;
    }
  }

  preg_match_all($image_pattern, $string, $matches);
  foreach ($matches[1] as $m) {
    $filename = basename($m);
    if ($space) {
      $path = $space.'/files/'.$filename;
    }
    else {
      $path = $filename;
    }
    $file = file_uri_to_object('public://'.$path);
    if (!empty($file->fid) && strpos($m, '/'.$path) !== FALSE) {
      $files[$file->fid]++;
    }
  }

  preg_match_all($link_pattern, $string, $matches, PREG_SET_ORDER);
  foreach ($matches as $m) {
    if (strpos($m[0], 'data-fid') === FALSE) {
      if ($space) {
        $path = $space.'/files/'.$filename;
      }
      else {
        $path = $filename;
      }
      $file = file_uri_to_object('public://'.$path);
      if (!empty($file->fid) && strpos($m, '/'.$path) !== FALSE) {
        $files[$file->fid]++;
      }
    }
  }

  return $files;
}

/**
 * Determines if a file needs its space changed.
 * Returns the new space if it does
 *
 * @param $file
 * @param $node
 * @return int
 */
function _os_files_install_find_new_space($file, $node) {
  $current_space = null;

  if (isset($file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'])) {
    $current_space = $file->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
  }

  if (isset($node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id']) && $current_space != $node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id']) {
    return $node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'];
  }

  // return nothing if the node is in the correct space already
  return 0;
}