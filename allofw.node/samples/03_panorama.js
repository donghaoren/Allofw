var allofw = require("allofw");
var allofwutils = require("allofw-utils");

var GL = allofw.GL3;

var window = new allofw.OpenGLWindow({ config: "allofw.yaml" });
window.makeContextCurrent();

var omni = new allofw.OmniStereo("allofw.yaml");
var nav = new allofwutils.WindowNavigation(window, omni);

// Build a cube (vertex array and buffer).
function MakeCube(cx, cy, cz, sx, sy, sz) {
    var v0 = [ cx - sx / 2, cy - sy / 2, cz - sz / 2 ];
    var v1 = [ cx - sx / 2, cy - sy / 2, cz + sz / 2 ];
    var v2 = [ cx - sx / 2, cy + sy / 2, cz - sz / 2 ];
    var v3 = [ cx - sx / 2, cy + sy / 2, cz + sz / 2 ];
    var v4 = [ cx + sx / 2, cy - sy / 2, cz - sz / 2 ];
    var v5 = [ cx + sx / 2, cy - sy / 2, cz + sz / 2 ];
    var v6 = [ cx + sx / 2, cy + sy / 2, cz - sz / 2 ];
    var v7 = [ cx + sx / 2, cy + sy / 2, cz + sz / 2 ];

    var vertices = [];

    var insert = (x) => {
        vertices.push(x[0]);
        vertices.push(x[1]);
        vertices.push(x[2]);
    };

    insert(v0); insert(v3); insert(v1); insert(v0); insert(v2); insert(v3);
    insert(v3); insert(v5); insert(v1); insert(v3); insert(v7); insert(v5);
    insert(v3); insert(v6); insert(v7); insert(v3); insert(v2); insert(v6);
    insert(v1); insert(v4); insert(v0); insert(v1); insert(v5); insert(v4);
    insert(v5); insert(v6); insert(v4); insert(v5); insert(v7); insert(v6);
    insert(v0); insert(v6); insert(v2); insert(v0); insert(v4); insert(v6);

    var buffer = new Float32Array(vertices);

    var vertex_buffer = new GL.Buffer();
    var vertex_array = new GL.VertexArray();
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer);
    GL.bufferData(GL.ARRAY_BUFFER, buffer.length * 4, buffer, GL.STATIC_DRAW);
    GL.bindVertexArray(vertex_array);
    GL.enableVertexAttribArray(0);
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer);
    GL.vertexAttribPointer(0, 3, GL.FLOAT, GL.FALSE, 12, 0);
    GL.bindVertexArray(0);
    var m = { };
    m.vertex_buffer = vertex_buffer;
    m.vertex_array = vertex_array;
    m.vertex_count = vertices.length / 3;
    return m;
}

// Main shader.
var program = allofwutils.utils.compileShaders({
    vertex: "#version 330\n" + omni.getShaderCode() + `
        layout(location = 0) in vec3 position;
        out vec3 vo_position;
        void main() {
            gl_Position = omni_render(omni_transform(position));
            vo_position = position;
        }
    `,
    fragment: "#version 330\n" + omni.getShaderCode() + `
        #define PI 3.1415926535897932
        in vec3 vo_position;
        uniform sampler2D texImage;
        layout(location = 0) out vec4 fragment_color;
        void main() {
            // Compute lng, lat from the 3D position.
            vec3 position = normalize(vo_position);
            float lng = atan(position.x, position.z);
            float lat = atan(position.y, length(position.xz));
            // You can play with distrotion here by changing lat, lng.

            fragment_color = texture(texImage, vec2(
                -lng / PI / 2.0 + 0.5,
                -lat / PI + 0.5
            ));
        }
    `
});

// Uniform set to 1.
GL.useProgram(program);
GL.uniform1i(GL.getUniformLocation(program, "texImage"), 0);
GL.useProgram(0);

// Load the equirectangular earth map to a texture.
// First read the image file.
var earth_map = allofw.graphics.loadImageData(require("fs").readFileSync("earth.png"));
// Create an OpenGL texture, set parameters.
var texture = new GL.Texture();
GL.bindTexture(GL.TEXTURE_2D, texture);
GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MAG_FILTER, GL.LINEAR);
GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR_MIPMAP_LINEAR);
GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_S, GL.CLAMP_TO_EDGE);
GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_T, GL.CLAMP_TO_EDGE);
GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA, earth_map.width(), earth_map.height(), 0, GL.RGBA, GL.UNSIGNED_BYTE, earth_map.pixels());
GL.generateMipmap(GL.TEXTURE_2D);
GL.bindTexture(GL.TEXTURE_2D, 0);

// Make the cube. We render the earth on a cube surface.
var cube = MakeCube(0, 0, 0, 10, 10, 10);

// Main rendering code.
omni.onCaptureViewport(function() {
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT);

    // Use the main program.
    GL.useProgram(program);
    // Set omnistereo uniforms (pose, eye separation, etc.)
    omni.setUniforms(program.id());
    // Use the earth texture.
    GL.bindTexture(GL.TEXTURE_2D, texture);
    // Use the cube's vertex array.
    GL.bindVertexArray(cube.vertex_array);
    // Draw the cube.
    GL.drawArrays(GL.TRIANGLES, 0, cube.vertex_count);
    // Cleanup.
    GL.bindTexture(GL.TEXTURE_2D, 0);
    GL.bindVertexArray(0);
    GL.useProgram(0);

    var error = GL.getError();
    if(error) {
        console.log(error);
    }
});

// Main loop (called by timer below).
function render() {
    omni.capture();
    sz = window.getFramebufferSize();
    omni.composite(0, 0, sz[0], sz[1]);
    window.swapBuffers();
}

timer = setInterval(function() {
    nav.update();
    render();
    window.pollEvents();

    if(window.shouldClose()) {
        clearInterval(timer);
    }
});

window.onClose(function() {
    clearInterval(timer);
});
