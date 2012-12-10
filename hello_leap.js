var app = require('http').createServer(handler);
var io = require('socket.io').listen(app);
var fs = require('fs');
var leap = require('./build/Release/leap');

app.listen(8080);
console.log("Listening on http://localhost:8080");

function handler (req, res) {
  fs.readFile(__dirname + '/index.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }

    res.writeHead(200);
    res.end(data);
  });
}

io.sockets.on('connection', function (socket) {
    var listener = {
        onInit: function(){
            console.log("onInit");
        },

        onConnect: function(){
           console.log("onConnect");
        },

        onDisconnect: function(){
           console.log("onDisconnect");
        },

        onFrame: function(message){
            console.log("Got frame with message "+ message);
            socket.emit('frame-event', {
                "frame": message
            });
        }
    };

    console.log("About to create controller");
    var controller = new leap.Controller(listener);
    console.log("Created controller");

    socket.on('browser-event', function (data) {
        console.log(data);
    });

});
