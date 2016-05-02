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
	
	// Create our data table out of JSON data loaded from server.
	var tempData = new google.visualization.DataTable(tempJsonData);
	var humidityData = new google.visualization.DataTable(humidityJsonData);
	var windData = new google.visualization.DataTable(windJsonData);
	var directionData = new google.visualization.DataTable(directionJsonData);
	var rainData = new google.visualization.DataTable(rainJsonData);

	var newWidth = $(window).width()/1.4;
	var newHeight = newWidth/2;
	
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
	
	// Instantiate and draw our chart, passing in some options.
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

$(window).smartresize(function () {
	initialiseCharts();
});