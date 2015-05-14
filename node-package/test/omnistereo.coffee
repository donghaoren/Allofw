allofw = require "allofw"

GL = allofw.GL3

w = new allofw.OpenGLWindow();
w.makeContextCurrent();
omni = new allofw.OmniStereo("allofw.yaml");

vertex_shader = "#version 330\n" + omni.getShaderCode() + """
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;
    out vec3 Color;
    void main() {
        Color = color;
        gl_Position = omni_render(omni_transform(position));
    }
"""

fragment_shader = "#version 330\n" + """
    in vec3 Color;
    layout(location = 0) out vec4 outputF;
    void main() {
        outputF = vec4(Color, 1.0);
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

    vertices = []

    cube = (x, y, z, size) ->
        c0 = 0.3
        c1 = 0.7
        v0 = [ x - size / 2, y - size / 2, z - size / 2, c0, c0, c0 ]
        v1 = [ x - size / 2, y - size / 2, z + size / 2, c0, c0, c1 ]
        v2 = [ x - size / 2, y + size / 2, z - size / 2, c0, c1, c0 ]
        v3 = [ x - size / 2, y + size / 2, z + size / 2, c0, c1, c1 ]
        v4 = [ x + size / 2, y - size / 2, z - size / 2, c1, c0, c0 ]
        v5 = [ x + size / 2, y - size / 2, z + size / 2, c1, c0, c1 ]
        v6 = [ x + size / 2, y + size / 2, z - size / 2, c1, c1, c0 ]
        v7 = [ x + size / 2, y + size / 2, z + size / 2, c1, c1, c1 ]

        insert = (x) ->
            vertices.push(x[0])
            vertices.push(x[1])
            vertices.push(x[2])
            vertices.push(x[3])
            vertices.push(x[4])
            vertices.push(x[5])

        insert(v0); insert(v1); insert(v3); insert(v0); insert(v3); insert(v2); # 0132;
        insert(v3); insert(v1); insert(v5); insert(v3); insert(v5); insert(v7); # 3157;
        insert(v3); insert(v7); insert(v6); insert(v3); insert(v6); insert(v2); # 3762;
        insert(v1); insert(v0); insert(v4); insert(v1); insert(v4); insert(v5); # 1045;
        insert(v5); insert(v4); insert(v6); insert(v5); insert(v6); insert(v7); # 5467;
        insert(v0); insert(v2); insert(v6); insert(v0); insert(v6); insert(v4); # 0264;

    for x in [-5..5]
        for y in [-5..5]
            for z in [-5..5]
                if x != 0 || y != 0 || z != 0
                    cube(x, y, z, 0.3)

    @total_vertices = vertices.length / 6

    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    vertices_array = new Float32Array(vertices)
    GL.bufferData(GL.ARRAY_BUFFER, 4 * vertices_array.length, vertices_array, GL.STATIC_DRAW)

    GL.bindVertexArray(vertex_array)
    GL.enableVertexAttribArray(0)
    GL.enableVertexAttribArray(1)
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    GL.vertexAttribPointer(0, 3, GL.FLOAT, GL.FALSE, 24, 0)
    GL.vertexAttribPointer(1, 3, GL.FLOAT, GL.FALSE, 24, 12)

setupRender = () ->
    compileShaders()
    setupBuffers()

omni.onCaptureViewport () ->
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.useProgram(program.id())
    omni.setUniforms(program.id())
    GL.bindVertexArray(vertex_array)
    GL.drawArrays(GL.TRIANGLES, 0, total_vertices)

render = () ->
    omni.capture()
    sz = w.getFramebufferSize()
    omni.composite(0, 0, sz[0], sz[1])
    w.swapBuffers();

setupRender()
render()
w.onRefresh(render)

timer = setInterval () ->
    render()
    w.pollEvents()

w.onClose () ->
    clearInterval(timer)


