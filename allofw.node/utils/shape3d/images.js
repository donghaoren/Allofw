// Display image patches.

var ImageObject = function() {
    ShapeObject.call(this);
};

ImageObject.prototype = Object.create(ShapeObject.prototype);

ImageObject.prototype.xywh = function(f) {
    this.variable("vec4", "__texture_xywh", function(d) {
        var rect = f(d);
        return [ rect.x, rect.y, rect.width, rect.height ];
    });
    return this;
};

ImageObject.prototype.image = function(image, width, height) {
    this.imagetexture = image;
    image.uploadTexture();
    this.uniform("int", "__texture_w", width !== undefined ? width : this.imagetexture.width());
    this.uniform("int", "__texture_h", height !== undefined ? height : this.imagetexture.height());
    return this;
};

ImageObject.prototype.compile = function(omni) {
    ShapeObject.prototype.compile.call(this, omni);
    GL.useProgram(this.program.id());
    GL.uniform1i(GL.getUniformLocation(this.program.id(), "texImage"), 1);
    GL.useProgram(0);
    return this;
};

ImageObject.prototype.render = function(omni) {
    this.imagetexture.bindTexture(1);
    ShapeObject.prototype.render.call(this, omni);
    this.imagetexture.unbindTexture(1);
};


ImageObject.prototype._vertexShader = function() { return [
    "vec3 center;",
    "vec3 normal;",
    "vec3 up;",
    "float scale;",

    "out vec3 centers;",
    "out vec3 normals;",
    "out vec3 ups;",
    "out float scales;",
    "out vec4 t_xywhs;",

    "__ATTRIBUTE_LINES__",

    "void main() {",
    "    computeAttributes();",
    "    centers = omni_transform(center);",
    "    normals = omni_transform_normal(normalize(normal));",
    "    ups = omni_transform_normal(normalize(up));",
    "    t_xywhs = __texture_xywh;",
    "    scales = scale;",
    "}"
].join("\n"); };
ImageObject.prototype._geometryShader = function() { return [
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

ImageObject.prototype._fragmentShader = function() { return [
    "layout(location = 0) out vec4 fragment_color;",
    "uniform sampler2D texImage;",
    "in vec2 texCoord;",
    "void main() {",
        "fragment_color = texture(texImage, texCoord / vec2(__texture_w, __texture_h));",
        "if(fragment_color.a == 0) discard;",
    "}"
].join("\n"); };

ImageObject.prototype.constructor = ImageObject;

shape3d.images = function() {
    return new ImageObject();
};

