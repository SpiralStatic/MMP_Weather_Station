<?php
	require ("db_connect.php");

	$query = "SELECT DATE_SUB(date_time, INTERVAL 1 MONTH), Wind_Direction, COUNT(*)
				FROM weather_reading WHERE date_time >= DATE_SUB(NOW(), INTERVAL 1 HOUR)
				GROUP BY Wind_Direction";
				
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array();

	$result_data['cols'] = array(
			array('label' => 'Wind Direction', 'type' => 'string'),
			array('label' => 'Direction Count', 'type' => 'number'),
		);
		
	while ($row = mysqli_fetch_assoc($result)) {	
		$result_data['rows'][] = array( 'c' => array(
		array('v' => $row['Wind_Direction']),
		array('v' => $row['COUNT(*)']),
		));
	}
	
	echo json_encode($result_data);
?>