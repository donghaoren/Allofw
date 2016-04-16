var allofw = require("allofw");
var GL = allofw.GL3;

var w = new allofw.OpenGLWindow();
w.makeContextCurrent();

var omni = new allofw.OmniStereo("allofw.yaml");

omni.onCaptureViewport(function() {
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);
    // Render something here.
});

var timer = setInterval(function() {
    omni.capture();
    var sz = w.getFramebufferSize();
    omni.composite(0, 0, sz[0], sz[1]);
    w.swapBuffers();

    w.pollEvents();
    if(w.shouldClose()) {
        clearInterval(timer);
    }
});

w.onClose(function() {
    clearInterval(timer);
});
