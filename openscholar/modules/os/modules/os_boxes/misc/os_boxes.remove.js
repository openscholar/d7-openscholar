
(function ($) {

Drupal.behaviors.osBoxesRemove = {
  attach: function (ctx) {
    $('li.link-count-widget-remove a').once('os-boxes-remove').click(function (e) {
      var widget = $(this).closest('.block');
      removeWidget(widget, this.search);
      $.ajax(this.href);
      e.preventDefault();
      e.stopPropagation();
    })
  }
};

var removed_widgets = {},
  template = 'This widget has been removed from this section. You can <a href="os/widget/{module}/{delta}/remove/{region}/{query}">undo this action</a>',
  error_template = 'This widget has been removed from this section. You must visit the <a href="cp/build/layout/{context}">Layout Form</a> to undo this action.';

function removeWidget(widget, query) {
  var id = widget.attr('id'),
      delta = widget.attr('delta'),
      region = findRegion(widget),
      module = widget.attr('module'),
      html = template;

  if (region) {
    html = html.replace('{delta}', delta);
    html = html.replace('{region}', region);
    html = html.replace('{query}', query);
    html = html.replace('{module}', module);

    removed_widgets[id] = widget.children().detach();
    widget.html(html);
    widget.find('a').click(function (e) {
      var widget = $(this).closest('.block');
      undoRemoveWidget(widget);
      $.ajax(this.href);
      e.preventDefault();
      e.stopPropagation();
    });
  }
  else {
    var matches = query.match(/=[^&]*&/);
    var context = matches[0].replace('=', '');
    html = error_template.replace('{context}', context);
    widget.html(html)
  }
}

function undoRemoveWidget(widget) {
  var id = widget.attr('id');

  widget.empty().append(removed_widgets[id]);
}

function findRegion(widget) {
  var region = widget.closest('.region, .nav'),
    classes = region.attr('class').split(' '),
    region_name = '';

  $.each(classes, function(i, v) {
    if (typeof v == 'string' && v.indexOf('region-') > -1) {
      region_name = v.replace('region-', '').replace('-', '_');
      // found what we need, break out
      // break out of the loop
      return false;
    }
  });

  return region_name;
}

})(jQuery);
