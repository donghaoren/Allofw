var allofw = require("allofw");

var GL = allofw.GL3;

var w = new allofw.OpenGLWindow();
w.makeContextCurrent();

function GetOpenGLCapabilities() {
    var glGetInteger = function(pname) {
        var buf = new Buffer(4);
        GL.getIntegerv(pname, buf)
        return buf.readUInt32LE(0);
    }
    var keys = [
        "MAX_VERTEX_ATTRIBS",
        "MAX_GEOMETRY_OUTPUT_VERTICES",
        "MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS",
        "MAX_GEOMETRY_OUTPUT_COMPONENTS",
        "MAX_DRAW_BUFFERS",
    ];
    var result = { }
    for(var i = 0; i < keys.length; i++) {
        result[keys[i]] = glGetInteger(GL[keys[i]]);
    }
    result['VERSION'] = GL.getString(GL.VERSION);
    result['VENDOR'] = GL.getString(GL.VENDOR);
    result['RENDERER'] = GL.getString(GL.RENDERER);
    return result;
}
console.log(GetOpenGLCapabilities());


var vertex_shader = [
"#version 330",
"in vec3 position;",
"in vec3 colorIn;",
"out vec3 color;",
"void main()",
"{",
    "color = colorIn;",
    "gl_Position = vec4(position, 1.0);",
"}"].join("\n");

var fragment_shader = [
"#version 330",
"in vec3 color;",
"out vec4 outputF;",
"void main()",
"{",
    "outputF = vec4(color, 1.0);",
"}"].join("\n");

var program, vertex_buffer, vertex_array;

function getShaderInfoLog(shader) {
    var buffer = new Buffer(4);
    GL.getShaderiv(shader, GL.INFO_LOG_LENGTH, buffer);
    var length = buffer.readUInt32LE(0);
    if(length > 0) {
        var buf = new Buffer(length);
        GL.getShaderInfoLog(shader, length, buffer, buf);
        return buf.toString("utf-8");
    } else {
        return null;
    }
}

function getProgramInfoLog(program) {
    var buffer = new Buffer(4);
    GL.getProgramiv(program, GL.INFO_LOG_LENGTH, buffer);
    var length = buffer.readUInt32LE(0);
    if(length > 0) {
        var buf = new Buffer(length);
        GL.getProgramInfoLog(program, length, buffer, buf);
        return buf.toString("utf-8");
    } else {
        return null;
    }
}

function compileShaders() {
    var shader_v = GL.createShader(GL.VERTEX_SHADER);
    GL.shaderSource(shader_v, [vertex_shader]);
    var shader_f = GL.createShader(GL.FRAGMENT_SHADER);
    GL.shaderSource(shader_f, [fragment_shader]);
    program = GL.createProgram();

    GL.compileShader(shader_v);
    var log = getShaderInfoLog(shader_v);
    if(log) console.log(log);
    GL.compileShader(shader_f);
    var log = getShaderInfoLog(shader_f);
    if(log) console.log(log);

    GL.attachShader(program, shader_v);
    GL.attachShader(program, shader_f);

    GL.bindFragDataLocation(program, 0, "outputF");

    GL.linkProgram(program);
    var log = getProgramInfoLog(program);
    if(log) console.log(log);

    GL.getProgramiv
}

function setupBuffers() {
    vertex_buffer = new GL.Buffer();
    vertex_array = new GL.VertexArray();

    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer);
    var err = GL.getError(); if(err != 0) throw new Error(err);
    var vertices1 = new Float32Array([
        0, 0, 0, 1, 0, 1,
        0, 1, 0, 1, 1, 1,
        1, 0, 0, 1, 1, 1
    ]);
    GL.bufferData(GL.ARRAY_BUFFER, 4 * vertices1.length, vertices1, GL.STATIC_DRAW);
    var err = GL.getError(); if(err != 0) throw new Error(err);

    GL.bindVertexArray(vertex_array);
    GL.enableVertexAttribArray(0);
    GL.enableVertexAttribArray(1);
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer);
    GL.vertexAttribPointer(1, 3, GL.FLOAT, GL.FALSE, 24, 0);
    GL.vertexAttribPointer(0, 3, GL.FLOAT, GL.FALSE, 24, 12);
    var err = GL.getError(); if(err != 0) throw new Error(err);
}

function setupRender() {
    compileShaders();
    setupBuffers();
}

function render() {
    var sz = w.getFramebufferSize();
    GL.viewport(0, 0, sz[0], sz[1]);

    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT);
    GL.useProgram(program.id());
    GL.bindVertexArray(vertex_array);
    GL.drawArrays(GL.TRIANGLES, 0, 3);
    w.swapBuffers();
}

setupRender();
render();
w.onRefresh(render);

var timer = setInterval(function() {
    render();
    w.pollEvents();
}, 1);

w.onClose(function() {
    clearInterval(timer);
});


