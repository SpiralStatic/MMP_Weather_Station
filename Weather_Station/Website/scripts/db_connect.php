<?php
	$hostname = "db.dcs.aber.ac.uk";
	$username = "cjt6";
	$password = "Wigan1992";
	$database = "cs394_15_16_cjt6";

	// Create connection
	$connection = new mysqli($hostname, $username, $password, $database);

	// Check connection
	if ($connection->connect_error) {
		die("Connection failed: " . $connection->connect_error);
	} 
?>