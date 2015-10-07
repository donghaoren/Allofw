// Make an object capable for accepting events.
// Binding behaviour is similar to d3.
// on("event:namespace", handler)
// raise("event", arguments, this)
var MakeEventSource = function (obj) {
    var handlers = {};
    obj.on = function (event, callback) {
        var e = event.split(".");
        if (!e[1]) e[1] = "";
        if (!handlers[e[0]]) handlers[e[0]] = {};
        handlers[e[0]][e[1]] = callback;
    };
    obj.raise = function (e) {
        if (!handlers[e]) return;
        for (var k in handlers[e]) {
            handlers[e][k].apply(null, Array.prototype.slice.call(arguments, 1));
        }
    };
};

exports.MakeEventSource = MakeEventSource;
