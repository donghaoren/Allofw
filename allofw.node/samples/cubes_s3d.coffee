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
size = 10
for i in [-size..size]
    for j in [-size..size]
        for k in [-size..size]
            if i != 0 || j != 0 || k != 0
                data.push( { x: i, y: j, z: k } )

# Define texts
cubes = S3.cubes()
    .attr("vec3", "center", "v1")
    .attr("vec3", "size", "vec3(0.2, 0.2, 0.2)")
    .variable("vec3", "v1", (d) -> [ d.x, d.y, d.z ])
    .compile(omni)
    .data(data)


omni.onCaptureViewport () ->
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);
    # Render cubes
    # GL.polygonMode(GL.FRONT_AND_BACK, GL.LINE)
    cubes.render(omni)
    # GL.polygonMode(GL.FRONT_AND_BACK, GL.FILL)

t0 = new Date().getTime() / 1000

console.log "Text count:", data.length

render = () ->
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
