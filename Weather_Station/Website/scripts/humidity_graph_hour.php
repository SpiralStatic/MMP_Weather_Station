<?php
	require ("db_connect.php");

	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH) AS TIME, HOUR(date_time), AVG(outward_humidity), MIN(outward_humidity), MAX(outward_humidity)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 1 HOUR)
				GROUP BY HOUR(date_time) ORDER BY date_time ASC";
				
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array();

	$result_data['cols'] = array(
			array('label' => 'Time Stamp', 'type' => 'datetime'),
			array('label' => 'Average Humidity', 'type' => 'number'),
			array('label' => 'Lowest Humidity', 'type' => 'number'),
			array('label' => 'Highest Humidity', 'type' => 'number')
		);
		
	while ($row = mysqli_fetch_assoc($result)) {
		$date_time = $row['TIME'];
		$date = DateTime::createFromFormat('Y-m-d H:i:s', $date_time);
		$formatted_date_time = $date->format('Y,m,d,H,i,s');
		unset($date);
		
		$result_data['rows'][] = array( 'c' => array(
			array('v' => 'Date('. $formatted_date_time. ')'),
			array('v' => $row['AVG(outward_humidity)']),
			array('v' => $row['MIN(outward_humidity)']),
			array('v' => $row['MAX(outward_humidity)'])
		));
	}
	
	echo json_encode($result_data);
?>