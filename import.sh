#!/bin/bash

# Usage example:
# bash import.sh /tmp/backup-file.sql http://localhost/openscholar/www

cd www
drush sql-drop -y
echo "Importing SQL file."
`drush sql-connect` < $1
drush updb -y
drush fra -y
drush cc all
drush vrd -y --uri=$2
drush dl acquia_connector -y --uri=$2
drush en acquia_search -y
drush en dblog -y
drush en devel -y
drush en os_search_solr -y

# Remove existing Solr index.
drush php-eval "apachesolr_environment_delete('acquia_search_server_1');"

# Add a local Solr index.
drush php-eval "apachesolr_environment_save(array('env_id' => 'default', 'name' => 'default', 'url' => 'http://localhost:8983/solr', 'service_class' => ''));"

# Index Solr.
# @todo: Allow switch to index Solr.
# drush solr-mark-all; drush solr-index

# Remove traces for emails.
echo "Remove traces for emails."
drush sql-query "UPDATE users SET mail='foo@bar.com'"
drush sql-query "UPDATE field_data_field_email SET field_email_value='foo@bar.com'"
drush sql-query "UPDATE field_revision_field_email SET field_email_value='foo@bar.com'"
drush sql-query "UPDATE registration SET anon_mail='foo@bar.com'"

# Set teh file system
drush vset file_public_path "sites/default/files"
drush vset file_private_path ""
drush vset file_temporary_path "/tmp"

# Make sure admin is not blocked
drush sqlq "UPDATE users SET status=1 WHERE uid=1;"
drush sqlq "DELETE FROM flood;"

# Open site as admin.
drush uli --uri=$2

