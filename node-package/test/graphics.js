var allofw = require("allofw");

var render = function(context) {
    var paint = context.paint();

    context.clear(255, 255, 255, 1);
    context.drawLine(0, 0, 1000, 1000, paint);
    paint.setTypeface("Arial", allofw.FONTSTYLE_NORMAL);
    paint.setTextSize(120);
    paint.setTextAlign(allofw.TEXTALIGN_CENTER);
    paint.setMode(allofw.PAINTMODE_FILL);
    context.drawText("Hello World", 500, 500, paint);
    paint.setTextSize(60);
    context.drawText("This is Node-Graphics (Skia Backend)", 500, 700, paint);
};

var s = new allofw.Surface2D(1000, 1000, allofw.SURFACETYPE_RASTER);
var context = new allofw.GraphicalContext2D(s);
render(context);


var request = require('request');
request({ url: 'http://www.baidu.com/img/bdlogo.png', encoding: null }, function(error, response, body) {
    var img = new allofw.Surface2D(body);
    context.drawSurface(img, 100, 0, 1000, 1000, 100, 100, 500, 500, context.paint());
    s.save("body.png");
});
