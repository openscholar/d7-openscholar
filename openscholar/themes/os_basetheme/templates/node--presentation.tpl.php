<?php
/**
 * @file
 * Custom teaser and full node display overrides for Presentation nodes.
 *
 * @see /themes/adaptivetheme/at_core/templates/node.tpl.php
 * @see /modules/os_features/os_presentations
 */

hide($content['comments']);
hide($content['links']);

if (!$page) {
  $body_value = '';
  if (!empty($content['body']['#items'][0]['value'])) {
    $body_value = render($content['body']);
  }
  if (isset($content['field_presentation_location']) && $content['field_presentation_location']['#items'][0]['value'] !== NULL) {
    $location_value = $content['field_presentation_location']['#items'][0]['value'];
  }

  if (isset($content['field_presentation_date']) && $content['field_presentation_date'][0]['#markup'] !== NULL) {
    $date_value = $content['field_presentation_date'][0]['#markup'];
  }

  if (isset($content['field_presentation_file'])) {
    // Renders all files in a list
    $file_value = render($content['field_presentation_file']);
  }
}

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="node-inner">
  <?php if ($teaser): // begin teaser ?>
    <?php print render($title_prefix); ?>
      <span class="title" <?php echo $title_attributes; ?>>
        <strong>
          <a href="<?php print $node_url; ?>" title="<?php print $title ?>">
            <?php print $title; ?></a><?php if (isset($location_value) && !empty($location_value)): ?>, <?php endif; ?>
        </strong>
      </span>
    <?php print render($title_suffix); ?>
    <?php if (isset($location_value) && !empty($location_value)): ?>
      at
      <span class="location">
        <strong><?php print $location_value; ?></strong><?php if (isset($date_value) && !empty($date_value)): ?>, <?php endif; ?>
      </span>
    <?php endif; ?>
    <?php if (isset($date_value) && !empty($date_value)): ?>
      <?php print $date_value; ?><?php if (isset($file_value) && !empty($file_value)): ?>: <?php endif; ?>
    <?php endif; ?>
    <?php if (isset($file_value) && !empty($file_value)): ?>
      <?php print $file_value; ?>
    <?php endif; ?>
  <?php endif; // end teaser ?>

  <?php if (!$teaser): // begin default adaptivetheme full page node tpl ?>
    <?php print render($title_prefix); ?>
    <?php if ($title && !$page): //widgets can display content on its own page?>
      <header<?php print $header_attributes; ?>>
        <?php if ($title): ?>
          <h1<?php print $title_attributes; ?>>
            <a href="<?php print $node_url; ?>" rel="bookmark"><?php print $title; ?></a>
          </h1>
        <?php endif; ?>
      </header>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if(!empty($user_picture) || $display_submitted): ?>
      <footer<?php print $footer_attributes; ?>>
        <?php print $user_picture; ?>
        <p class="author-datetime"><?php print $submitted; ?></p>
      </footer>
    <?php endif; ?>

    <div<?php print $content_attributes; ?>>
      <?php print render($content); ?>
    </div>

    <?php if ($links = render($content['links'])): ?>
      <nav<?php print $links_attributes; ?>><?php print $links; ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  <?php endif; ?>
  <div<?php print $content_attributes; ?>>
    <?php print $body_value; ?>
  </div>
  <?php if ($teaser): // show vocabulary in case of teaser at bottom ?>
    <?php print render($content['og_vocabulary']); ?>
  <?php endif; // end teaser for vocabulary ?>
  </div> <!-- /div.node-inner -->
</article>
