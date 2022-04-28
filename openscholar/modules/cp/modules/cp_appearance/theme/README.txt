cp_appearance
==================================

cp_appearance provides an interface to the OpenScholar control panel for
selecting themes and theme flavors.

Using themes in cp_appearance
----------------------------------

To tell cp_appearance that your theme is usable as an OS site, its info file should include the following line:
  os[theme_type] = 'vsite'

### Privacy

@todo privacy/access

os[theme_access]

Flavors
-----------------------------------
flavors
plugins[os][flavor] = flavors //directory to flavors


Git library
-----------------------------------
The subtheme feature, which allow to a simple user upload flavor of a theme,
allow adding a flavor via a git repository. The cloning option need a php
library for that:

1. Go to profiles/openscholar/libraries
2. Clone the repository https://github.com/cpliakas/git-wrapper.git into the git
   or use:
    git clone https://github.com/cpliakas/git-wrapper.git git
3. From there you will need a composer, you can use the next two commands:
    curl -s https://getcomposer.org/installer | php
    php composer.phar install
