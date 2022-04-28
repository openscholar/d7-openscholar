# GK+

Customizations for Gary King's website.

# Overview

Historically, Gary King's site was one of the first OpenScholar sites, and due
to Gary's close involvement and leadership in the OpenScholar project, Gary's
personal site became a test-bed for more experimental and fancy features.

See also the gking theme, where a good deal of custom javascript effects are.

This module provides a number of miscellaneous customizations:

* Alter gary's "people" page to display all person nodes and remove the pager
* Allows nodes of type 'page' to not have titles if they are in the research-interest taxonomy
* Add checkbox on nodes toggling 'minimal' theme when viewing that node
* Add checkbox on terms toggling header hidden from anon users, and disabled to auth users
* Add checkbox on terms toggling whether or not to indent term when displayed as child
* Overrides existing taxonomy theme function defaults to display grandchild terms
* Displays nodeorder link to admin on taxonomy terms (using the the nodeorder module)
* Cron looks for files in sites/default/files/gking/tmpdir, and attempts to copy
  new versions of an existing attachment (automatically updating the filefield).

This feature has been rejected for migration:

* Displays term links like Parent (Child) when listed on nodes.

These components have been moved to other modules in SCHOLAR-3-0:

* Adds "Edit annotation" links to node contextual links (by Jeffrey); moved to
  contextual_annotation module.
* Ensure term links use the aliased path when listing term links.

# Setup

This module assumes specific content and configurations:
1. A vsite named 'gking'
2. All dependencies enabled (nodeorder, contextual_annotation)
3. Change the active theme to gking
4. Create a vocabulary named 'Research Interests' with "Sortable" selected
5. Terms, child terms, and grandchild terms in the 'Research Interests' vocab
