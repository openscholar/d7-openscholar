#!/bin/tcsh

### Begin OpenScholar file upload =======================
# @file
# @author Oren Robinson <oren_robinson@harvard.edu>
# @since 2013-01-02
#
# @section DESCRIPTION
#
# Runs CURL to upload a pdf file to http://example.com
# Only replaces (overwrites) the file if it already exists,
# otherwise does nothing.

set BASE_URL='http://example.com'
set PAGE='gking_upload_filefield'
set FILEPATH=`pwd`
set FILENAME="$1.pdf"
set AUTH='password123'
set LOG='gking_filefield_upload.log.html'

cd $FILEPATH

# Prepares the curl command
set CURL="curl -F filename=${FILENAME} -F contents=<${FILENAME} -F mimetype=application/pdf -F auth=${AUTH} -L -# --url ${BASE_URL}/${PAGE}"

# Dear developer: uncomment to have output go to log file.
# If you do this, you will not see a progress bar while the file uploads.
# set CURL="curl -F filename=${FILENAME} -F contents=<${FILENAME} -F mimetype=application/pdf -F auth=${AUTH} -L -# --url ${BASE_URL}/${PAGE} -o $LOG"

# Dear developer: uncomment to see executed command. 
# echo "${CURL}"

# Curls a URL with posted file data.
echo "Uploading ${FILENAME} to ${BASE_URL}..."
$CURL
