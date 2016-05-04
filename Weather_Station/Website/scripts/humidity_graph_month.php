<?php
	/* Humidity Graph - Last Month */
	
	/*
	* Retrieves the humidity data for the last month to display in its chart
	*/
	
	require_once ("db_connect.php"); // Connects to database

	// Database query
	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH) AS TIME, DAY(date_time), AVG(outward_humidity), MIN(outward_humidity), MAX(outward_humidity)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 1 MONTH)
				GROUP BY DAY(date_time) ORDER BY date_time ASC";
				
	// Result of query			
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array(); // Initialise array

	// Create chart columns
	$result_data['cols'] = array(
			array('label' => 'Time Stamp', 'type' => 'datetime'),
			array('label' => 'Average Humidity', 'type' => 'number'),
			array('label' => 'Lowest Humidity', 'type' => 'number'),
			array('label' => 'Highest Humidity', 'type' => 'number')
		);
		
	// Fill each of the columns with the rows of data [JSON]
	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['TIME'];
		$date = DateTime::createFromFormat('Y-m-d H:i:s', $date_time);
		$formatted_date_time = $date->format('Y,m,d,H,i,s'); // Reformat date into accepted JSON date
		unset($date);
		
		$result_data['rows'][] = array( 'c' => array(
			array('v' => 'Date('. $formatted_date_time. ')'),
			array('v' => $row['AVG(outward_humidity)']),
			array('v' => $row['MIN(outward_humidity)']),
			array('v' => $row['MAX(outward_humidity)'])
		));
	}
	
	echo json_encode($result_data); // Encode as JSON
?>