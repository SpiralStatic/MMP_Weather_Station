<!DOCTYPE html>
<html lang="en">

<head> <!-- HTML Header that contains information on the page -->
	<meta charset="UTF-8"/>
	<link rel="stylesheet" type="text/css" href="styling.css">
	<link rel="alternate" href="rssFile.xml" title="MMP Weather Station" type="application/rss+xml" />
	<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
	<script type="text/javascript" src="scripts/search_frame_styling.js"></script>
	<script type="text/javascript" src="scripts/update_rss.js"></script>
	<title>Weather Station - Feed</title>
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
		<h1>Feed</h1>
		<p>To subscribe to the latest weather readings from Aberystwyth Weather Station, ensure
		you have a RSS compatible browser, if not download one of the many extensions and/or plugins
		available. Next. simply either click on the RSS icon at the bottom-right of the screen or use the
		browsers feature to subscribe to the feed. Happy Reading!
		
		<h1>Latest Reading</h1>
		<?php
			require("scripts/latest_readings.php");
		?>
	</section>
	
	<footer>
		<a href="rssFile.xml"><img src="images/rssIcon.svg" alt="Subscribe to the RSS Feeed" /></a>
	</footer>
</body>

</html>