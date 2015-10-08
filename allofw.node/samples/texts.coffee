allofw = require "allofw"
allofwutils = require "allofwutils"

GL = allofw.GL3

w = new allofw.OpenGLWindow();
w.makeContextCurrent();

omni = new allofw.OmniStereo("allofw.yaml");
nav = new allofwutils.WindowNavigation(w, omni)

S3 = allofwutils.shape3d

# Generate data.
data = []
size = 40
for i in [-size..size]
    for j in [-size..size]
        for k in [-size..size]
            if i != 0 || j != 0 || k != 0
                data.push( { x: i, y: j, z: k } )

# Define texts
texts = S3.texts()
    .attr("vec3", "center", "v1")
    .attr("vec3", "up", "vec3(0, 1, 0.001)")
    .attr("vec3", "normal", "-vec3(cos(angle) * v1.x + sin(angle) * v1.z, v1.y, -sin(angle) * v1.x + cos(angle) * v1.z)")
    .attr("float", "scale", "0.01 / 4")
    .uniform("float", "angle", 0)
    .variable("vec3", "v1", (d) -> [ d.x, d.y, d.z ])
    .text((data) -> Math.sin(data.x + data.y + data.z).toFixed(2))
    .compile(omni)
    .data(data)


omni.onCaptureViewport () ->
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);
    # Render texts
    texts.render(omni)

t0 = new Date().getTime() / 1000

console.log "Text count:", data.length

render = () ->
    time = new Date().getTime() / 1000 - t0
    time *= 3

    # Set uniform or variables again.
    texts
      .uniform("float", "angle", time)

    omni.capture()
    sz = w.getFramebufferSize()
    omni.composite(0, 0, sz[0], sz[1])
    w.swapBuffers();

timer = setInterval () ->
    nav.update()
    render()
    w.pollEvents()

    if w.shouldClose()
        clearInterval(timer)

w.onClose () ->
    clearInterval(timer)
