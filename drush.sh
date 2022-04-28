#!/usr/bin/env bash

PROJECT_DIR=$( cd "$(dirname "$0")" ; pwd -P )

DEST=$1

# Let the first character be an @, to allow for un-trainable muscle memory and old scripts.
if [[ $DEST == @* ]]; then
	DEST=${DEST:1}
fi

DOCKER_COMPOSE="$PROJECT_DIR/docker/$DEST/docker-compose.yml";
if [ ! -e "$DOCKER_COMPOSE" ]; then
  echo "**ERROR: $DOCKER_COMPOSE does not exist.";
  exit 1;
fi

# Remove first argument.
shift;

# Run drush command through docker.
DRUSH_COMMAND="docker-compose -f ${DOCKER_COMPOSE} exec --user=82 php drush -r www/ $@";
$DRUSH_COMMAND;
