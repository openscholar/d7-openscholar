
		function getWindowHeight() {
			var windowHeight = 0;
			if (typeof(window.innerHeight) == 'number') {
				windowHeight = window.innerHeight;
			}
			else {
				if (document.documentElement && document.documentElement.clientHeight) {
					windowHeight = document.documentElement.clientHeight;
				}
				else {
					if (document.body && document.body.clientHeight) {
						windowHeight = document.body.clientHeight;
					}
				}
			}
			return windowHeight;
		}
		function setFooter() {
			if (document.getElementById) {
				var windowHeight = getWindowHeight();
				if (windowHeight > 0) {
					var headerHeight = document.getElementById('header-container').offsetHeight;
					var contentHeight = document.getElementById('columns').offsetHeight;
					var navHeight = document.getElementById('menu-bar').offsetHeight;
					var footerElement = document.getElementById('footer');
					var footerHeight  = footerElement.offsetHeight;
					if (windowHeight - (navHeight + headerHeight + contentHeight + footerHeight) >= 0) {
						footerElement.style.position = 'absolute';
						footerElement.style.top = (windowHeight - footerHeight) + 'px';
					}
					else {
						footerElement.style.position = 'static';
					}
				}
			}
		}
		window.onload = function() {
			setFooter();
		}
		window.onresize = function() {
			setFooter();
		}
	
		
//Insert the secondary nav to the content region for smartphone
 jQuery(window).resize(function() {
  if (jQuery(window).width() < 600) {
     jQuery(".region-header-third #block-os-secondary-menu").insertAfter(".region-sidebar-second");
  }

 });