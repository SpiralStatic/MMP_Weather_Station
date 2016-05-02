var navToggle = document.getElementById('menuButton'),
body = document.body;

navToggle.addEventListener('click', function(e) {
	body.className = body.className? '' : 'has_nav';
	e.preventDefault();
});