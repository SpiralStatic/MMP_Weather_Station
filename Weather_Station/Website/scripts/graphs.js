/* Graphs */
	
/*
* Uses the google charts package to produce the weather graphs and
* charts from the database data
*/


// Loads the required packages for the google charts
google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(initialiseCharts);

// Retrieves the current tab period selection for a chart
function getTimePeriod(chart){
	var periodItem = document.getElementById(chart);
	return {
		aPeriod : periodItem.getAttribute('data-period')
	};
}

// Creates and draws the series of weather charts
function initialiseCharts() {
	var tempPeriod = getTimePeriod('tempGraph').aPeriod;
	var humidityPeriod = getTimePeriod('humidityGraph').aPeriod;
	var windPeriod = getTimePeriod('windGraph').aPeriod;
	var directionPeriod = getTimePeriod('directionGraph').aPeriod;
	var rainPeriod = getTimePeriod('rainGraph').aPeriod;
	
	var tempJsonData = $.ajax({
		url: "scripts/temp_graph_"+tempPeriod+".php",
		dataType: "json",
		async: false
	}).responseText;
  
  	var humidityJsonData = $.ajax({
		url: "scripts/humidity_graph_"+humidityPeriod+".php",
		dataType: "json",
		async: false
	}).responseText;
	
	var windJsonData = $.ajax({
		url: "scripts/wind_graph_"+windPeriod+".php",
		dataType: "json",
		async: false
	}).responseText;
	
	var directionJsonData = $.ajax({
		url: "scripts/direction_graph_"+directionPeriod+".php",
		dataType: "json",
		async: false
	}).responseText;
	
	var rainJsonData = $.ajax({
		url: "scripts/rain_graph_"+rainPeriod+".php",
		dataType: "json",
		async: false
	}).responseText;
	
	// Create the data table out of the loaded JSON data
	var tempData = new google.visualization.DataTable(tempJsonData);
	var humidityData = new google.visualization.DataTable(humidityJsonData);
	var windData = new google.visualization.DataTable(windJsonData);
	var directionData = new google.visualization.DataTable(directionJsonData);
	var rainData = new google.visualization.DataTable(rainJsonData);
	
	// Sets the charts height and width in relation to screen size
	var newWidth = $(window).width()/1.4;
	var newHeight = newWidth/2;
	
	// Describe the options for each chart
	var tempOptions = {
		title: 'Temperature',
		legend: { position: 'bottom' },
		vAxis: { title: 'Temperature °C'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	
	var humidityOptions = {
		title: 'Humidity',
		legend: { position: 'bottom' },
		vAxis: { title: 'Humidity %'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth	: 2,
		width: newWidth,
		height: newHeight
	};
	
	var windOptions = {
		title: 'Wind Speed',
		legend: { position: 'bottom' },
		vAxis: { title: 'Wind Speed m/s'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	
	var directionOptions = {
		title: 'Wind Direction',
		legend: { position: 'bottom' },
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		width: newWidth,
		height: newHeight
	};
	
	var rainOptions = {
		title: 'Rainfall',
		legend: { position: 'bottom' },
		vAxis: { title: 'Rainfall mm'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	
	// Instantiate and draw the chart, passing in some options.
	var tempChart = new google.visualization.LineChart(document.getElementById('tempGraph'));
	tempChart.draw(tempData, tempOptions);
	var humidityChart = new google.visualization.LineChart(document.getElementById('humidityGraph'));
	humidityChart.draw(humidityData, humidityOptions);
	var windChart = new google.visualization.LineChart(document.getElementById('windGraph'));
	windChart.draw(windData, windOptions);
	var directionChart = new google.visualization.PieChart(document.getElementById('directionGraph'));
	directionChart.draw(directionData, directionOptions);
	var rainChart = new google.visualization.LineChart(document.getElementById('rainGraph'));
	rainChart.draw(rainData, rainOptions);
}

// Uses smartresize.js to automatically resize on charts after screen resize
$(window).smartresize(function () {
	initialiseCharts();
});