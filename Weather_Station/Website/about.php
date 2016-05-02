<!DOCTYPE html>
<html lang="en">

<head> <!-- HTML Header that contains information on the page -->
	<meta charset="UTF-8"/>
	<link rel="stylesheet" type="text/css" href="styling.css">
	<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
	<script type="text/javascript" src="scripts/search_frame_styling.js"></script>
	<script type="text/javascript" src="scripts/update_rss.js"></script>
	<title>Weather Station - About</title>
</head>

<body> <!-- HTML Body that contains site content -->
	
	<header>
		<a href="home.php" id="logoLink">
			<img id="logo" src="images/weatherLogo.svg" alt="Weather Station Logo"
					onerror="this.onerror=null; this.src='images/weatherLogo.png'"></a>
					
		<input type="button" id="menuButton">
		<script type="text/javascript" src="scripts/menubutton.js"></script>
	</header>
	
	<nav class="menu">
		<ul id="menuItems">
			<a class="menuLink" href="home.php"><li>HOME</li></a>
			<a class="menuLink" href="data.php"><li>DATA</li></a>
			<a class="menuLink" href="feed.php"><li>FEED</li></a>
			<a class="menuLink" href="about.php"><li>ABOUT</li></a>
		</ul>
	</nav>
	
	<section>
		<h1>About</h1>
		<p>This website was created alongside the building of a weather station in completion of a Computer Science Major
		Project at Aberystwyth University. The aim of this was to produce a site that meets the required functions 
		of representing the weather readings in both a useful yet visually pleasing way.</p>
		<img id="aberystwythMap" src="images/aberystwythMap.png" alt="Aberystwyth Map">
	</section>
	
	<footer>
		<a href="rssFile.xml"><img src="images/rssIcon.svg" alt="Subscribe to the RSS Feeed" /></a>
	</footer>
</body>

</html>