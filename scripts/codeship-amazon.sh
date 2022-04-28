#!/usr/bin/env bash
# Quick codeship script to push builds to a pair of acquia repos as new branches are made.

# builds all the composer files in root/sites
function buildComposer() {
    export ORIG=$(pwd)
    echo 'Begin pulling in site-specific code'
    cd $1
    GITDIR=$(find $1/$2 -type d -name 'gitdir')
    while read -r line; do
        ROOT=$(dirname $line);
        mv $ROOT/gitdir $ROOT/.git
    done <<< "$GITDIR"
    for site in $(ls openscholar/sites/); do
        cd openscholar/sites/$site
        echo "Installing site-specific modules for $site"
        composer install -n
        if ! composer install -n; then
          rm -r $1/$2/sites/$site/modules/
          composer install -n
        fi
        MODULE=$(composer show -s | sed -e '1,/requires/d' -e 's/ [^[:space:]]*$// ')
        cd $1/$2/sites/$site/modules/$MODULE
        git branch | grep -v "master" | xargs git branch -D
        cd -
        mv $1/$2/sites/$site/modules/$MODULE/.git $1/$2/sites/$site/modules/$MODULE/gitdir
#        if [ -d "$1/$2/sites/$site/custom" ]; then
#          ln -s ../custom $1/$2/sites/$site/modules/custom 2>&1>/dev/null
#        fi
        git add $1/$2/sites/$site
        cd $1
    done
    cd $ORIG
}

# pull down the acquia branch
mkdir -p ~/src/amazon/
git config --global user.email "openscholar@swap.lists.harvard.edu"
git config --global user.name "OpenScholar Auto Push Bot"

BUILD_ROOT='/home/rof/src/amazon'
DOCROOT='web';

if git show-ref -q --verify refs/tags/$CI_BRANCH 2>&1 > /dev/null; then
  # This is just a tag push
  # There's no need to build ever for tags
  # All we need to do it
  #export $BRANCH = $(git branch --contains tags/$CI_BRANCH | grep -s 'SCHOLAR-' | sed -n 2p)
  export TAG_COMMIT=$(git rev-list -n 1 $CI_BRANCH)
  git clone git@bitbucket.org:openscholar/deploysource.git
  cd deploysource
  export ROOT_COMMIT=$(git log --all --grep="git-subtree-split: $TAG_COMMIT" | grep "^commit" | sed "s/commit //" | head -n 1)
  if [ -z "$ROOT_COMMIT" ]; then
    exit 1
  fi
  git checkout $ROOT_COMMIT
  git tag $CI_BRANCH
  git push --tags
  exit 0
elif git ls-remote --heads git@bitbucket.org:openscholar/deploysource.git | grep -sw $CI_BRANCH 2>&1>/dev/null; then
  git clone -b $CI_BRANCH git@bitbucket.org:openscholar/deploysource.git  ~/src/amazon;
  cd ~/src/amazon
else
  git clone -b SCHOLAR-3.x git@bitbucket.org:openscholar/deploysource.git  ~/src/amazon;
  cd ~/src/amazon
  git checkout -b $CI_BRANCH;
fi

# Build this branch and push it to Amazon

# Set up global configuration and install tools needed to build
composer global require drush/drush:8.1.18
mkdir -p ~/.drush
printf "disable_functions =\nmemory_limit = 256M\ndate.timezone = \"America/New_York\"" > ~/.drush/php.ini
export PATH="$HOME/.composer/vendor/bin:$PATH"
drush --version || exit 1
npm install -g bower
npm install -g node-sass
export NODE_PATH=/home/rof/.nvm/v0.10.48/lib/node_modules

# Drush executable.
[[ $DRUSH && ${DRUSH-x} ]] || DRUSH=drush
cd $BUILD_ROOT
rm .gitmodules
#List of files from docroot that should be preserved
preserve_files=( .htaccess robots_disallow.txt sites 404_fast.html favicon.ico files )
#Backup the make files
cp -f openscholar/openscholar/drupal-org-core.make /tmp/
cp -f openscholar/openscholar/drupal-org.make /tmp/
cp -f openscholar/openscholar/bower.json /tmp/
git subtree pull -q -m "$CI_MESSAGE" --prefix=openscholar git@github.com:openscholar/openscholar.git $CI_BRANCH

