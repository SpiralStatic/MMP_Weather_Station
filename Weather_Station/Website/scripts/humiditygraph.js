google.charts.setOnLoadCallback(initialiseHumidityChart);

function getScriptPeriod(){
	var script = document.querySelector('script[data-period]');
	return {
		aPeriod : script.getAttribute('data-period')
	};
}

function initialiseHumidityChart() {
	var period = getScriptPeriod().aPeriod;
	var jsonData = $.ajax({
		url: "scripts/humidity_graph_"+period+".php",
		dataType: "json",
		async: false
	}).responseText;
  
	// Create our data table out of JSON data loaded from server.
	var data = new google.visualization.DataTable(jsonData);
	//console.log(jsonData);
	var newWidth = $(window).width()/1.4;
	var newHeight = newWidth/2;
	var options = {
		title: 'Outside Humidity',
		legend: { position: 'bottom' },
		hAxis: { title: 'Time Period' },
		vAxis: { title: 'Humidity'},
		colors: ['#639bc9', '#99dad8', '#ffc600'],
		lineWidth: 2,
		width: newWidth,
		height: newHeight
	};
	drawChart(data, options);
}

function drawChart(data, options) {
	// Instantiate and draw our chart, passing in some options.
	var chart = new google.visualization.LineChart(document.getElementById('humidityGraph'));
	chart.draw(data, options);
}

$(window).smartresize(function () {
	initialiseHumidityChart();
});