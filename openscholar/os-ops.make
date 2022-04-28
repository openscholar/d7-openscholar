; OPS MakeFile for OpenScholar
core = 7.x
api = 2

projects[ossap][type] = module
projects[ossap][download][type] = git
projects[ossap][download][url] = "git://github.com/openscholar/ossap.git"

projects[integrated_support][type] = module
projects[integrated_support][download][type] = git
projects[integrated_support][download][branch] = master
projects[integrated_support][download][url] = "https://github.com/openscholar/integrated_support.git"

; Libraries.
libraries[tapir][type] = "libraries"
libraries[tapir][download][type] = "git"
libraries[tapir][download][url] = "https://github.com/sagotsky/tapir.git"

libraries[pear][type] = "libraries"
libraries[pear][download][type] = "file"
libraries[pear][download][url] = "https://github.com/sagotsky/tapir/blob/master/pear-http-request2.tar.gz"

libraries[composer][type] = "libraries"
libraries[composer][download][type] = "file"
libraries[composer][download][url] = "http://github.com/openscholar/integrated_support/blob/master/libraries/composer_knplabs_github-api.tar.gz"

libraries[php-trello][type] = "libraries"
libraries[php-trello][download][type] = "file"
libraries[php-trello][download][url] = "http://bitbucket.org/mattzuba/php-trello/get/v1.1.1.zip"

libraries[php-markdown][type] = "libraries"
libraries[php-markdown][download][type] = "git"
libraries[php-markdown][download][tag] = "1.4.0"
libraries[php-markdown][download][url] = "https://github.com/michelf/php-markdown.git"
