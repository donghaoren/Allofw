allofw = require "allofw"

GL = allofw.GL3

w = new allofw.OpenGLWindow();
w.makeContextCurrent();

vertex_shader = """
    #version 330
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 colorIn;
    out vec3 color;
    void main() {
        color = colorIn;
        gl_Position = vec4(position, 1.0);
    }
"""

fragment_shader = """
    #version 330
    in vec3 color;
    out vec4 outputF;
    void main() {
        outputF = vec4(color, 1.0);
    }
"""

getShaderInfoLog = (shader) ->
    buffer = new Buffer(4)
    GL.getShaderiv(shader, GL.INFO_LOG_LENGTH, buffer)
    length = buffer.readUInt32LE(0)
    if length > 0
        buf = new Buffer(length)
        GL.getShaderInfoLog(shader, length, buffer, buf)
        buf.toString("utf-8")

getProgramInfoLog = (program) ->
    buffer = new Buffer(4)
    GL.getProgramiv(program, GL.INFO_LOG_LENGTH, buffer)
    length = buffer.readUInt32LE(0)
    if length > 0
        buf = new Buffer(length)
        GL.getProgramInfoLog(program, length, buffer, buf)
        buf.toString("utf-8")
    else
        null

compileShaders = () ->
    shader_v = GL.createShader(GL.VERTEX_SHADER)
    GL.shaderSource(shader_v, [vertex_shader])
    shader_f = GL.createShader(GL.FRAGMENT_SHADER)
    GL.shaderSource(shader_f, [fragment_shader])
    @program = GL.createProgram()

    GL.compileShader(shader_v)
    log = getShaderInfoLog(shader_v)
    if log?
        console.log(log)
    GL.compileShader(shader_f)
    log = getShaderInfoLog(shader_f)
    if log?
        console.log(log)

    GL.attachShader(program, shader_v)
    GL.attachShader(program, shader_f)

    GL.bindFragDataLocation(program, 0, "outputF")

    GL.linkProgram(program)
    log = getProgramInfoLog(program)
    if log?
        console.log(log)

setupBuffers = () ->
    @vertex_buffer = new GL.Buffer()
    @vertex_array = new GL.VertexArray()

    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    vertices1 = new Float32Array([
        0, 0, 0, 1, 0, 1,
        0, 1, 0, 1, 1, 1,
        1, 0, 0, 1, 1, 1
    ])
    GL.bufferData(GL.ARRAY_BUFFER, 4 * vertices1.length, vertices1, GL.STATIC_DRAW)

    GL.bindVertexArray(vertex_array)
    GL.enableVertexAttribArray(0)
    GL.enableVertexAttribArray(1)
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    GL.vertexAttribPointer(0, 3, GL.FLOAT, GL.FALSE, 24, 0)
    GL.vertexAttribPointer(1, 3, GL.FLOAT, GL.FALSE, 24, 12)

setupRender = () ->
    compileShaders()
    setupBuffers()

render = () ->
    sz = w.getFramebufferSize()
    GL.viewport(0, 0, sz[0], sz[1])

    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.useProgram(program.id())
    GL.bindVertexArray(vertex_array)
    GL.drawArrays(GL.TRIANGLES, 0, 3)
    w.swapBuffers()

setupRender()
render()
w.onRefresh(render)

timer = setInterval () ->
    render()
    w.pollEvents()

w.onClose () ->
    clearInterval(timer)


