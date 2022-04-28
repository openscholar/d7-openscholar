#!/usr/bin/env bash
set -e

# Modify the MySQL settings below so they will match your own.
MYSQL_USERNAME="root"
MYSQL_PASSWORD="password"
MYSQL_HOST="mysql"
MYSQL_DB_NAME="scholar"

# Modify the URL below to match your OpenScholar base domain URL.
BASE_DOMAIN_URL="http://localhost"

# Modify the login details below to be the desired login details for the Administrator account.
ADMIN_USERNAME="admin"
ADMIN_PASSWORD="admin"
ADMIN_EMAIL="admin@example.com"

echo -e "\n # Drupal running on this PHP version"
php -v

if [ -d "www/sites/default" ]; then
  chmod 777 www/sites/default
  rm -rf www/
  mkdir www
fi

bash scripts/build

cd www

drush si -y openscholar --locale=en --account-name=$ADMIN_USERNAME --account-pass=$ADMIN_PASSWORD --account-mail=$ADMIN_EMAIL --db-url=mysql://$MYSQL_USERNAME:$MYSQL_PASSWORD@$MYSQL_HOST/$MYSQL_DB_NAME --uri=$BASE_DOMAIN_URL openscholar_flavor_form.os_profile_flavor=development openscholar_install_type.os_profile_type=vsite
chmod 755 sites/default
chmod 755 sites/default/settings.php
echo "\$conf['mail_system'] = array('default-system' => 'DevelMailLog', 'mimemail' => 'MimeMailSystem');" >> sites/default/settings.php
drush vset purl_base_domain $BASE_DOMAIN_URL

# These commands migrates dummy content and is used for development and testing. Comment out both lines if you wish to have a clean OpenScholar installation.
drush en -y os_migrate_demo
echo -e "\n # Run migrates ..."
drush mi --all --user=1

echo -e "\n # Run extra captcha SQL ..."
# Disable captcha for forms that use behat tests
drush sql-query "INSERT INTO captcha_points (form_id, module, captcha_type) VALUES ('comment_node_blog_form', NULL, NULL);"
drush sql-query "INSERT INTO captcha_points (form_id, module, captcha_type) VALUES ('user_register_form', NULL, NULL);"
drush sql-query "UPDATE captcha_points SET captcha_type = NULL  WHERE form_id = 'registration_form';"

echo -e "\n # Get user login url: "
# This command does the login for you when the build script is done. It will open a new tab in your default browser and login to your project as the Administrator. Comment out this line if you do not want the login to happen automatically.
drush uli --uri=$BASE_DOMAIN_URL
drush updatedb -y

echo -e "\n # Finished drupal install"
if [ -d "www/sites/default" ]; then
  echo -e "\n # Directory exists, exit 0"
  exit 0
fi
