<?php

/**
 * @file
 * Theme template for a list of tweets.
 *
 * Available variables in the theme include:
 *
 * 1) An array of $tweets, where each tweet object has:
 *   $tweet->id
 *   $tweet->username
 *   $tweet->userphoto
 *   $tweet->text
 *   $tweet->timestamp
 *   $tweet->time_ago
 *
 * 2) $twitkey string containing initial keyword.
 *
 * 3) $title
 *
 */
?>
<?php if ($lazy_load): ?>
  <?php print $lazy_load; ?>
<?php else: ?>

<div class="tweets-pulled-listing">

  <?php if (!empty($title)): ?>
    <h2><?php print $title; ?></h2>
  <?php endif; ?>

  <?php if (is_array($tweets)): ?>
    <?php $tweet_count = count($tweets); ?>
    
    <ul class="tweets-pulled-listing">
    <?php foreach ($tweets as $tweet_key => $tweet): ?>
      <li>
        <div class="tweet-authorphoto"><img src="<?php print $tweet->userphoto; ?>" alt="<?php print $tweet->username; ?>" /></div>
        <?php if ($tweet->is_retweet) { print "<span class=\"fa fa-retweet\" ></span>"; } ?>
        <span class="tweet-author"><?php print l($tweet->username, '//twitter.com/' . $tweet->username, array('attributes' => array('target' => '_blank'))); ?></span>
        <span class="tweet-text"><?php print preg_replace('/(<a href="(http|https):[^"]+")/is','\\1 target="_blank"',  twitter_pull_add_links($tweet->text)); ?></span>
        <?php if ($tweet->media_url): ?>
        <div class="tweet-media_url"><img src="<?php print str_replace('http://', '//', $tweet->media_url); ?>" /></div>
        <?php endif; ?>
        <div class="tweet-time"><?php print l($tweet->time_ago, '//twitter.com/' . $tweet->username . '/status/' . $tweet->id, array('attributes' => array('target' => '_blank')));?></div>

        <?php if ($tweet_key < $tweet_count - 1): ?>
          <div class="tweet-divider"></div>
        <?php endif; ?>
        
      </li>
    <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>

<?php endif; ?>
