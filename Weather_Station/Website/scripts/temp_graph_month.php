<?php
	/* Temperature Graph - Last Month */
	
	/*
	* Retrieves the temperature data for the last month to display in its chart
	*/
	
	require_once ("db_connect.php"); // Connects to database

	// Database query
	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH) AS TIME, DAY(date_time), AVG(outward_temperature), MIN(outward_temperature), MAX(outward_temperature)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 30 DAY)
				GROUP BY DAY(date_time) ORDER BY date_time ASC";
	
	// Result of query	
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array(); // Initialise array
	
	// Create chart columns
	$result_data['cols'] = array(
			array('label' => 'Time Stamp', 'type' => 'datetime'),
			array('label' => 'Average Temp', 'type' => 'number'),
			array('label' => 'Lowest Temp', 'type' => 'number'),
			array('label' => 'Highest Temp', 'type' => 'number')
		);
	
	// Fill each of the columns with the rows of data [JSON]
	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['TIME'];
		$date = DateTime::createFromFormat('Y-m-d H:i:s', $date_time);
		$formatted_date_time = $date->format('Y,m,d,H,i,s'); // Reformat date into accepted JSON date
		unset($date);
		
		$result_data['rows'][] = array( 'c' => array(
			array('v' => 'Date('. $formatted_date_time. ')'),
			array('v' => $row['AVG(outward_temperature)']),
			array('v' => $row['MIN(outward_temperature)']),
			array('v' => $row['MAX(outward_temperature)'])
		));
	}
	
	echo json_encode($result_data); // Encode as JSON
?>