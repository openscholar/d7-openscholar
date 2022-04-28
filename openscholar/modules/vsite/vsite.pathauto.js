/**
 * Dynamically validates custom alias input values on node edit/add forms.
 */
(function ($) {
    // Stores behaviors as a property of Drupal.behaviors.
    Drupal.behaviors.vsite_pathauto = {
        attach: function (context, settings) {
            $(document).ready(function () {
                // On both node add and node edit forms, dynamically validates.
                // If a user enters a leading or trailing slash,
                // remove it when the input blurs, and display a message.
                $('#edit-path-alias').blur(function () {
                    var path = $('#edit-path-alias').attr('value');

                    // Strips leading slash
                    if (path.indexOf('/') == 0) {
                        path = path.replace(/^\/+/, '');
                        vsite_pathauto_warning('URL alias may not begin with a slash.');
                    }

                    // Strips trailing slash
                    if (path.substr(-1) == '/') {
                        path = path.replace(/\/+$/, '');
                        vsite_pathauto_warning('URL alias may not end with a slash.');
                    }

                    // Replaces the invalid value with the cleaned value.
                    $('#edit-path-alias').attr('value', path);
                });

                // Removes any previous warning messages when new value is input.
                $('#edit-path-alias').change(function () {
                    $('div.pathauto_extra-warning').remove();
                });

                // Displays a small warning to the user that the original input
                // was invalid and has been replaced, explaining why.
                function vsite_pathauto_warning(str) {
                    // Inserts the warning at the end of the existing description, on it's own line.
                    $('#edit-path-alias + div.description').after('<div class="description pathauto_extra-warning">' + str + '</div>');
                    // Triggers change() event in order to update vertical tabs preview text.
                    $('#edit-path-alias').focus();
                }
            });
        }

    }
}(jQuery));