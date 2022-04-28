(function () {
  window.onload = function () {
    var data = {
      url: window.location.href,
      width: document.body.clientWidth,
      height: document.body.clientHeight
    };

    window.parent.postMessage(JSON.stringify(data), "*");
  };
})();