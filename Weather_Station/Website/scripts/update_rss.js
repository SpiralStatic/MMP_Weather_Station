/* Update RSS */
	
/*
* Updates RSS Feed at certain intervals
*/

// The inteval to trigger the update
var intervalID = window.setInterval(updateRSS, 10000);

// Updates RSS Feed
function updateRSS() {
	$.ajax({
		type: "GET",
		url: "scripts/rss.php",
	})
	.done(function (msg) {
        //alert("RSS Feed Updated + msg");
     });
}