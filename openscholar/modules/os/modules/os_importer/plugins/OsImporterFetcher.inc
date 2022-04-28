<?php
/**
 * @file
 *
 * Provides the Class for Feeds Token Fetcher.
 */
/**
 * Base class for the token fetcher.
 */
class OsImporterFetcher extends FeedsFetcher {

  public function fetch(FeedsSource $source) {
    $wrapper = entity_metadata_wrapper('node', $source->feed_nid);

    if (empty($wrapper->field_rss_url)) {
      return new FeedsFetcherResult('<xml></xml>');
    }

    // Build the URL for import and verify for response code 200 from the URL.
    $result = drupal_http_request($wrapper->field_rss_url->value());

    if ($result->code != 200) {
      // oops... error!
      drupal_set_message(t('Unable to get feeds from: @url', array('@url' => $wrapper->field_rss_url->value())));
      return new FeedsFetcherResult('<xml></xml>');
    }

    return new FeedsFetcherResult($result->data);
  }
}
