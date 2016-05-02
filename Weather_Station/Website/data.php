<!DOCTYPE html>
<html lang="en">

<head> <!-- HTML Header that contains information on the page -->
	<meta charset="UTF-8"/>
	<link rel="stylesheet" type="text/css" href="styling.css">
	<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
	<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
	<script type="text/javascript" src="scripts/smartresize.js"></script>
	<script type="text/javascript" src="scripts/graphs.js"></script>
	<script type="text/javascript" src="scripts/period_switch.js"></script>
	<script type="text/javascript" src="scripts/update_rss.js"></script>
	<title>Weather Station - Data</title>
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
		<h1>Weather Data</h1>
		<nav id="tempTabs">
			<ul class="tabLinks">
				<li id="tempGraphMonthTab" class="active"><a href="javascript:periodSwitch('tempGraph', 'month'); initialiseCharts();">Month</a></li>
				<li id="tempGraphDayTab"><a href="javascript:periodSwitch('tempGraph', 'day'); initialiseCharts();">Day</a></li>
				<li id="tempGraphHourTab"><a href="javascript:periodSwitch('tempGraph', 'hour'); initialiseCharts();">Hour</a></li>
			</ul>
		</nav>
		
		<div class="graph" id="tempGraph" data-period="month"></div>
		
		<nav id="humidityTabs">
			<ul class="tabLinks">
				<li id="humidityGraphMonthTab" class="active"><a href="javascript:periodSwitch('humidityGraph', 'month'); initialiseCharts();">Month</a></li>
				<li id="humidityGraphDayTab"><a href="javascript:periodSwitch('humidityGraph', 'day'); initialiseCharts();">Day</a></li>
				<li id="humidityGraphHourTab"><a href="javascript:periodSwitch('humidityGraph', 'hour'); initialiseCharts();">Hour</a></li>
			</ul>
		</nav>
		<div class="graph" id="humidityGraph" data-period="month"></div>
		
		<nav id="windTabs">
			<ul class="tabLinks">
				<li id="windGraphMonthTab" class="active"><a href="javascript:periodSwitch('windGraph', 'month'); initialiseCharts();">Month</a></li>
				<li id="windGraphDayTab"><a href="javascript:periodSwitch('windGraph', 'day'); initialiseCharts();">Day</a></li>
				<li id="windGraphHourTab"><a href="javascript:periodSwitch('windGraph', 'hour'); initialiseCharts();">Hour</a></li>
			</ul>
		</nav>
		<div class="graph" id="windGraph" data-period="month"></div>
		
		<nav id="directionTabs">
			<ul class="tabLinks">
				<li id="directionGraphMonthTab" class="active"><a href="javascript:periodSwitch('directionGraph', 'month'); initialiseCharts();">Month</a></li>
				<li id="directionGraphDayTab"><a href="javascript:periodSwitch('directionGraph', 'day'); initialiseCharts();">Day</a></li>
				<li id="directionGraphHourTab"><a href="javascript:periodSwitch('directionGraph', 'hour'); initialiseCharts();">Hour</a></li>
			</ul>
		</nav>
		<div class="graph" id="directionGraph" data-period="month"></div>
		
		<nav id="rainTabs">
			<ul class="tabLinks">
				<li id="rainGraphMonthTab" class="active"><a href="javascript:periodSwitch('rainGraph', 'month'); initialiseCharts();">Month</a></li>
				<li id="rainGraphDayTab"><a href="javascript:periodSwitch('rainGraph', 'day'); initialiseCharts();">Day</a></li>
				<li id="rainGraphHourTab"><a href="javascript:periodSwitch('rainGraph', 'hour'); initialiseCharts();">Hour</a></li>
			</ul>
		</nav>
		<div class="graph" id="rainGraph" data-period="month"></div>
		
	</section>
	
	<footer>
		<a href="rssFile.xml"><img src="images/rssIcon.svg" alt="Subscribe to the RSS Feeed" /></a>
	</footer>
</body>

</html>