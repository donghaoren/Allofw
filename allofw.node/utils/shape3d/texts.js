var ShapeObject = require("./general_object.js").ShapeObject;
var VariableValue = require("./general_object.js").VariableValue;
var TextCache = require("../text/textcache.js").TextCache;
var GL = require("allofw").GL3;

var TextObject = function() {
    ShapeObject.call(this);
    this.text_value = new VariableValue("string", "");
    this.font_value = new VariableValue("object", { family: "Arial", size: 96 });
    this.style_value = new VariableValue("object", { fill: [ 255, 255, 255, 1 ] });

    this.textcache = new TextCache(2048, 2048);
    this.uniform("int", "__texture_w", this.textcache.width);
    this.uniform("int", "__texture_h", this.textcache.height);

    var self = this;

    this.variable("vec4", "__texture_xywh", function(d) {
        var rect = self.data2text.get(d);
        return [ rect.x, rect.y, rect.w, rect.h ];
    });
    this.variable("vec2", "__texture_offset", function(d) {
        var rect = self.data2text.get(d);
        return [ rect.x_offset, rect.baseline_offset ];
    });
};

TextObject.prototype = Object.create(ShapeObject.prototype);

TextObject.prototype.text = function(f) {
    this.text_value = new VariableValue("string", f);
    return this;
};

TextObject.prototype.font = function(f) {
    this.font_value = new VariableValue("object", f);
    return this;
};

TextObject.prototype.style = function(f) {
    this.style_value = new VariableValue("object", f);
    return this;
};

TextObject.prototype.data = function(data) {
    var self = this;
    this.textcache.clear();
    this.data2text = new WeakMap();
    data.forEach(function(d) {
        self.data2text.set(d,
            self.textcache.addText(
                self.text_value.get(d),
                self.font_value.get(d),
                self.style_value.get(d)
            )
        );
    });

    ShapeObject.prototype.data.call(this, data);

    this.textcache.surface.uploadTexture();

    return this;
};

TextObject.prototype.compile = function(omni) {
    ShapeObject.prototype.compile.call(this, omni);
    GL.useProgram(this.program.id());
    GL.uniform1i(GL.getUniformLocation(this.program.id(), "texCache"), 1);
    GL.useProgram(0);
    return this;
};

TextObject.prototype.render = function(omni) {
    this.textcache.surface.bindTexture(1);
    ShapeObject.prototype.render.call(this, omni);
    this.textcache.surface.unbindTexture(1);
};


TextObject.prototype._vertexShader = function() { return [
    "vec3 center;",
    "vec3 normal;",
    "vec3 up;",
    "float scale;",

    "out vec3 centers;",
    "out vec3 normals;",
    "out vec3 ups;",
    "out float scales;",
    "out vec4 t_xywhs;",
    //"out vec2 t_offsets;",

    "__ATTRIBUTE_LINES__",

    "void main() {",
    "    computeAttributes();",
    "    centers = omni_transform(center);",
    "    normals = omni_transform_normal(normalize(normal));",
    "    ups = omni_transform_normal(normalize(up));",
    "    t_xywhs = __texture_xywh;",
    //"    t_offsets = __texture_offset;",
    "    scales = scale;",
    "}"
].join("\n"); };
TextObject.prototype._geometryShader = function() { return [
    "layout(points) in;",
    "layout(triangle_strip, max_vertices = 4) out;",
    "in vec3 centers[1];",
    "in vec3 normals[1];",
    "in vec3 ups[1];",
    "in float scales[1];",
    "in vec4 t_xywhs[1];",
    //"in vec2 t_offsets[1];",

    "out vec2 texCoord;",

    "void main() {",
    "    vec3 p;",
    "    vec3 ex = normalize(cross(ups[0], normals[0]));",
    "    vec3 ey = normalize(cross(normals[0], ex));",
    "    vec2 swh = vec2(scales[0] * t_xywhs[0].z / 2, scales[0] * t_xywhs[0].w / 2);",
    "    vec3 center = centers[0];",
    "    p = center - ex * swh.x + ey * swh.y;",
    "    texCoord = t_xywhs[0].xy;",
    "    gl_Position = omni_render(p);",
    "    EmitVertex();",
    "    p = center + ex * swh.x + ey * swh.y;",
    "    texCoord = t_xywhs[0].xy + vec2(t_xywhs[0].z, 0);",
    "    gl_Position = omni_render(p);",
    "    EmitVertex();",
    "    p = center - ex * swh.x - ey * swh.y;",
    "    texCoord = t_xywhs[0].xy + vec2(0, t_xywhs[0].w);",
    "    gl_Position = omni_render(p);",
    "    EmitVertex();",
    "    p = center + ex * swh.x - ey * swh.y;",
    "    texCoord = t_xywhs[0].xy + t_xywhs[0].zw;",
    "    gl_Position = omni_render(p);",
    "    EmitVertex();",
    "    EndPrimitive();",
    "}"
].join("\n"); };

TextObject.prototype._fragmentShader = function() { return [
    "layout(location = 0) out vec4 fragment_color;",
    "uniform sampler2D texCache;",
    "in vec2 texCoord;",
    "void main() {",
        "fragment_color = texture(texCache, texCoord / vec2(__texture_w, __texture_h));",
        "if(fragment_color.a == 0) discard;",
    "}"
].join("\n"); };

TextObject.prototype.constructor = TextObject;

exports.texts = function() {
    return new TextObject();
};

