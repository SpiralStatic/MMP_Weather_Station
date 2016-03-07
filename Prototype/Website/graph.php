<?php
	require ("db_connect.php");
	$query = "SELECT Time_Stamp, Temperature_In, Temperature_Out FROM weather_reading ORDER BY Time_Stamp ASC";
	$result = mysqli_query($connection, $query) or die ("Unable to proceed with selection query");
	
	$result_data = array();

	$result_data['cols'] = array(
			array('label' => 'Time Stamp', 'type' => 'datetime'),
			array('label' => 'Inward Temperature', 'type' => 'number'),
			array('label' => 'Outward Temperature', 'type' => 'number')
		);
		
	while ($row = mysqli_fetch_assoc($result)) {
		$time_stamp = $row['Time_Stamp'];
		$date = DateTime::createFromFormat('Y-m-d H:i:s', $time_stamp);
		$formatted_time_stamp = $date->format('Y,m,d,H,i,s');
		unset($date);
		
		$result_data['rows'][] = array( 'c' => array(
			array('v' => 'Date('. $formatted_time_stamp. ')'),
			array('v' => $row['Temperature_In']),
			array('v' => $row['Temperature_Out'])
		));
	}
	
	echo json_encode($result_data);
?>