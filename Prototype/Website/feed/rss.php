<?php
    header("Content-Type: application/rss+xml; charset=UTF-8");
	require("../db_connect.php");
	$rssFile = "rssFile.xml";
	$editFile = fopen($rssFile, 'w') or die("can't open file");
	
	$rssFeed = '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
	$rssFeed .= '<rss version="2.0">' . "\n";
	$rssFeed .= '<channel>' . "\n";
	$rssFeed .= '<title>MMP Weather Station</title>' . "\n";
	$rssFeed .= '<link>http://192.168.1.64/website/weather</link>' . "\n";
	$rssFeed .= '<description>Retrieves the most recent data from a weather station</description>' . "\n";
	$rssFeed .= '<language>en-gb</language>' . "\n";
	$rssFeed .= '<copyright>Copyright 2016 Chris Tsoi</copyright>' . "\n";
	
	$query = "SELECT DISTINCT * FROM weather_reading WHERE weather_reading.Time_Stamp = (SELECT MAX(Time_Stamp) FROM weather_reading ORDER BY Time_Stamp DESC)";
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");

	while ($row = mysqli_fetch_assoc($result)) {
		$time_stamp = $row['Time_Stamp'];
		$temperature_one = round($row['Temperature_In']);
		$temperature_two = round($row['Temperature_Out']);
		$humidity_one = round($row['Humidity_In']);
		$humidity_two = round($row['Humidity_Out']);
		$wind_speed = round($row['Wind_Speed']);
		$wind_direction = $row['Wind_Direction'];
		$rainfall = round($row['Rainfall']);
		
		$rssFeed .= '<item>' . "\n";
		$rssFeed .= '<title>' . "Latest Reading" . '</title>' . "\n";
		$rssFeed .= '<description>' . "\n";
        $rssFeed .= '[ Inward Temperature: ' . $temperature_one . " ]\n";
		$rssFeed .= '[ Outward Temperature: ' . $temperature_two . " ]\n";
		$rssFeed .= '[ Inward Humidity: ' . $humidity_one . " ]\n";
		$rssFeed .= '[ Outward Humidity: ' . $humidity_two . " ]\n";
        $rssFeed .= '[ Wind Speed:' . $wind_speed . " ]\n";
		$rssFeed .= '[ Wind Direction: ' . $wind_direction . " ]\n";
		$rssFeed .= '[ Rainfall: ' . $rainfall . " ]\n";
		$rssFeed .= '</description>' . "\n";
		$rssFeed .= '<pubDate>'. $time_stamp . 'GMT</pubDate>';
        $rssFeed .= '</item>' . "\n";
		
	}
 
    $rssFeed .= '</channel>' . "\n";
    $rssFeed .= '</rss>' . "\n";
 
	fwrite($editFile, $rssFeed);
	fclose($editFile);
	
	header( 'Location: http://192.168.1.64/website/feed/rssFile.xml' ) ;
?>