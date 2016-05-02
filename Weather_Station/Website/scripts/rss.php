<?php
    header("Content-Type: application/rss+xml; charset=UTF-8");
	require_once("db_connect.php");
	$rssFile = "../rssFile.xml";
	$editFile = fopen($rssFile, 'w') or die("can't open file");
	
	$rssFeed = '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
	$rssFeed .= '<rss version="2.0">' . "\n";
	$rssFeed .= '<channel>' . "\n";
	$rssFeed .= '<title>MMP Weather Station</title>' . "\n";
	$rssFeed .= '<link>localhost/website/home.php</link>' . "\n";
	$rssFeed .= '<description>Retrieves the most recent data from a weather station</description>' . "\n";
	$rssFeed .= '<language>en-gb</language>' . "\n";
	$rssFeed .= '<copyright>Copyright 2016 Chris Tsoi</copyright>' . "\n";
	
	$query = "SELECT DISTINCT * FROM weather_reading WHERE weather_reading.date_time = (SELECT MAX(date_time) FROM weather_reading ORDER BY date_time DESC)";
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");

	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['date_time'];
		$date_time = date("D, d M Y H:i:s T", strtotime($date_time));
		$temperature_two = $row['outward_temperature'];
		$humidity_two = $row['outward_humidity'];
		$wind_speed = $row['wind_speed'];
		$wind_direction = $row['wind_direction'];
		$rainfall = $row['rainfall'];
		
		$rssFeed .= '<item>' . "\n";
		$rssFeed .= '<title>' . "Latest Reading" . '</title>' . "\n";
		$rssFeed .= '<description>' . "\n";
		$rssFeed .= '[ Temperature: ' . $temperature_two . " ]\n";
		$rssFeed .= '[ Humidity: ' . $humidity_two . " ]\n";
        $rssFeed .= '[ Wind Speed:' . $wind_speed . " ]\n";
		$rssFeed .= '[ Wind Direction: ' . $wind_direction . " ]\n";
		$rssFeed .= '[ rainfall: ' . $rainfall . " ]\n";
		$rssFeed .= '</description>' . "\n";
		$rssFeed .= '<pubDate>'. $date_time . '</pubDate>';
        $rssFeed .= '</item>' . "\n";
		
	}
 
    $rssFeed .= '</channel>' . "\n";
    $rssFeed .= '</rss>' . "\n";
 
	fwrite($editFile, $rssFeed);
	fclose($editFile);
	
	header('Location: ../rssFile.xml') ;
?>