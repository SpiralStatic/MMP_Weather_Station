<?php
	$hostname = "localhost";
	$username = "******";
	$password = "******";
	$database = "weather_prototype";

	// Create connection
	$connection = new mysqli($hostname, $username, $password, $database);

	// Check connection
	if ($connection->connect_error) {
		die("Connection failed: " . $connection->connect_error);
	} 
?>