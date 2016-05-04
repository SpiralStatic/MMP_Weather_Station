<?php
	/* Wind Speed Graph - Last Month */
	
	/*
	* Retrieves the wind speed data for the last month to display in its chart
	*/
	
	require_once ("db_connect.php"); // Connects to database

	// Database query
	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH) AS TIME, DAY(date_time), AVG(wind_speed), MIN(wind_speed), MAX(wind_speed)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 1 MONTH)
				GROUP BY DAY(date_time) ORDER BY date_time ASC";
				
	// Result of query			
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array(); // Initialise array
	
	// Create chart columns
	$result_data['cols'] = array(
			array('label' => 'Time Stamp', 'type' => 'datetime'),
			array('label' => 'Average Wind Speed', 'type' => 'number'),
			array('label' => 'Lowest Wind Speed', 'type' => 'number'),
			array('label' => 'Highest Wind Speed', 'type' => 'number')
		);
		
	// Fill each of the columns with the rows of data [JSON]
	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['TIME'];
		$date = DateTime::createFromFormat('Y-m-d H:i:s', $date_time);
		$formatted_date_time = $date->format('Y,m,d,H,i,s'); // Reformat date into accepted JSON date
		unset($date);
		
		$result_data['rows'][] = array( 'c' => array(
			array('v' => 'Date('. $formatted_date_time. ')'),
			array('v' => $row['AVG(wind_speed)']),
			array('v' => $row['MIN(wind_speed)']),
			array('v' => $row['MAX(wind_speed)'])
		));
	}
	
	echo json_encode($result_data); // Encode as JSON
?>