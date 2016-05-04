// Import the allofw library.
var allofw = require("allofw");
var GL = allofw.GL3;

// Create the window.
var w = new allofw.OpenGLWindow({ config: "allofw.yaml" });
w.makeContextCurrent();

// Create the OmniStereo.
var omni = new allofw.OmniStereo("allofw.yaml");

// Set capture callback.
omni.onCaptureViewport(function() {
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);

    // TODO: Render something here.

});

// Render as fast as possible.
var timer = setInterval(function() {
    // Capture the scene.
    omni.capture();
    // Composite the scene to the full window.
    var sz = w.getFramebufferSize();
    omni.composite(0, 0, sz[0], sz[1]);

    // Swap buffers.
    w.swapBuffers();

    // Poll window events.
    w.pollEvents();
    if(w.shouldClose()) {
        clearInterval(timer);
    }
}, 0);

w.onClose(function() {
    clearInterval(timer);
});
