var intervalID = window.setInterval(updateRSS, 10000);

function updateRSS() {
	$.ajax({
		type: "GET",
		url: "scripts/rss.php",
	})
	.done(function (msg) {
        //alert("RSS Feed Updated + msg");
     });
}