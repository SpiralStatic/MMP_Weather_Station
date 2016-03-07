<!DOCTYPE html>
<?php
	require ("db_connect.php");
?>
<html lang="en">

<head> <!-- HTML Header that contains information on the page -->
	<meta charset="UTF-8"/>
	<link rel="stylesheet" type="text/css" href="styling.css">
	<link rel="alternate" href="feed/rssFile.xml" title="MMP Weather Station" type="application/rss+xml" />
	<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
	<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
	<script type="text/javascript">
		google.charts.load('44', {'packages':['corechart']});
		google.charts.setOnLoadCallback(drawChart);

		function drawChart() {
			var jsonData = $.ajax({
				url: "graph.php",
				dataType: "json",
				async: false
			}).responseText;
          
			// Create our data table out of JSON data loaded from server.
			var data = new google.visualization.DataTable(jsonData);
			//console.log(jsonData);
			var options = {
				title: 'Temperatures',
				legend: { position: 'bottom' },
				hAxis: { title: 'Date' },
				vAxis: { title: 'Temperature °C'},
				width: 920,
				height: 540
			};
			
			// Instantiate and draw our chart, passing in some options.
			var chart = new google.visualization.LineChart(document.getElementById('temp_graph'));
			chart.draw(data, options);
		}
	</script>
	
	<title>Weather Prototype</title>
</head>

<body> <!-- HTML Body that contains site content -->
	<section>
		<h1>Weather Prototype Website</h1>
		<a href="feed/rss.php" class="feed"></a>
		<?php
			require("get_readings.php"); // Retrieve most recent data from get_readings.php
		?>
	</section>
	
	<!-- Create temperature graph -->
	<article>
		<div id="temp_graph"></div>
	</article>
</body>

</html>