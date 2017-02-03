var graphics = require("allofw").graphics;

var render = function(context) {
    var paint = context.paint();

    context.clear(255, 255, 255, 1);
    context.drawLine(0, 0, 1000, 1000, paint);
    paint.setTypeface("Arial", graphics.FONTSTYLE_NORMAL);
    paint.setTextSize(120);
    paint.setTextAlign(graphics.TEXTALIGN_CENTER);
    paint.setMode(graphics.PAINTMODE_FILL);
    context.drawText("Hello World", 500, 500, paint);
    paint.setTextSize(60);
    context.drawText("This is Node-Graphics (Skia Backend)", 500, 700, paint);
};

var s = new graphics.Surface2D(1000, 1000, graphics.SURFACETYPE_RASTER);
var context = new graphics.GraphicalContext2D(s);
render(context);


var request = require('request');
request({ url: 'http://www.baidu.com/img/bdlogo.png', encoding: null }, function(error, response, body) {
    var img = new graphics.Surface2D(body);
    console.log(img.width(), img.height());
    context.drawSurface(img, 100, 0, 1000, 1000, 100, 100, 500, 500, context.paint());
    s.save("body.png");
});

// context = null;
// s = null;
