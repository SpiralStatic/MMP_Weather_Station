function periodSwitch(graph, period) {
	var graphPeriod = document.getElementById(graph);
	
	if(period == "month") {
		graphPeriod.setAttribute('data-period', 'month');
		document.getElementById(graph.toString()+'MonthTab').classList.add('active');
		document.getElementById(graph.toString()+'DayTab').classList.remove('active');
		document.getElementById(graph.toString()+'HourTab').classList.remove('active');
	}
	else if(period == "day") {
		graphPeriod.setAttribute('data-period', 'day');
		document.getElementById(graph.toString()+'MonthTab').classList.remove('active');
		document.getElementById(graph.toString()+'DayTab').classList.add('active');
		document.getElementById(graph.toString()+'HourTab').classList.remove('active');
	}
	else if(period == "hour") {
		graphPeriod.setAttribute('data-period', 'hour')
		document.getElementById(graph.toString()+'MonthTab').classList.remove('active');
		document.getElementById(graph.toString()+'DayTab').classList.remove('active');
		document.getElementById(graph.toString()+'HourTab').classList.add('active');
	}
}