<?php
	/* Wind Direction Graph - Last Day */
	
	/*
	* Retrieves the wind direction data for the last day to display in its chart
	*/
	
	require ("db_connect.php"); // Connects to database
	
	// Database query
	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH), Wind_Direction, COUNT(*)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 1 DAY)
				GROUP BY Wind_Direction";
				
	// Result of query
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array(); // Initialise array
	
	// Create chart columns
	$result_data['cols'] = array(
			array('label' => 'Wind Direction', 'type' => 'string'),
			array('label' => 'Direction Count', 'type' => 'number'),
		);
	
	// Fill each of the columns with the rows of data [JSON]
	while ($row = mysqli_fetch_assoc($result)) {	
		$result_data['rows'][] = array( 'c' => array(
		array('v' => $row['Wind_Direction']),
		array('v' => $row['COUNT(*)']),
		));
	}
	
	echo json_encode($result_data); // Encode as JSON
?>