var Networking = function(config, role) {
    var self = this;
    this.broadcast = function() { };

    allofwutils.utils.makeEventSource(this);

    if(role == "renderer") {
        var sub = require("zmq").socket("sub");
        sub.connect(config.broadcasting.renderer.sub);
        sub.subscribe("");
        sub.on("message", function(msg) {
            try {
                var obj = JSON.parse(msg);
                self.raise.apply(this, [ obj[0] ].concat(obj[1]));
            } catch(e) {
                console.log(e.stack);
            }
        });
        console.log("Renderer: Listening on " + config.broadcasting.renderer.sub);
    }
    if(role == "simulator") {
        var pub = require("zmq").socket("pub");
        pub.bind(config.broadcasting.simulator.pub);
        console.log("Controller: Braodcasting on " + config.broadcasting.simulator.pub);
        this.broadcast = function(path) {
            try {
                var obj = [ path, Array.prototype.slice.call(arguments, 1) ];
                pub.send(JSON.stringify(obj));
            } catch(e) {
                console.log(e.stack);
            }
        };
    }
};

allofwutils.Networking = Networking;

function HTTPServer(config) {
    var self = this;
    this.handlers = { };

    var express = require("express");
    var app = express();
    var http = require('http').Server(app);
    var io = require('socket.io')(http);

    app.use(express.static(config.http.static));

    http.listen(config.http.port, function() {
        console.log("HTTPServer: Listening on port " + config.http.port);
    });

    this.sockets = new Set();

    io.on('connection', function(socket) {
        console.log("New connection: " + socket.id);
        self.sockets.add(socket);

        socket.on('disconnect', function() {
            console.log("Connection closed: " + socket.id);
            self.sockets.delete(socket);
        });

        socket.on('m', function(msg) {
            try {
                if(self.handlers[msg[0]]) {
                    self.handlers[msg[0]].apply(null, msg[1]);
                }
            } catch(e) {
                console.log(e.stack);
            }
        });
    });

    this.current_message_queue = [];
    setInterval(function() {
        if(self.current_message_queue.length > 0) {
            for(var item of self.sockets) {
                item.emit("ms", self.current_message_queue);
            }
        }
        self.current_message_queue = [];
    }, 200);
}

HTTPServer.prototype.broadcast = function(path) {
    this.current_message_queue.push([ path, Array.prototype.slice.call(arguments, 1) ]);
};

HTTPServer.prototype.on = function(event, handler) {
    this.handlers[event] = handler;
};

allofwutils.HTTPServer = HTTPServer;
