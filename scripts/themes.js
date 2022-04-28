const fs = require('fs'),
  path = require('path'),
  sass = require('node-sass');

var root = __dirname.replace(path.sep+'scripts', '');
var args = process.argv.slice(2);
var webroot = args[0];


/**
 * Collects all files in a directory, including descendents, that match a given pattern
 * @param startPath - the directory to search in
 * @param filter - the pattern of files to search for
 * @param callback - function to call when the list is complete
 */
function fromDir(startPath, filter, callback){

  //console.log('Starting from dir '+startPath+'/');

  if (!fs.existsSync(startPath)) {
    console.log("no dir ", startPath);
    return;
  }
  //console.log("Searching: "+startPath);
  var files = fs.readdirSync(startPath);
  var paths = [];
  for (var i=0 ;i<files.length; i++) {
    var filename = path.join(startPath,files[i]);
    var stat = fs.lstatSync(filename);
    if (stat.isDirectory()) {
      fromDir(filename, filter, function (p) {
        paths = paths.concat(p);
      }); //recurse
    }
    else if (filename.match(filter)) {
      paths.push(filename);
    };
  };

  callback(paths);
};

/**
 * Renders a sass file and its imports into css
 * @param directory - directory where all included files are, also where the output will go
 * @param source  - the source file to render into css
 */
function renderFile(directory, source) {
  sass.render({
    file: source,
    includePaths: [
      directory,
      __dirname
    ]
  }, function (err, result) {
    if (!err) {
      var base = path.basename(source, '.sass'),
        filename = directory + '/css/' + base + '_override.css';
      if (!fs.existsSync(directory + '/css')) {
        fs.mkdirSync(directory + '/css');
      }

      var flag = 'w';
      var fd = fs.open(filename, flag, parseInt('666', 8), function (err, fd) {
        if (!err) {
          fs.write(fd, result.css, 0, result.css.length, 0, function (err, bytesWritten, buffer) {
            if (err) {
              console.log(err);
            }
          });
        }
        else {
          console.error(err);
        }
      });
    }
    else {
      console.error(err);
    }
  })
}

/*
 * All of the base theme files import the _colors file. But they don't know where that file is
 * We can tell it which _colors file to use by setting includePaths to the directory of a site-specific module.
 * Sass will do its thing, put together those files, and give us the raw css that we can save to whatever file we want
 * In our case, it will be site-specific/css/theme_name.css
 */
fromDir(root+'/openscholar/themes/', /\.sass/, function (basePaths) {
  // basePaths has all the files in that include _colors, which
  if (basePaths.length == 0) {
    return;
  }

  fromDir(webroot+'/sites/', /\.sass/, function (overridePaths) {
    // overridePaths has all the _colors files. This is also where we'll be putting the fully built files
    for (var i = 0; i < overridePaths.length; i++) {
      var overrideDir = path.dirname(overridePaths[i]);
      for (var j = 0; j < basePaths.length; j++) {
        var baseFilename = path.basename(basePaths[j], '.sass');
        // function to force new scope
        renderFile(overrideDir, basePaths[j]);
      }
    }
  });
});
