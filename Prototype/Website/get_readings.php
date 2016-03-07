<?php
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
		
		htmlspecialchars($wind_direction);
		
		echo "
			<section>
			<hr class=","bold",">
				<h1>Weather Reading:</h1>
				<hr>
				Date Time: <input class='datetime' type='text' name='datetime' value='$time_stamp' readonly>
				<hr>
				Temperatures: [1] <input class='temperatureone' type='text' name='temperatureone' value='$temperature_one"," C","' readonly>
				[2] <input class='temperaturetwo' type='text' name='temperaturetwo' value='$temperature_two"," C","' readonly>
				<hr>
				Humidities: [1]<input class='humidityone' type='text' name='humidityone' value='$humidity_one"," %","' readonly>
				[2] <input class='humiditytwo' type='text' name='humiditytwo' value='$humidity_two"," %","' readonly>
				<hr>
				Wind Speed: <input class='windspeed' type='text' name='windspeed' value='$wind_speed"," mph","' readonly>
				Wind Direction: <input class='winddirection' type='text' name='winddirection' value='$wind_direction' readonly>
				<hr>
				Rainfall: <input class='rainfall' type='text' name='rainfall' value='$rainfall"," mm","' readonly>
				<hr>
			</section>
		";
	}
?>