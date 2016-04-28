// Text rendering engine.
var graphics = require("allofw").graphics;

// Create a text cache of given width and height.
var TextCache = function(width, height) {
    if(width === undefined) width = 1024;
    if(height === undefined) height = 1024;
    this.entries = { };
    this.current_x = 0;
    this.current_y = 0;
    this.current_height = 0;
    this.width = width;
    this.height = height;
    this.surface = new graphics.Surface2D(width, height, graphics.SURFACETYPE_RASTER);
    this.context = new graphics.GraphicalContext2D(this.surface);
    this.context.clear(0, 0, 0, 0);
    this.paint = this.context.paint();
    this.paint.setLineJoin(graphics.LINEJOIN_ROUND);
};

var Font = function(font) {
    if(!font) font = { };
    this.family = font.family !== undefined ? font.family : "Arial";
    this.style = font.style !== undefined ? font.style : "normal";
    this.size = font.size !== undefined ? font.size : 12;
};

Font.prototype.hash = function() {
    return "F" + this.family + "," + this.size;
};

var Style = function(style) {
    if(!style) style = { };
    this.fill = style.fill;
    this.stroke = style.stroke;
    this.stroke_width = style.stroke_width !== undefined ? style.stroke_width : 1;
    this.order = style.order !== undefined ? style.order : "fill,stroke";
};

Style.prototype.hash = function() {
    return "S" +
           (this.fill ? this.fill.join(",") : "null") + "," +
           (this.stroke ? this.stroke.join(",") : "null") +
           this.stroke_width +
           this.order;
};

var string2fontstyle = {
    "normal": graphics.FONTSTYLE_NORMAL,
    "bold": graphics.FONTSTYLE_BOLD,
    "italic": graphics.FONTSTYLE_ITALIC,
    "bolditalic": graphics.FONTSTYLE_BOLDITALIC
};

// Add text of a given font to the cache.
TextCache.prototype.addText = function(text, font, style) {
    font = new Font(font);
    style = new Style(style);
    var hash = JSON.stringify(text) + font.hash() + style.hash();
    if(this.entries[hash]) {
        return this._layout2TextRect(this.entries[hash]);
    }
    this.paint.setTextSize(font.size);
    this.paint.setTypeface(font.family, string2fontstyle[font.style]);
    var width = this.paint.measureText(text);
    var height = font.size;
    var bbox_width = Math.ceil(width + 4 + style.stroke_width);
    var bbox_height = Math.ceil(height + 2 + style.stroke_width);
    var x_offset = 2 + style.stroke_width / 2;
    var baseline_offset = height - 2 + style.stroke_width / 2;

    if(bbox_width > this.width) {
        throw "E_FIT";
    }
    // If can't fit in current line, start a new line.
    if(this.current_x + bbox_width > this.width) {
        if(this.current_y + this.current_height + bbox_height > this.height) {
            throw "E_FIT";
        }
        this.current_x = 0;
        this.current_y += this.current_height;
        this.current_height = bbox_height;
    }
    // Here it must fit.

    var layout = {
        x: this.current_x, y: this.current_y,
        x_offset: x_offset,
        baseline_offset: baseline_offset,
        bbox_width: bbox_width,
        bbox_height: bbox_height
    };

    this.current_x += bbox_width;
    this.current_height = Math.max(this.current_height, bbox_height);

    this.entries[hash] = layout;
    // Render.
    var draw_x = layout.x + x_offset;
    var draw_y = layout.y + baseline_offset;
    if(style.order == "fill,stroke") {
        if(style.fill) {
            this.paint.setColor(style.fill[0], style.fill[1], style.fill[2], style.fill[3]);
            this.paint.setMode(graphics.PAINTMODE_FILL);
            this.context.drawText(text, draw_x, draw_y, this.paint);
        }
        if(style.stroke) {
            this.paint.setColor(style.stroke[0], style.stroke[1], style.stroke[2], style.stroke[3]);
            this.paint.setStrokeWidth(style.stroke_width);
            this.paint.setMode(graphics.PAINTMODE_STROKE);
            this.context.drawText(text, draw_x, draw_y, this.paint);
        }
    } else {
        if(style.stroke) {
            this.paint.setColor(style.stroke[0], style.stroke[1], style.stroke[2], style.stroke[3]);
            this.paint.setStrokeWidth(style.stroke_width);
            this.paint.setMode(graphics.PAINTMODE_STROKE);
            this.context.drawText(text, draw_x, draw_y, this.paint);
        }
        if(style.fill) {
            this.paint.setColor(style.fill[0], style.fill[1], style.fill[2], style.fill[3]);
            this.paint.setMode(graphics.PAINTMODE_FILL);
            this.context.drawText(text, draw_x, draw_y, this.paint);
        }
    }
    this.updated = true;
    // this.paint.setColor(0, 0, 0, 1);
    // this.paint.setStrokeWidth(1);
    // this.paint.setMode(graphics.PAINTMODE_STROKE);
    // this.context.drawRectangle(layout.x + 0.5, layout.y + 0.5, layout.bbox_width - 1, layout.bbox_height - 1, this.paint);

    return this._layout2TextRect(layout);
};
TextCache.prototype._layout2TextRect = function(layout) {
    return {
        x: layout.x, y: layout.y,
        w: layout.bbox_width, h: layout.bbox_height,
        x_offset: layout.x_offset,
        baseline_offset: layout.baseline_offset
    };
};
// Query the cache for a text and given font.
TextCache.prototype.getTextRect = function(text, font, style) {
    font = new Font(font);
    style = new Style(style);
    var hash = JSON.stringify(text) + font.hash() + style.hash();
    var layout = this.entries[hash];
    if(!layout) return null;
    return this._layout2TextRect(layout);
};

// Clear the cache.
TextCache.prototype.clear = function() {
    this.entries = { };
    this.current_x = 0;
    this.current_y = 0;
    this.current_height = 0;
    this.context.clear(0, 0, 0, 0);
};


text.TextCache = TextCache;
