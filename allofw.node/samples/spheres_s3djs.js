var allofw = require("allofw");
var allofwutils = require("allofwutils");

var GL = allofw.GL3;

var window = new allofw.OpenGLWindow();
window.makeContextCurrent();

var omni = new allofw.OmniStereo("allofw.yaml");
var nav = new allofwutils.WindowNavigation(window, omni);

// S3 refer to the shape3d library.
var S3 = allofwutils.shape3d;

// Generate some 3D points.
var data = [];
var size = 5;
for(var i = -size; i <= size; i++) {
    for(var j = -size; j <= size; j++) {
        for(var k = -size; k <= size; k++) {
            // Skip the origin.
            if(i != 0 || j != 0 || k != 0) {
                data.push( { x: i, y: j, z: k } );
            }
        }
    }
}

// Sphere objects.
spheres = S3.spheres()
    // Attributes are graphical properties.
    .attr("vec3", "center", "v1")
    .attr("float", "radius", "0.15 + sin(v1.x + phase) * 0.1")
    // Variables are bound to data.
    .variable("vec3", "v1", (d) => [ d.x, d.y, d.z ])
    // Uniforms are given as "global" for the shader.
    .uniform("float", "phase", 0.0)
    // Compile after specifying everything.
    .compile(omni)
    // Put the data into it.
    .data(data)

function GetTimeInSeconds() {
    return new Date().getTime() / 1000;
}

omni.onCaptureViewport(function() {
    var time = GetTimeInSeconds();
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT);
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);
    // Update uniforms.
    spheres.uniform("float", "phase", Math.sin(time) * Math.PI);
    // Render spheres
    spheres.render(omni);
});

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
