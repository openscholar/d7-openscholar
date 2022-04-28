#!/usr/bin/env bash

echo -e "\n # Start services and run behat tests ..."
systemctl start httpd

chmod -R 777 /var/www/html/www/sites/default/files/

cd /opt/apache-solr/apache-solr-3.6.2/example/solr/conf
yes | cp /var/www/html/www/profiles/openscholar/modules/contrib/apachesolr/solr-conf/solr-3.x/* .
yes | cp /var/www/html/www/profiles/openscholar/behat/solr/solrconfig.xml .
cd /opt/apache-solr/apache-solr-3.6.2/example
java -jar start.jar &
sleep 10
cd /var/www/html/www
drush en os_search_solr -y
drush solr-mark-all
drush solr-index
drush vset oembedembedly_api_key ${EMBEDLYAPIKEY}
drush vset os_boxes_rss2json_api_key ${RSS2JSON_API_KEY}

cd /var/www/html/openscholar/behat
sh -c "echo 127.0.0.1  lincoln.local >> /etc/hosts"
sh -c "cat lincoln-vhost.txt > /etc/httpd/conf.d/lincoln.local.conf"
systemctl restart httpd

Xvfb :99 -ac &
THE_X_PID=$!
export DISPLAY=:99
sleep 5
# run the server
java -jar /opt/selenium-server-standalone.jar > /dev/null 2>&1 &
THE_S_PID=$!
sleep 10

if [ "${TEST_SUITE}" = 'restful' ]; then
  # Clear cache twice for restful
  cd /var/www/html/www
  echo -e "\n # GET api/blog/12 try1"
  wget localhost/api/blog/12
  drush cache-clear all
  echo -e "\n # GET api/blog/12 try2"
  wget localhost/api/blog/12
  drush cache-clear all
  echo -e "\n # GET api/blog/12 try3"
  wget localhost/api/blog/12
fi

cd /var/www/html/openscholar/behat
composer install
cp behat.local.yml.travis behat.local.yml
./bin/behat -V

# Run tests
echo -e "\n # Run tests"
./bin/behat --tags="${TEST_SUITE}" --strict

if [ $? -ne 0 ]; then
  echo "Behat failed"
  # kill Xvfb
  kill -15 ${THE_X_PID}
  # kill selenium
  kill -15 ${THE_S_PID}
  exit 1
fi

# kill Xvfb
kill -15 ${THE_X_PID}
# kill selenium
kill -15 ${THE_S_PID}
