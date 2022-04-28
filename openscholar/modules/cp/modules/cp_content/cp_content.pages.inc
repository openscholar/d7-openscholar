<?php
function cp_content_add_page() {
  $item = menu_get_item('node/add');
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.

  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  $bundles = os_get_bundles();

  $links = array();
  foreach ($bundles as $type => $name) {
    $url_str = str_replace('_', '-', $type);
    foreach ($content as $link) {
      if ($link['link_path'] == 'node/add/'.$url_str) {
        $links[] = $link;
      }
    }
  }

  return theme('node_add_list', array('content' => $links));
}