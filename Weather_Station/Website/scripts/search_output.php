<?php
	/* Search Output */
	
	/*
	* Retrieves all readings from the queried date entered in the search input box
	*/
	
	// Checks HTTP if submit is declared
	if(isset($_POST['submit'])){
		// Checks URL if search is declared
		if(isset($_GET['search'])){
			// Checks HTTP is date is declared
			if($_POST['date']){
				$date = $_POST['date']; // Set date from given
				$date = filter_var($date, FILTER_SANITIZE_STRING); // Filter the date
				
				require("db_connect.php"); // Connects to database
				
				// Database query
				$query = "SELECT date_time, inward_temperature, inward_humidity, wind_speed, wind_direction, rainfall
				FROM weather_reading
				WHERE DATE(date_time) = '$date'";
				
				// Result of query
				$result = mysqli_query($connection, $query) or die ("<p>Unable to proceed with selection query</p>");
				
				// Produce Results
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