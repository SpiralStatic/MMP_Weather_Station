/* Menu Button */
	
/*
* Adds an event listener to the navigation button to trigger
* showing the menu on and off
*/

var navToggle = document.getElementById('menuButton'), // Get the Button
body = document.body; // Get document body

// Adds the class .has_nav to the body when clicked
navToggle.addEventListener('click', function(e) {
	body.className = body.className? '' : 'has_nav';
	e.preventDefault();
});