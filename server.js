var express = require('express');
var bodyParser = require('body-parser');
var rest = require('restler');
var particleInfo = require('./particleInfo');

var app = express();
var urlencodedParser = bodyParser.urlencoded({ extended: false });

app.use(express.static('public', {'index': ['index.html', 'index.htm']}));

app.get('/temperature', function (req, res) {
    rest.get('https://api.particle.io/v1/devices/' + particleInfo.deviceID + '/temp?access_token=' + particleInfo.accessToken)
        .on('success', function(data) {
            res.status(200).send(JSON.parse(data.result).toString());
        }).on('fail', function(data, response){
        res.status(response.statusCode).send(data).end();
    });
});

app.post('/setLED', urlencodedParser, function (req, res) {
    var ledIdx = req.body.ledIdx;
    var ledR = req.body.ledR;
    var ledG = req.body.ledG;
    var ledB = req.body.ledB;

    var argString =  ledIdx + "," + ledR + "," + ledG + "," + ledB;

    rest.post('https://api.particle.io/v1/devices/' + particleInfo.deviceID + '/setLED', {
        data: { 'access_token': particleInfo.accessToken, 'args': argString}
    }).on('success', function(data) {
        res.status(200).send(JSON.parse(data.return_value).toString());
    }).on('fail', function(data, response){
        res.status(response.statusCode).send(data).end();
    });
});

var server = app.listen(3000, function () {
    var host = server.address().address;
    var port = server.address().port;
    console.log('Particle Example Server at http://%s:%s', host, port);
});