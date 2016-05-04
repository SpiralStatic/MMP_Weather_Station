<?php
	/* Latest Readings */
	
	/*
	* Retrieves the latest weather reading and displays it in a <section>
	*/
	
	require("db_connect.php"); // Connects to database
	
	// Database query
	$query = "SELECT DISTINCT * 
	FROM weather_reading
	WHERE date_time = (SELECT MAX(date_time) FROM weather_reading ORDER BY date_time DESC)";
	
	// Result of query
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	// Produce Results
	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['date_time'];
		$temperature = round($row['outward_temperature']);
		$humidity = round($row['outward_humidity']);
		$wind_speed = round($row['wind_speed']);
		$wind_direction = $row['wind_direction'];
		$rainfall = round($row['rainfall']);
		
		htmlspecialchars($wind_direction);
		
		echo "
			<section id='latestReading'>
				<p>Date Time: <input class='reading' type='text' name='datetime' value='$date_time' readonly></p>
				<p>Temperature: <input class='reading' type='text' name='temperature' value='$temperature"," &deg;C","' readonly></p>
				<p>Humidity: <input class='reading' type='text' name='humidity' value='$humidity"," %","' readonly></p>
				<p>Wind Speed: <input class='reading' type='text' name='windspeed' value='$wind_speed"," m/s","' readonly></p>
				<p>Wind Direction: <input class='reading' type='text' name='winddirection' value='$wind_direction' readonly></p>
				<p>Rainfall: <input class='reading' type='text' name='rainfall' value='$rainfall"," mm","' readonly></p>
			</section>
		";
	}
?>