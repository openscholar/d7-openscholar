/**
 * Generic helper functions that more than one module will need.
 * Usually to make up for some idiosyncrasy of the platform.
 */
(function (w) {

  w.osCommonHelpers = {
    findLibraryPath: findLibraryPath,
    findDomain: findDomain,
    addDependency: addDependency
  };

  /**
   * Finds the root path of the library.
   * Some libraries include other assets that are not necessarily js files. Images, templates, etc.
   * This will find the root path of the library so those assets can be referenced.
   */
  function findLibraryPath(name) {
    var s = window.document.scripts,
      i = 0;

    for (;i < s.length; i++) {
      if (s[i].src.indexOf(name) !== -1) {
        var path = s[i].src,
          pos = path.lastIndexOf('/');

        return path.slice(0, pos);
      }
    }
  }

  /**
   *  Given a path, finds the root domain of it.
   *  Needed to whitelist external asset domains
   */
  function findDomain(path) {
    var parser = document.createElement('a');
    parser.href = path;

    return parser.protocol+'//'+parser.hostname;
  }

  /**
   * Adds a dependency for a module. Use this to allow other modules to hook themselves into other modules without
   * constantly updating the target module.
   */
  var deps = {};
  function addDependency(module, dependency) {
    if (typeof dependency != 'undefined') {
      deps[module] = deps[module] || [];
      deps[module].push(dependency);
    }

    return deps[module];
  }



  var m = angular.module('osHelpers', []);


  /**
   * Takes a string and cleans it for use in an id attribute.
   */
  m.filter('idClean', function () {
    return function (input, prefix) {
      if (input) {
        input = input.replace(/_/g, '-');
        input = input.replace(/\s/g, '-');
        input = input.replace(/--/g, '-');

        if (prefix) {
          return prefix + '-' + input;
        }
        else {
          return input;
        }
      }
    };
  });

})(window);