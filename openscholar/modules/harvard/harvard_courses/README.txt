-- SUMMARY --

This module enable the creation of node of type 'courses' automatocally by
importing them using feeds module.

-- INSTALLATION --

* Enable module.
* Important - Clear cash on site - other wise you will receive a message about
missing fetcher.

-- FUNCTIONALITY --

On each Group site (deparment, personal or project) 2 new fields are added,
those field enable to add catalog number or deparment id used for querying the
courses API. Those fields are editable in the harvard courses app settings -
SITE_NAME/cp/build/features/harvard_courses.

On cron feeds module regroup all the deparment ids and catalog numbers from all
sites and import all those courses and add them as group content.
Each course is generated a single time and is updated automatically when the
origin has changed.

Note: Editing the course will reflect on all sites that are refering this
course.


-- HOW IS THE MODULE WORKING --

Upon installation 2 node of type 'Harvard API importer' are created
automatically, those are the actual importers:

  1. '/content/department-importer' - Is the importer of deparment courses, will
  import all the courses marked under the depatment IDs chossen in all sites.

  2. '/content/catalog-importer' - Is the importer for single course ids added
  in sites.

Example:

In a Deparment site, we click edit and add to the deparment ID field: 'COMPSCI'
and in the Catalog number field: 4949.

On cron the first importer will import all courses marked as COMPSCI, and the
course with the category number, 4949.


Parsing of course fields is based on thios article:
  https://manual.cs50.net/HarvardCourses_API








