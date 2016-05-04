/* Period Switch */
	
/*
* Switches the time period of the graph to month/day/hour depending on the tab clicked
*/

// Takes in the graph and period parameters
function periodSwitch(graph, period) {
	var graphPeriod = document.getElementById(graph); // Selects the graph
	
	// If clicked tab is month change
	if(period == "month") {
		graphPeriod.setAttribute('data-period', 'month');
		document.getElementById(graph.toString()+'MonthTab').classList.add('active');
		document.getElementById(graph.toString()+'DayTab').classList.remove('active');
		document.getElementById(graph.toString()+'HourTab').classList.remove('active');
	}
	// If clicked tab is day change
	else if(period == "day") {
		graphPeriod.setAttribute('data-period', 'day');
		document.getElementById(graph.toString()+'MonthTab').classList.remove('active');
		document.getElementById(graph.toString()+'DayTab').classList.add('active');
		document.getElementById(graph.toString()+'HourTab').classList.remove('active');
	}
	// If clicked tab is hour change
	else if(period == "hour") {
		graphPeriod.setAttribute('data-period', 'hour')
		document.getElementById(graph.toString()+'MonthTab').classList.remove('active');
		document.getElementById(graph.toString()+'DayTab').classList.remove('active');
		document.getElementById(graph.toString()+'HourTab').classList.add('active');
	}
}