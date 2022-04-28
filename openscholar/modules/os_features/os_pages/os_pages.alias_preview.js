/**
 * Enhances node path alias entry with automatic preview on forms.
 */
(function ($) {
    // Stores behaviors as a property of Drupal.behaviors.
    Drupal.behaviors.alias_preview = {
        attach: function (context, settings) {
            $(document).ready(function () {
                // On node edit forms only, creates a static "Link URL" preview.
                // Finds any existing alias value,
                // then injects it into the title field description.
                if ($('#edit-path-alias').length) {
                    var alias = $('#edit-path-alias').val();
                    if (alias.length) {
                        var base_url = Drupal.settings.alias_preview.prefix;
                        var description = '<strong>URL:</strong> ' + base_url + '/' + alias + ' <a id="pathauto-extra-edit-path" href="#path[pathauto]">edit</a>';
                        alias_preview_description_init();
                        $('.form-item-title .description').html(description);
                        alias_preview_scroll();
                    }
                }

                // On node add forms, use AJAX callback to generate alias preview.
                $('.page-node-add #edit-title').change(function () {
                    alias_preview_description_init();
                    alias_preview_ajax_handler();
                });

                /**
                 * Provides a smooth animation when the user clicks "edit"
                 */
                function alias_preview_scroll() {
                    // Brings the user to the custom alias field with proper settings checked.
                    $('.form-item-title .description a').click(function () {
                        $('html, body', context).animate({ scrollTop: $('#edit-actions-top').offset().top });
                        $('#edit-path .fieldset-title', context).click();
                        $('#edit-path-pathauto').click().attr('checked', false).attr('value', 0);
                        $('.form-item-path-alias').removeClass('form-disabled');
                        $('#edit-path-alias').removeAttr("disabled").focus().select();
                    });
                }

                /**
                 * Adds the AJAX handler to generate pathauto aliases for preview.
                 */
                function alias_preview_ajax_handler() {
                    // Verifies settings before continuing...
                    if (Drupal.settings.alias_preview && Drupal.settings.alias_preview.make_alias) {
                        // Prepares the ajax callback URL to query.
                        var href = Drupal.settings.alias_preview.url;
                        // Prepares the URL parameters to call for this node.
                        var data = Drupal.settings.alias_preview;
                        // Cleans the title value stored in our URL params.
                        data.title = $.trim($('#edit-title').attr('value'));

                        // Only proceeds if there is a title value, and
                        // if the "Generate automatic alias" setting is checked.
                        if (data.title.length > 0 && $('#edit-path-pathauto').attr('checked')) {
                            // Adds autocomplete-style default Drupal throbber.
                            $('.form-item-title .description').html('<strong>Link URL:</strong> <span class="ajax-progress"><span class="throbber"></span></span>');
                            // Fetches the generated alias from the menu callback.
                            $.getJSON(href, data, function (json) {
                                // If we got a successful AJAX response...
                                if (json.status) {

                                    // Prepares the alias, removing purl.
                                    var alias = json.data;
                                    var base_url = Drupal.settings.alias_preview.prefix;
                                    var purl = Drupal.settings.alias_preview.purl;
                                    if (purl.length) {
                                      alias = alias.slice(purl.length + 1);
                                    }

                                    // Updates the existing pathauto alias field with the new value.
                                    $('#edit-path-alias').attr('value', alias);
                                    // Updates the title input field description to immediately show user.
                                    var description = '<strong>Link URL:</strong> ' + base_url + '/' + alias + ' <a id="pathauto-extra-edit-path" href="#path[pathauto]">edit</a>';
                                    $('.form-item-title .description').html(description);

                                    alias_preview_scroll();
                                }
                            });
                        }
                    }
                }

                /**
                 * Ensures that the description <div> exists below the title.
                 *
                 * It won't exist at first when the page loads.
                 */
                function alias_preview_description_init() {
                    if (! $('.form-item-title .description').length) {
                        $('<div class="description"></div>').insertAfter('#edit-title');
                    }
                }
            });
        }
    };
}(jQuery));
