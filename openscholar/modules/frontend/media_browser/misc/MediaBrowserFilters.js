(function() {

angular.module('mediaBrowser.filters', [])
  .filter('mbFilename', function () {
    return function (input, search) {
      if (input && search) {
        var results;
        search = search.toLowerCase();

        if (Array.isArray(input)) {
          results = [];
          for (var i=0; i<input.length; i++) {
            if (input[i].name.toLowerCase().indexOf(search) !== -1) {
              results.push(input[i]);
            }
          }
        }
        else if (typeof input == 'object') {
          results = {};
          for (var key in input) {
            if (input[key].name.toLowerCase().indexOf(search) !== -1) {
              results[key] = input[key];
            }
          }
        }
        else {
          return input;
        }

        return results;
      }
      return input;
    }
  })
  .filter('mbExtensions', function () {
    function testFile(file, extensions) {
      var filename = file.filename,
        ext = filename.slice(filename.lastIndexOf('.')+1);

      return !(file.schema == 'public' || file.schema == 'private') || (extensions.indexOf(ext) !== -1);
    }
    return function (input, extensions) {
      if (!extensions.length) {
        return input;
      }

      if (Array.isArray(input)) {
        results = [];
        for (var i=0;i<input.length;i++) {
          if (testFile(input[i], extensions)) {
            results.push(input[i]);
          }
        }
        return results;
      }
      else if (typeof input == 'object') {
        results = {};
        for (var k in input) {
          if (testFile(input[k], extensions)) {
            results[k] = input[k];
          }
        }
        return results;
      }
      return input;
    }
  })
  .filter('mbType', function () {
    function testFile(file, types) {
      return types.indexOf(file.type) != -1;
    }

    return function (input, types) {
      if (types == undefined || types.length == 0) {
        return input;
      }

      if (Array.isArray(input)) {
        results = [];
        for (var i=0; i<input.length; i++) {
          if (testFile(input[i], types)) {
            results.push(input[i]);
          }
        }
        return results;
      }
      else if (typeof input == 'object') {
        results = {};
        for (var k in input) {
          if (testFile(input[k], types)) {
            results[k] = input[k];
          }
        }
        return results;
      }
      return input;
    }
  });

})();