; Drupal.org Makefile for OpenScholar
; Patches in https://github.com/openscholar/openscholar-libraries/tree/master/d7patches

core = 7.x
api = 2

projects[advagg][subdir] = "contrib"
projects[advagg][version] = 2.6

projects[apachesolr][subdir] = "contrib"
projects[apachesolr][version] = 1.6
projects[apachesolr][patch][] = "https://www.drupal.org/files/issues/apachesolr-support_php_72-2946579-2.patch"

projects[apachesolr_attachments][subdir] = "contrib"
projects[apachesolr_attachments][version] = 1.2

projects[apachesolr_og][subdir] = "contrib"
projects[apachesolr_og][revision] = 5fda19fc208bd62713be99a839aed0528d9d3db7

projects[boxes][subdir] = "contrib"
projects[boxes][version] = 1.0-beta7
projects[boxes][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/5ee44eccd37a4dded599c822b8f1b0d114ed332f/d7patches/boxes.load_from_spaces_and_fix_fatals_v2.patch"
projects[boxes][patch][] = "https://www.drupal.org/files/1859150-1-boxes-strict_warning.patch"
projects[boxes][patch][] = "https://www.drupal.org/files/issues/default_object_empty_value-2042101-3.patch"

projects[cache_consistent][subdir] = "contrib"
projects[cache_consistent][version] = 1.2

projects[calendar][subdir] = "contrib"
projects[calendar][version] = 3.5
projects[calendar][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/64c329a8f952ebbd39ce03c4bf38d4fcfc43bd60/d7patches/calendar-fix-week-view-overflow_v2.patch"
projects[calendar][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/calendar.scroll_to_first_event.patch"

projects[captcha][subdir] = "contrib"
projects[captcha][version] = 1.5

projects[colorbox][subdir] = "contrib"
projects[colorbox][version] = 2.13

projects[comment_sources][subdir] = "contrib"
projects[comment_sources][version] = 2.0
projects[comment_sources][patch][] = "https://drupal.org/files/issues/22086870-comment-source-wrong-query-field-1.patch"
projects[comment_sources][patch][] = "https://drupal.org/files/issues/change-submit-handler-index-2513794-2.patch"
projects[comment_sources][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/c34949150fd73abe02b7c0315bbf93b5b94d066d/d7patches/comment_sources.9139.comment_settings_v2.patch"

projects[context][subdir] = "contrib"
projects[context][version] = 3.10
projects[context][patch][] = "http://drupal.org/files/os-custom-beta4.patch"
projects[context][patch][] = "http://drupal.org/files/1855004.context.float_block_weights.patch"
projects[context][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/a7b262f1a9784678387024d46a4a55f3852e0370/d7patches/context-10085-breadcrumb_menu_context_v2.patch"

projects[contextual_annotation][subdir] = "contrib"
projects[contextual_annotation][type] = module
projects[contextual_annotation][download][type] = git
projects[contextual_annotation][download][url] =  "http://git.drupal.org/project/contextual_annotation.git"
; needs patches to contrib

projects[ctools][subdir] = "contrib"
projects[ctools][version] = 1.10
;projects[ctools][download][type] = git
;projects[ctools][download][branch] = 7.x-1.x
;projects[ctools][download][revision] = be2607142ce97d093acce9417833640680330efe
;projects[ctools][patch][] = "http://drupal.org/files/1707810-ctools-fields-mock-field-7.patch"
;projects[ctools][patch][] = "http://drupal.org/files/ctools-plugin_extension-1623044-1.patch"
;projects[ctools][patch][] = "https://drupal.org/files/issues/2147905-allow-alter-with-more-context-1.patch"

projects[date][subdir] = "contrib"
projects[date][version] = 2.8

projects[date_ical][subdir] = "contrib"
projects[date_ical][version] = 2.7

projects[devel][subdir] = "contrib"
projects[devel][version] = 1.3

projects[disqus][subdir] = "contrib"
projects[disqus][version] = 1.9
projects[disqus][patch][] = "http://drupal.org/files/comment_sources-disqus-2120703-1.patch"
projects[disqus][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/disqus-9804_cross_domain_fix.patch"

projects[dragndrop_upload][subdir] = "contrib"
projects[dragndrop_upload][version] = 1.x-dev
projects[dragndrop_upload][revision] = f356772

projects[dyntextfield][subdir] = "contrib"
projects[dyntextfield][type] = module
projects[dyntextfield][download][type] = git
projects[dyntextfield][download][tag] = 1.0
projects[dyntextfield][download][url] = "https://github.com/amitaibu/dyntextfield.git"

projects[entity][subdir] = "contrib"
projects[entity][version] = 1.7
projects[entity][patch][] = http://drupal.org/files/1972668-file-delete-1.patch
projects[entity][patch][] = https://www.drupal.org/files/issues/2086225-entity-access-check-18.patch
projects[entity][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/b84d1690344730d1443a75ba36f903d3255458d6/d7patches/entity_wrapper.patch"

projects[entitycache][subdir] = "contrib"
projects[entitycache][version] = 1.5
projects[entitycache][patch][] = https://www.drupal.org/files/issues/2516094-1-entitycache_fix_arry_flip.patch
projects[entitycache][patch][] = https://www.drupal.org/files/issues/2018-07-05/entitycache_php_7-2981629-0.patch

projects[entityreference][subdir] = "contrib"
projects[entityreference][version] = 1.2
projects[entityreference][patch][] = "http://drupal.org/files/1802916-er-show-identifier-14.patch"

projects[entityreference_prepopulate][subdir] = "contrib"
projects[entityreference_prepopulate][version] = 1.5
projects[entityreference_prepopulate][patch][] = "https://drupal.org/files/issues/1994702-er-ajax-9.patch"

projects[elysia_cron][subdir] = "contrib"
projects[elysia_cron][version] = 2.4

projects[eva][subdir] = "contrib"
projects[eva][version] = 1.2
projects[eva][revision] = "6d92c27"

projects[expire][subdir] = "contrib"
projects[expire][version] = 2.0-beta2

projects[facetapi][subdir] = "contrib"
projects[facetapi][version] = 1.1
projects[facetapi][patch][] = "http://drupal.org/files/2006704-facetapi-er-regroup-2.patch"

projects[features][subdir] = "contrib"
projects[features][version] = 2.10
projects[features][patch][] = "https://www.drupal.org/files/issues/2018-03-12/features-support_php_72-2931464-10.patch"


projects[feeds][subdir] = "contrib"
projects[feeds][version] = 2.0-alpha7
projects[feeds][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/feeds-fid_9097_resubmitting.patch"

projects[feeds_tamper][subdir] = "contrib"
projects[feeds_tamper][version] = 1.2

projects[feeds_xpathparser][subdir] = "contrib"
projects[feeds_xpathparser][download][type] = git
projects[feeds_xpathparser][download][branch] = 7.x-3.x
projects[feeds_xpathparser][download][revision] = 5bea17e

projects[field_collection][subdir] = "contrib"
projects[field_collection][version] = 1.0-beta5

projects[field_group][subdir] = "contrib"
projects[field_group][version] = 1.1

projects[field_redirection][subdir] = "contrib"
projects[field_redirection][version] = 2.5

projects[file_entity][subdir] = "contrib"
projects[file_entity][download][type] = git
projects[file_entity][download][branch] = 7.x-2.x
projects[file_entity][download][revision] = 68ab8ed52f9bb993e8f3c541b89420637e440609
projects[file_entity][patch][] = "https://www.drupal.org/files/file_entity.1834902-3.dimension_overrides.patch"
projects[file_entity][patch][] = "https://www.drupal.org/files/filter-by-file-schema-type-1881356-12.patch"
; https://www.drupal.org/project/file_entity/issues/1987568
projects[file_entity][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/master/d7patches/file_entity-alt_title_double_encoding-1987568-68ab8ed52.patch"

projects[filefield_paths][subdir] = "contrib"
;projects[filefield_paths][version] = 1.1
; projects[filefield_paths][version] = 1.0-beta3+2-dev
projects[filefield_paths][download][type] = git
projects[filefield_paths][download][branch] = 7.x-5.x
projects[filefield_paths][download][revision] = 84fb637
projects[filefield_paths][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/deprecated-2103151-os.patch"

projects[flag][subdir] = "contrib"
projects[flag][version] = 3.9

projects[hierarchical_taxonomy][subdir] = "contrib"
projects[hierarchical_taxonomy][download][type] = git
projects[hierarchical_taxonomy][download][url] = "http://git.drupal.org/project/hierarchical_taxonomy.git"
projects[hierarchical_taxonomy][download][branch] = 7.x-1.x
projects[hierarchical_taxonomy][download][revision] = 5bbe344
projects[hierarchical_taxonomy][patch][] = "http://drupal.org/files/2034713-hs-comaprse-name-2.patch"

projects[honeypot][subdir] = "contrib"
projects[honeypot][version] = 1.24

projects[html_title][subdir] = "contrib"
projects[html_title][version] = 1.1
projects[html_title][patch][] = "https://www.drupal.org/files/issues/fix_for_other_titles-2783297.patch"

projects[imageapi_optimize][subdir] = "contrib"
projects[imageapi_optimize][download][type] = git
projects[imageapi_optimize][download][branch] = 7.x-1.x
projects[imageapi_optimize][download][revision] = 234f208

projects[imagemagick][subdir] = "contrib"
projects[imagemagick][version] = 1.0
projects[imagemagick][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/imagemagick.no-resize-for-animated-gifs.patch"

projects[imagefield_crop][subdir] = "contrib"
; projects[imagefield_crop][version] = 2.0
projects[imagefield_crop][download][type] = git
projects[imagefield_crop][download][branch] = 7.x-2.x
projects[imagefield_crop][download][revision] = 4a5302
projects[imagefield_crop][patch][] = "http://drupal.org/files/imagefield_crop-hook_imagefield_crop_instance_alter-1915510-2.patch"
projects[imagefield_crop][patch][] = "http://drupal.org/files/imagefield_crop-max_filesize-1923934-1.patch"

projects[jcarousel][subdir] = "contrib"
projects[jcarousel][version] = 2.6

projects[jquery_update][subdir] = "contrib"
projects[jquery_update][version] = 2.7
; projects[jquery_update][download][revision] = 65eecb0

projects[js][subdir] = "contrib"
projects[js][version] = 1.0-beta2

projects[job_scheduler][subdir] = "contrib"
projects[job_scheduler][version] = 2.0-alpha3

projects[libraries][subdir] = "contrib"
projects[libraries][version] = 2.1
projects[libraries][patch][] = "http://drupal.org/files/0001-Fix-1938638-by-coredumperror-Fix-typo.patch"

projects[link][version] = 1.6
projects[link][subdir] = "contrib"

projects[linkchecker][subdir] = "contrib"
projects[linkchecker][version] = 1.0-beta1

;projects[media][subdir] = "contrib"
;projects[media][version] = 2.1
; projects[media][download][type] = git
; projects[media][download][branch] = 7.x-2.x
; projects[media][download][revision] = "b433b278d7e0ab7420f5a874886843fb9fe7ebe2"
; projects[media][patch][] = "http://drupal.org/files/issues/1121808-media-resuse-files-by-uri.patch"
; projects[media][patch][] = "http://drupal.org/files/media-7.x-2.x-fix-class-array.patch"
; projects[media][patch][] = "http://drupal.org/files/media_fatal_wysiwyg_remove_1937864_11.patch"

projects[media_gallery][subdir] = "contrib"
projects[media_gallery][version] = 2.x-dev
projects[media_gallery][download][type] = git
projects[media_gallery][download][branch] = 7.x-2.x
projects[media_gallery][download][revision] = "f28ffd1a6f5eaa4eb6554643a3db4dd4543923e1"
projects[media_gallery][patch][] = "http://drupal.org/files/media_gallery-double_browser-1939186-3.patch"
projects[media_gallery][patch][] = "http://drupal.org/files/media_gallery-remove_taxonomy_shenanigans-1686498-9.patch"
projects[media_gallery][patch][] = "http://drupal.org/files/media_gallery-rename_field-1940036-3.patch"
projects[media_gallery][patch][] = "http://drupal.org/files/media_gallery-lightbox_double_file-1977822-1.patch"
projects[media_gallery][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/media_gallery-multiple.full.display.patch"
projects[media_gallery][patch][] = "https://www.drupal.org/files/issues/2585509-media_gallery-lost_files.patch"

projects[message][subdir] = "contrib"
projects[message][version] = 1.12

projects[metatag][subdir] = "contrib"
projects[metatag][version] = 1.22

projects[module_filter][subdir] = "contrib"
projects[module_filter][version] = 1.7

projects[memcache][subdir] = "contrib"
projects[memcache][version] = 1.6

projects[migrate][subdir] = "contrib"
projects[migrate][version] = 2.5

projects[migrate_extras][patch][] = "http://drupal.org/files/migrate_extras-duplicate_MigrateDestinationOgMembership-1788440-5.patch"
projects[migrate_extras][subdir] = "contrib"
projects[migrate_extras][version] = 2.4

projects[mollom][subdir] = "contrib"
projects[mollom][version] = 2.14

projects[multiform][subdir] = "contrib"
projects[multiform][version] = 1.0

projects[nice_menus][subdir] = "contrib"
projects[nice_menus][version] = 2.5
; todo change the address of the patch once the PR is merged,
projects[nice_menus][patch][] = "https://gist.githubusercontent.com/RoySegall/6118a03520c81ae4e012/raw/733e63036dd6d6ad4706f90569a6dc075f5ca0a3/nice_menus.fatal_localized_options.patch"

projects[nodeformcols][subdir] = "contrib"
projects[nodeformcols][version] = 1.0

projects[node_revision_restrict][subdir] = "contrib"
projects[node_revision_restrict][version] = 1.5

projects[entity_validator][subdir] = "contrib"
projects[entity_validator][download][type] = git
projects[entity_validator][download][url] = "https://github.com/Gizra/entity_validator.git"
projects[entity_validator][download][branch] = "7.x-1.x"
projects[entity_validator][download][revision] = dc26154

projects[nodeorder][subdir] = "contrib"
projects[nodeorder][version] = 1.1

projects[oembed][subdir] = "contrib"
projects[oembed][version] = 1.0-rc2
projects[oembed][patch][] = "http://cgit.drupalcode.org/oembed/patch/?id=a27adf7c1afe763ee5f386f30f0aea73a6097ff1"
projects[oembed][patch][] = "http://drupal.org/files/issues/oembed.2134415.wysiwyg_dimensions.patch"
projects[oembed][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/oembed.alt_tags_no_escape.patch"
projects[oembed][patch][] = "http://drupal.org/files/issues/split-up-regex-2739023-1.patch"
projects[oembed][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/oembed.8762.uri_fragments.patch"
projects[oembed][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/oembed.backup_providers.patch"
projects[oembed][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/oembed.9433.youtube_https_schema.patch"
projects[oembed][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/bf967967e18b992a9c2c8aaa8bcd83308dc916d2/d7patches/oembed.16779.youtube-embed-title.patch"
projects[oembed][patch][] = "https://www.drupal.org/files/issues/2020-11-19/oembed-sec_fix_sa_contrib_2020_036-3183485-3.patch"
projects[oembed][patch][] = "https://www.drupal.org/files/issues/2020-12-17/oembed-update_youtube_provider_endpoint_to_use_https-3188850-2.patch"
projects[oembed][patch][] = "https://www.drupal.org/files/oembed-2021015-1.patch"

projects[og][subdir] = "contrib"
projects[og][version] = 2.9
; projects[og][patch][] = "https://drupal.org/files/issues/2231217-og-save-no-entity-load-1.patch"
projects[og][patch][] = "https://drupal.org/files/issues/add_entity_type_parameter-2308279-3.patch"

projects[og_moderation][subdir] = "contrib"
projects[og_moderation][version] = 2.0
projects[og_moderation][patch][] = "http://drupal.org/files/og_moderation-remove_node_grants-2091179-7.patch"

projects[og_tasks][subdir] = "contrib"
projects[og_tasks][version] = 1.0
projects[og_tasks][download][type] = git
projects[og_tasks][download][url] = "http://git.drupal.org/project/og_tasks.git"
projects[og_tasks][patch][] = "http://drupal.org/files/port_code_to_og7.x-2_1834076_3.patch"
projects[og_tasks][patch][] = "http://drupal.org/files/check_spaces_preset-2059881-6.patch"
projects[og_views][subdir] = "contrib"
projects[og_views][version] = 1.0

projects[og_vocab][subdir] = "contrib"
projects[og_vocab][version] = 1.2
projects[og_vocab][patch][] = "https://drupal.org/files/issues/2224007-og-vocab-set-value-1.patch"
projects[og_vocab][patch][] = "https://drupal.org/files/issues/og-vocab-reset-to-alpahbetical-2174907-4.patch"
projects[og_vocab][patch][] = "https://drupal.org/files/issues/og-vocab-widget-settings-from-entity.patch"
projects[og_vocab][patch][] = "https://drupal.org/files/issues/hide-field-when-empty-2243091-1.patch"

projects[olark][subdir] = "contrib"
projects[olark][version] = 1.0-beta1
projects[olark][patch][] = "http://drupal.org/files/olark-suppress_roles-1984210-1.patch"
projects[olark][patch][] = "http://drupal.org/files/olark-js-to-d7-update-1785322-5.patch"

projects[password_policy][subdir] = "contrib"
projects[password_policy][version] = 2.0-alpha6
; projects[password_policy][patch][] = "http://drupal.org/files/password-policy-anonymous-users.patch"
; projects[password_policy][patch][] = "http://drupal.org/files/password_policy-remove_focus-1998862-7455062.patch"
; projects[password_policy][patch][] = "https://gist.github.com/sagotsky/7321750/raw/b26f07c902f7cd5ef8650a3a6891941054a4a8db/password_policy-consecutive_regex-2127421-1.patch"

projects[pathauto][subdir] = "contrib"
projects[pathauto][version] = 1.2

projects[pinserver][subdir] = "contrib"
projects[pinserver][type] = module
projects[pinserver][download][type] = git
projects[pinserver][download][url] = "https://github.com/openscholar/pinserver.git"
projects[pinserver][download][tag] = 7.x-3.4.25

projects[purl][subdir] = "contrib"
; projects[purl][version] = 1.0-beta1+11-dev
projects[purl][download][type] = git
projects[purl][download][branch] = 7.x-1.x
projects[purl][download][revision] = 469e8668
projects[purl][patch][] = "http://drupal.org/files/1473502-purl-init-2_0.patch"
projects[purl][patch][] = "http://drupal.org/files/1982198-purl-cache-set-1.patch"
projects[purl][patch][] = "http://drupal.org/files/729862-9-purl-inbound-alter-modifies-q.patch"
projects[purl][patch][] = "http://drupal.org/files/issues/purl-integrate_redirect_keep_options-1735266-4.patch"
projects[purl][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/7c23e1b93d708335a01535c2937c3a8c843edd9c/d7patches/purl_fix-redirect-failures.patch"

projects[recaptcha][subdir] = "contrib"
projects[recaptcha][version] = 2.2
projects[recaptcha][patch][] = "https://www.drupal.org/files/issues/2018-05-31/2493183_ajax_support_98.patch"

projects[registration][subdir] = "contrib"
projects[registration][version] = 1.5
projects[registration][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/registration_link_pass_entity.patch"
projects[registration][patch][] = "https://www.drupal.org/files/issues/registration-add-context-to-headers-2564355-1.patch"
projects[registration][patch][] = "https://www.drupal.org/files/issues/registration_fatal_error_class-2546836-6.patch"

projects[respondjs][subdir] = "contrib"
projects[respondjs][version] = 1.1

projects[restful][subdir] = "contrib"
projects[restful][version] = "1.10"
# Patch to be able to update a taxonomy term.
projects[restful][patch][] = "https://patch-diff.githubusercontent.com/raw/RESTful-Drupal/restful/pull/445.diff"
projects[restful][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/restful.put_file.patch"

projects[redirect][subdir] = "contrib"
projects[redirect][version] = 1.0-rc1
projects[redirect][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/01c080be3ea5572f34f744156f4b07d887427506/d7patches/redirect-9589-enable-redirection.patch"

projects[robotstxt][subdir] = "contrib"
projects[robotstxt][version] = "1.1"
projects[robotstxt][patch][] = "http://drupal.org/files/issues/send_cache_headers-1923838-5.patch"

projects[restws][subdir] = "contrib"
projects[restws][version] = "2.8"

projects[shorten][subdir] = "contrib"
projects[shorten][version] = 1.2

projects[securepages][subdir] = "contrib"
projects[securepages][version] = 1.0-beta1

projects[spaces][subdir] = "contrib"
; projects[spaces][version] = 3.0-alpha1+9-dev
projects[spaces][revision] = "eac3a7e"
projects[spaces][patch][] = "http://drupal.org/files/1470434-spaces-og-28.patch"
projects[spaces][patch][] = "http://drupal.org/files/spaces_ui-show_disabled-1662918-1.patch"
projects[spaces][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/spaces.disabled_features.patch"
projects[spaces][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/61a3280f4bb1bd76dae3b229e859a94233620ec2/d7patches/spaces_og-dont-delete-purl-early.patch"
projects[spaces][download][type] = git
projects[spaces][download][branch] = 7.x-3.x
projects[spaces][download][revision] = eac3a7e

projects[stringoverrides][version] = 1.8
projects[stringoverrides][subdir] = "contrib"

projects[strongarm][subdir] = "contrib"
projects[strongarm][version] = 2.0-rc1

projects[term_reference_tree][subdir] = "contrib"
projects[term_reference_tree][download][type] = git
projects[term_reference_tree][download][branch] = 7.x-1.x
projects[term_reference_tree][download][revision] = 98f658b9d8c4e4585c8fa98cf81aad6bf990f7b4
projects[term_reference_tree][patch][] = "https://drupal.org/files/issues/2235057-term-er-3.patch"

projects[title][subdir] = "contrib"
projects[title][version] = 1.0-alpha9

projects[token][subdir] = "contrib"
projects[token][version] = 1.4

projects[transliteration][subdir] = "contrib"
projects[transliteration][version] = 3.1

projects[twitter_pull][subdir] = "contrib"
projects[twitter_pull][version] = 1.0-rc4
projects[twitter_pull][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/twitter_pull-8577-retweet_option.patch"
projects[twitter_pull][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/twitter_pull.class.inc_get-media_url.patch"
projects[twitter_pull][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/twitter_pull-distinguish_retweets.patch"
projects[twitter_pull][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/twitter_pull.class.inc-tweet-mode-extended.patch"

projects[views][subdir] = "contrib"
projects[views][version] = 3.21

projects[views_bulk_operations][subdir] = "contrib"
projects[views_bulk_operations][version] = 3.4

projects[views_og_cache][subdir] = "contrib"
projects[views_og_cache][version] = 1.1
projects[views_og_cache][patch][] = "https://drupal.org/files/issues/2226219-easy-key-data-override-2.patch"

projects[views_slideshow][subdir] = "contrib"
projects[views_slideshow][version] = 3.9

projects[views_litepager][subdir] = "contrib"
projects[views_litepager][version] = 3.0
projects[views_litepager][patch][] = "http://drupal.org/files/views_litepager-requirements-array-error_1976056-2.patch"
projects[views_litepager][patch][] = "http://drupal.org/files/views_litepager-off_by_one-2006992-1.patch"

projects[views_infinite_scroll][subdir] = "contrib"
projects[views_infinite_scroll][version] = 1.1
; projects[views_infinite_scroll][patch][] = "http://drupal.org/files/views_infinite_scroll-vbo-1965288-2.patch"
; projects[views_infinite_scroll][patch][] = "http://localhost/patch/views_infinite_scroll-no_auto_scroll-2012910-1.patch"

projects[web_widgets][subdir] = "contrib"
projects[web_widgets][version] = 1.0-alpha2

projects[wysiwyg][subdir] = "contrib"
projects[wysiwyg][download][type] = git
projects[wysiwyg][download][branch] = 7.x-2.x
projects[wysiwyg][download][revision] = 9490393
projects[wysiwyg][patch][] = "http://drupal.org/files/wysiwyg-table-format.patch"

projects[wysiwyg_filter][subdir] = "contrib"
projects[wysiwyg_filter][version] = 1.6-rc3
projects[wysiwyg_filter][revision] = "4848d306a2f7526f7eeaf22edb9268d3c96654fe"
projects[wysiwyg_filter][patch][] = "https://www.drupal.org/files/issues/wysiwyg-filter-border-radius-support-2692163-1.patch"

; Libraries.
libraries[jquery.bgiframe][type] = "libraries"
libraries[jquery.bgiframe][download][type] = "git"
libraries[jquery.bgiframe][download][url] = "https://github.com/brandonaaron/bgiframe.git"

libraries[clippy][type] = "libraries"
libraries[clippy][download][type] = "file"
libraries[clippy][download][url] = "https://github.com/mojombo/clippy/archive/master.zip"

libraries[colorbox][type] = "libraries"
libraries[colorbox][download][type] = "file"
libraries[colorbox][download][url] = "https://github.com/jackmoore/colorbox/archive/1.4.14.zip"

libraries[jquery.cycle][type] = "libraries"
libraries[jquery.cycle][download][type] = "file"
; libraries[jquery.cycle][download][url] = "https://github.com/downloads/malsup/cycle/jquery.cycle.all.2.88.js"
; get 2.88 blob from github so it keeps default filename.
libraries[jquery.cycle][download][url] = "https://raw.github.com/malsup/cycle/c2b85942513801befea2ddf407eb7d2a17e441e8/jquery.cycle.all.js"

libraries[tinymce][type] = "libraries"
libraries[tinymce][download][type] = "file"
libraries[tinymce][download][url] = "http://github.com/downloads/tinymce/tinymce/tinymce_3.5.8.zip"

libraries[ckeditor][type] = "libraries"
libraries[ckeditor][download][type] = "get"
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.6.2/ckeditor_4.6.2_standard.zip"

libraries[spyc][type] = "libraries"
libraries[spyc][download][type] = "file"
libraries[spyc][download][url] = "https://github.com/mustangostang/spyc/archive/0.6.2.zip"

libraries[respondjs][type] = "libraries"
libraries[respondjs][download][type] = "file"
libraries[respondjs][download][url] = "https://github.com/scottjehl/Respond/archive/1.3.0.zip"

libraries[responsiveslides][type] = "libraries"
libraries[responsiveslides][download][type] = "git"
libraries[responsiveslides][download][url] = "https://github.com/viljamis/ResponsiveSlides.js.git"
libraries[responsiveslides][download][revision] = "120079561d7a4f8a6459f7a5d8aa657ad5d3db83"
libraries[responsiveslides][patch][] = "https://gist.githubusercontent.com/RoySegall/412084926772e2e4181d/raw/8b98aa3d880ab40ac74a6343d74928a7a1667b7a/foo.patch"

libraries[flexslider][type] = "libraries"
libraries[flexslider][download][type] = "file"
libraries[flexslider][download][url] = "https://github.com/downloads/woothemes/FlexSlider/FlexSlider-2.0.zip"

libraries[select2][type] = "libraries"
libraries[select2][download][type] = "file"
libraries[select2][download][url] = "https://github.com/ivaynberg/select2/archive/3.4.3.zip"

libraries[twitter-api-php][type] = "libraries"
libraries[twitter-api-php][download][type] = "file"
libraries[twitter-api-php][download][url] = "https://github.com/J7mbo/twitter-api-php/archive/1.0.5.zip"

libraries[autopager][type] = "libraries"
libraries[autopager][download][type] = "file"
libraries[autopager][download][url] = "https://github.com/sagotsky/jquery-autopager/archive/v1.2.zip"
libraries[autopager][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/jquery-autopager_fix-load-prev-next.patch"

libraries[html5shiv][type] = "libraries"
libraries[html5shiv][download][type] = "file"
libraries[html5shiv][download][url] = "http://raw.github.com/aFarkas/html5shiv/master/dist/html5shiv.js"

libraries[hopscotch][type] = "libraries"
libraries[hopscotch][download][type] = "file"
libraries[hopscotch][download][url] = "https://github.com/linkedin/hopscotch/archive/b41ab659507175264ab6347d0032f03e42b961d1.zip"

libraries[jquery.bgiframe][type] = "libraries"
libraries[jquery.bgiframe][download][type] = "git"
libraries[jquery.bgiframe][download][url] = "https://github.com/brandonaaron/bgiframe.git"

; CKEDITOR plugins
libraries[colorbutton][type] = "libraries"
libraries[colorbutton][subdir] = "ckeditor/plugins"
libraries[colorbutton][download][type] = "file"
libraries[colorbutton][download][url] = "http://download.ckeditor.com/colorbutton/releases/colorbutton_4.5.8.zip"

libraries[image2][type] = "libraries"
libraries[image2][subdir] = "ckeditor/plugins"
libraries[image2][download][type] = "file"
libraries[image2][download][url] = "http://download.ckeditor.com/image2/releases/image2_4.5.11.zip"

libraries[panelbutton][type] = "libraries"
libraries[panelbutton][subdir] = "ckeditor/plugins"
libraries[panelbutton][download][type] = "file"
libraries[panelbutton][download][url] = "http://download.ckeditor.com/panelbutton/releases/panelbutton_4.5.8.zip"

libraries[mathjax][type] = "libraries"
libraries[mathjax][subdir] = "ckeditor/plugins"
libraries[mathjax][download][type] = "git"
libraries[mathjax][download][url] = "https://github.com/RoySegall/mathjax.git"
libraries[mathjax][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/mathjax-9530-cdn-path-update.patch"

libraries[justify][type] = "libraries"
libraries[justify][subdir] = "ckeditor/plugins"
libraries[justify][download][type] = "file"
libraries[justify][download][url] = "http://download.ckeditor.com/justify/releases/justify_4.5.8.zip"

libraries[font][type] = "libraries"
libraries[font][subdir] = "ckeditor/plugins"
libraries[font][download][type] = "file"
libraries[font][download][url] = "http://download.ckeditor.com/font/releases/font_4.5.8.zip"

libraries[lineutils][type] = "libraries"
libraries[lineutils][subdir] = "ckeditor/plugins"
libraries[lineutils][download][type] = "file"
libraries[lineutils][download][url] = "http://download.ckeditor.com/lineutils/releases/lineutils_4.5.9.zip"

libraries[widget][type] = "libraries"
libraries[widget][subdir] = "ckeditor/plugins"
libraries[widget][download][type] = "file"
libraries[widget][download][url] = "http://download.ckeditor.com/widget/releases/widget_4.5.9.zip"

libraries[colordialog][type] = "libraries"
libraries[colordialog][subdir] = "ckeditor/plugins"
libraries[colordialog][download][type] = "file"
libraries[colordialog][download][url] = "http://download.ckeditor.com/colordialog/releases/colordialog_4.5.10.zip"

libraries[indentblock][type] = "libraries"
libraries[indentblock][subdir] = "ckeditor/plugins"
libraries[indentblock][download][type] = "file"
libraries[indentblock][download][url] = "http://download.ckeditor.com/indentblock/releases/indentblock_4.5.10.zip"

libraries[bidi][type] = "libraries"
libraries[bidi][subdir] = "ckeditor/plugins"
libraries[bidi][download][type] = "file"
libraries[bidi][download][url] = "http://download.ckeditor.com/bidi/releases/bidi_4.5.10.zip"

libraries[dragresize][type] = "libraries"
libraries[dragresize][subdir] = "ckeditor/plugins"
libraries[dragresize][download][type] = "git"
libraries[dragresize][download][url] = "https://github.com/openscholar/ck-dragresize.git"

; Angular js libraries.
libraries[angular_select2][type] = "libraries"
libraries[angular_select2][download][type] = "get"
libraries[angular_select2][download][url] = "https://github.com/angular-ui/ui-select/archive/v0.12.0.zip"
libraries[angular_select2][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/angular_select-7024-async.patch"

libraries[angularSlideables][type] = "libraries"
libraries[angularSlideables][download][type] = "file"
libraries[angularSlideables][download][url] = "https://raw.githubusercontent.com/EricWVGG/AngularSlideables/master/angularSlideables.js"
libraries[angularSlideables][patch][] = "https://raw.githubusercontent.com/openscholar/openscholar-libraries/baed338248cc88fc174675b40fec6aba9c2dbe9a/d7patches/angularSlideables-wrap_file_in_jQuery_function_scope.patch"

; YUI js libraries
libraries[yui][type] = "libraries"
libraries[yui][download][type] = "file"
libraries[yui][download][url] = "https://github.com/yui/yui3/archive/v3.18.1.tar.gz"

libraries[jsonp-url][type] = "libraries"
libraries[jsonp-url][download][type] = "file"
libraries[jsonp-url][download][url] = "https://raw.githubusercontent.com/yui/yui3/v3.18.1/build/jsonp-url/jsonp-url-min.js"

libraries[oop][type] = "libraries"
libraries[oop][download][type] = "file"
libraries[oop][download][url] = "https://raw.githubusercontent.com/yui/yui3/v3.18.1/build/oop/oop-min.js"

libraries[jsonp][type] = "libraries"
libraries[jsonp][download][type] = "file"
libraries[jsonp][download][url] = "https://raw.githubusercontent.com/yui/yui3/v3.18.1/build/jsonp/jsonp-min.js"

libraries[google-api-php-client][type] = "libraries"
libraries[google-api-php-client][download][type] = "file"
libraries[google-api-php-client][download][url] = "https://github.com/google/google-api-php-client/releases/download/v2.2.2/google-api-php-client-2.2.2.zip"
