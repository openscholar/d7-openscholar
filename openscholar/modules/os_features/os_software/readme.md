# OS Software

Provides a parent content type *Software Project* which allows the creation of
child *Software Release* nodes. This way, one project lists a number of
downloadable packages.

## Rbuild

The Rbuild custom module depends on OS Software. It provides an alternative
field option that, when selected, checks for updates as a cron job and creates
*Software Release* nodes programmatically.