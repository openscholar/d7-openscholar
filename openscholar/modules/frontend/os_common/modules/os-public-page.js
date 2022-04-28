(function ($) {

  var m = angular.module('os-public-page', ['FileEditorModal']);

  m.run(['$rootScope', 'FILEEDITOR_RESPONSES', function ($rs, FER) {
    $('li.link-count-file-edit a').each(function () {
      var $this = $(this),
        fid = this.href.match(/file\/([\d]*)\/edit/)[1];
      $this.attr({
        'file-editor-modal': '',
        'fid': fid,
        'on-close': 'reload($result)'
      });
    });

    $rs.reload = function (result) {
      if (result == FER.SAVED) {
        window.location.reload();
      }
    }
  }]);

})(jQuery);
