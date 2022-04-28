#!/bin/sh

# On cloud hosting environments it is not always an option to give a repec archive
# its own apache vhost.  Citation Distribute attempts to alleviate that by providing
# the `/repec/` page.  wgetting it (or hitting it with this script as a cron job) will
# copy it, resulting in a repec archive.

if [ $# -ne 2 ] ; then
  echo -e "\nUsage: citation_distribute_repec_cron.sh http://your-site.com/repec /path/to/repec/archive\n"
  exit 1
fi

if [ ! -w "$2" ] ; then
  echo -e "\nError: Directory '$2' is not writeable or not a directory.\n"
  exit 1
fi

TMP=$(mktemp -d)
wget -m "$1" -P "$TMP"

find $TMP -name index.html -delete
DIR=$(find $TMP -name repec);
for file in $(ls $DIR) ; do
  if [[ -d "$DIR/$file" ]] ; then
    mv -f "$DIR/$file/"* "$2/$file/"
  else 
    mv -f "$DIR/$file" "$2/"
  fi
done

find "$2" -name "*.rdf" | while read file ; do
  sed -i 's/<br \/>//' "$file" # clean up line breaks
done

find "$2" -name "*.rdf" -size 0 -delete # clean empty files - these happen when a node is unshared

# repec won't let us use anything but wpaper.  rename all these otherwise correct templates...
find "$2" -name "*.rdf" | while read file ; do
  sed -i 's/Template-Type: ReDIF-Article 1.0/Template-Type: ReDIF-Paper 1.0/' $file
  sed -i '/^Journal/d' $file
  sed -i '/^Year/d' $file
  sed -i '/^Publisher/d' $file
  sed -i '/^Volume/d' $file
  sed -i '/^Pages/d' $file
  sed -i '/^Issue/d' $file
  sed -i '/^Edition/d' $file

# change handles to wpaper
  sed -i 's/^Handle: RePEc:qsh:journl:/Handle: RePEc:qsh:wpaper:/' $file 
  sed -i 's/^Handle: RePEc:qsh:bookch:/Handle: RePEc:qsh:wpaper:/' $file 

  sed -i 's/Template-Type: ReDIF-Chapter 1.0/Template-Type: ReDIF-Paper 1.0/' $file

  sed -i 's/Template-Type: ReDIF-Book 1.0/Template-Type: ReDIF-Paper 1.0/' $file
  sed -i 's/^Editor-Name\(.*\)/Author-Name\1/' $file
  sed -i '/^Publication-Status/d' $file

  # these appear in one paper and look like repec attributes
  sed -i 's/^Methods?:/ Methods:/i' $file
  sed -i 's/^Results?:/ Results:/i' $file
  sed -i 's/^Conclusions?:/ Conclusion:/i' $file
  sed -i 's/^Designs?:/ Design:/i' $file
  sed -i 's/^Settings?:/ Setting:/i' $file
  sed -i 's/^Participants?:/ Participants:/i' $file

  sed -i 's/node\//\/node\//' $file # this is a new error... 
done
rm -rf $TMP
