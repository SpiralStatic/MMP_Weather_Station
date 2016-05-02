<?php
	if(isset($_POST['submit'])){
		if(isset($_GET['search'])){
			if($_POST['date']){
				$date = $_POST['date'];;
				$date= filter_var($date, FILTER_SANITIZE_STRING);
				// Connect  to the database
				require("db_connect.php");
				// Database query
				$query = "SELECT date_time, inward_temperature, inward_humidity, wind_speed, wind_direction, rainfall
				FROM weather_reading
				WHERE DATE(date_time) = '$date'";
				//echo $query; // Debugging
				$result = mysqli_query($connection, $query) or die ("<p>Unable to proceed with selection query</p>");
				
				//Produce Results
				while ($row = mysqli_fetch_assoc($result)) {
					echo "<section><p>Date Time: [" . $row['date_time'] . "]</p>";
					echo "<p>Temperature: [" . $row['inward_temperature'] . " &degC]</p>";
					echo "<p>Humidity: [" . $row['inward_humidity'] . " %]</p>";
					echo "<p>Wind Speed: [" . $row['wind_speed'] . " m/s]</p>";
					echo "<p>Wind Direction: [" . $row['wind_direction'] . "]</p>";
					echo "<p>rainfall: [" . $row['rainfall'] . " mm]</p></section>";
				}
			}
			else{
				echo "<p>Please enter a valid search query</p>";
			}
		}
	}
?>