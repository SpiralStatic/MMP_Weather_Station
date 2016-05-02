google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(initialiseCharts);

function getTimePeriod(graph){
	var periodItem = document.getElementById(graph);
	return {
		aPeriod : periodItem.getAttribute('data-period')
	};
}

function initialiseCharts() {
	var tempPeriod = getTimePeriod('tempGraph').aPeriod;
	var humidityPeriod = getTimePeriod('humidityGraph').aPeriod;
	
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
	
	// Create our data table out of JSON data loaded from server.
	var tempData = new google.visualization.DataTable(tempJsonData);
	var humidityData = new google.visualization.DataTable(humidityJsonData);

	var newWidth = $(window).width()/1.4;
	var newHeight = newWidth/2;
	
	var tempOptions = {
		title: 'Outside Temperatures',
		legend: { position: 'bottom' },
		hAxis: { title: 'Time Period' },
		vAxis: { title: 'Temperature °C'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	
	var humidityOptions = {
		title: 'Outside Humidity',
		legend: { position: 'bottom' },
		hAxis: { title: 'Time Period' },
		vAxis: { title: 'Humidity'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	
	// Instantiate and draw our chart, passing in some options.
	var tempChart = new google.visualization.LineChart(document.getElementById('tempGraph'));
	tempChart.draw(tempData, tempOptions);
	var humidityChart = new google.visualization.LineChart(document.getElementById('humidityGraph'));
	humidityChart.draw(humidityData, humidityOptions);
}

$(window).smartresize(function () {
	initialiseCharts();
});