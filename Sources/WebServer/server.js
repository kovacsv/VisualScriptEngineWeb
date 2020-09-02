var path = require ('path');
var express = require ('express');
var app = express ();

app.use (express.static (path.join (__dirname, 'public')));
app.get  ('/request', function (req, res) {
	res.send ('response');
});

app.listen (8080);