#Only build if no build has ever happened, or if the make files have changed
if [ ! -d openscholar/openscholar/modules/contrib ] || [ $FORCE_REBUILD == "1" ] || [ "$(cmp -b 'openscholar/openscholar/drupal-org-core.make' '/tmp/drupal-org-core.make')" != "" ] || [ "$(cmp -b 'openscholar/openscholar/drupal-org.make' '/tmp/drupal-org.make')" != "" ] || [ "$(cmp -b 'openscholar/openscholar/bower.json' '/tmp/bower.json')" != "" ]; then
# Chores.
echo "Rebuilding..."
for DIR in $BUILD_ROOT/www-build $BUILD_ROOT/www-backup openscholar/openscholar/1 openscholar/openscholar/modules/contrib openscholar/openscholar/themes/contrib openscholar/openscholar/libraries; do
	rm -Rf $DIR &> /dev/null
done
cd openscholar/openscholar

$DRUSH make --no-core --contrib-destination drupal-org.make .
(
	# Download composer components
	composer install
	rm -rf libraries/git/symfony/event-dispatcher/.git
	rm -f libraries/git/symfony/event-dispatcher/.gitignore
	git rm -r --cached libraries/git/symfony/event-dispatcher
	rm -rf libraries/git/symfony/process/.git
	rm -f libraries/git/symfony/process/.gitignore
	git rm -r --cached libraries/git/symfony/process

	# Get the angular components
	bower -q install
)

cd ../../
$DRUSH make openscholar/openscholar/drupal-org-core.make $BUILD_ROOT/www-build

# Backup files from existing installation.
cd $BUILD_ROOT
ls
for BACKUP_FILE in "${preserve_files[@]}"; do
	rm -Rf www-build/$BACKUP_FILE
	mv $DOCROOT/$BACKUP_FILE www-build/
done
# Move the profile in place.
ln -s ../../openscholar/openscholar www-build/profiles/openscholar

# link up phpmyadmin
# ln -s ../phpMyAdmin-3.5.2.2-english $BUILD_ROOT/www-build/phpmyadmin

#link up js.php
ln -s ../openscholar/openscholar/modules/contrib/js/js.php $BUILD_ROOT/www-build/js.php

# Fix permissions before deleting.
# chmod -R +w $BUILD_ROOT/$DOCROOT/sites/* || true
rm -Rf $BUILD_ROOT/$DOCROOT || true

#remove install.php
rm -Rf $BUILD_ROOT/www-build/install.php || true

# Restore updated site.
mv $BUILD_ROOT/www-build $BUILD_ROOT/$DOCROOT
# Add New Files to repo and commit changes
git add --all $BUILD_ROOT/$DOCROOT
#Copy unmakable modules
cp -R openscholar/temporary/* openscholar/openscholar/modules/contrib/
# iCalcreator cannot be downloaded via make because a temporary token is needed,
# so we have the library inside os_events directory and we copy it to libraries.
cp -R openscholar/openscholar/modules/os_features/os_events/iCalcreator openscholar/openscholar/libraries/
# Download the git wrapper library using the composer.

for DIR in openscholar/openscholar/libraries openscholar/openscholar/themes/contrib openscholar/openscholar/modules/contrib
do
if [ -d "$DIR" ]; then
git add --all -f $DIR
fi
done

#remove automatic testing files and tools
ls $BUILD_ROOT/openscholar
rm -rf $BUILD_ROOT/openscholar/behat &> /dev/null

#pull in site-specific code
# Disable for now something ODDD is happening...
#buildComposer "$BUILD_ROOT" "$DOCROOT"
node "$BUILD_ROOT/openscholar/scripts/themes.js" "$DOCROOT"
git add web/sites
git commit -a -m "$CI_MESSAGE" -m "" -m "git-subtree-split: $CI_COMMIT_ID"
#END BUILD PROCESS
else

#remove automatic testing files and tools
ls $BUILD_ROOT/openscholar
rm -rf $BUILD_ROOT/openscholar/behat &> /dev/null

#Copy unmakable modules, when we don’t build
cp -R openscholar/temporary/* openscholar/openscholar/modules/contrib/

#pull in site-specific code
buildComposer "$BUILD_ROOT" "$DOCROOT"
node "$BUILD_ROOT/openscholar/scripts/themes.js" "$DOCROOT"
git add web/sites
git commit -a -m "$CI_MESSAGE" -m "" -m "git-subtree-split: $CI_COMMIT_ID" || git commit --amend -m "$CI_MESSAGE" -m "" -m "git-subtree-split: $CI_COMMIT_ID"
fi

git push origin $CI_BRANCH
echo -e "\033[1;36mFINISHED BUILDING $CI_BRANCH ON BITBUCKET\e[0m"
