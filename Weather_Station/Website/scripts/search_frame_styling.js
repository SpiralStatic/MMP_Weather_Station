/* Search Frame Styling */
	
/*
* Modifies the CSS of the iframe in which the search resuls appear
*/

$(document).ready(function() {
	$('#searchFrame').load( function () {
		$(this).contents().find('body').css('font', '1.2em monospace');
		$(this).contents().find('section').css('display', 'inline-block');
		$(this).contents().find('section').css('padding-right', '1em');
		$(this).contents().find('section').css('padding-left', '1em');
		$(this).contents().find('section').css('padding-bottom', '1em');
	});
});