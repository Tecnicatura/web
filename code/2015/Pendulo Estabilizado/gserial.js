'use strict';

var serialPort = require("serialport"); //load serial port library object
//var readline = serialPort.parsers.readline;     //parser-separator for data input
const Readline = serialPort.parsers.Readline;
var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');

app.listen(80);

function handler(req, res) {

  var errMgm = function(err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading static html');
    }

    res.writeHead(200);
    res.end(data);
  };

  console.log(req['url']);
  if (req['url'] === "/") {
    fs.readFile(__dirname + '/public/' + 'gsmooth.html', errMgm);
  } else {
    fs.readFile(__dirname + '/public/' + req['url'], errMgm);
  }
}


if (io) { //server created
  console.log('listening');
  console.log('Formato ejemplo:');
  console.log('{"t":0,"e":0,"c":0,"p":0,"r":1,"kp":1,"ki":0,"kd":0,"kp2":1,"ki2":0,"kd2":0,"cOn":1,"ds":1,"ps":700,"dm":5,"dl":600,"dt":5}');

  var sp = null;
  var parser;
  io.on('connection', function(socket) {

    if (sp === null || parser == null) {
      // sp = new serialPort("/dev/pts/7", {
      // sp = new serialPort("/dev/ttyS41", {
        sp = new serialPort("/dev/rfcomm0", {
        baud: 9600
      });
      parser = sp.pipe(new Readline({
        delimiter: '\r'
      }));

      sp.on("open", function() {
        console.log('open serial port');
      });

      sp.on('error', function(data) {
        console.log('error:' + data);
      });

    } else {
      console.log("ya no necesita abrir puerto");
    }
    parser.on('data', function(data) { //callback de recepción de datos
      socket.emit('newServerData', data);
      console.log('rx local: ' + data); //.toString('hex'));
    });
    socket.on('pedirPresets', function() {
      var presets = JSON.parse(fs.readFileSync(__dirname + "/presets.json", 'utf8'));
      socket.emit('valoresPreset', presets);
    });
    socket.on('guardarPresets', function(presets) {

      var content = JSON.stringify(presets);
      console.log("por aca pasa: "+content);
      fs.writeFile(__dirname + "/presets.json", content, function(err) {
        if (err) {
          return console.log(err);
        }

        console.log("The file was saved!");
      });
    });
    socket.on('clientData', function(clientData) {
      console.log("client: " + clientData);
      sp.write(clientData);
      sp.drain();
    });

  });
}

/*
//FORMA INCOMPRENSIBLE DE CREAR UN SERVER USANDO CANTIDAD MAXIMA DE LIBRERIAS
var express = require('express');
var appExpress = express();
var http = require('http');
var server = http.createServer(appExpress);
var io = require('socket.io').listen(server);
server.listen(80);

//FIN DE FORMA INCOMPRENSIBLE

appExpress.use(express.static('public'));


//ATIENDE METODO GET
appExpress.get('/', function (req, res) {
    res.sendFile(__dirname + '/public/' + 'gsmooth.html');
});
*/
