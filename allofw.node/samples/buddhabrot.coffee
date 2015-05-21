allofw = require "allofw"

GL = allofw.GL3
graphics = allofw.graphics;

w = new allofw.OpenGLWindow({ width: 600, height: 600 });
w.makeContextCurrent();

vertex_shader = """
    #version 330
    uniform vec2 offset;
    uniform float rotation;
    layout(location = 0) in vec2 c_input;
    out vec2 cs;
    void main() {
        cs = c_input;
        cs = vec2(cos(rotation) * cs.x + sin(rotation) * cs.y, -sin(rotation) * cs.x + cos(rotation) * cs.y);
        cs += offset;
    }
"""

geometry_shader = """
    #version 330
    layout(points) in;
    layout(points, max_vertices = 256) out;

    const int max_iteration = 256;

    in vec2 cs[1];

    void main() {
        vec2 c = cs[0];
        vec2 z0 = vec2(0, 0);
        vec2 z = z0;
        int t = 0;
        while(t < max_iteration) {
            // z' = z^2 + c
            z = vec2(z.x * z.x - z.y * z.y, z.x * z.y * 2.0) + c;
            if(length(z) >= 2) break;
            t += 1;
        }
        if(t < max_iteration) {
            z = z0;
            t = 0;
            while(t < max_iteration) {
                z = vec2(z.x * z.x - z.y * z.y, z.x * z.y * 2.0) + c;
                if(length(z) >= 10) break;
                if(t > 0) {
                    gl_Position = vec4(z / 2, 0, 1);
                    gl_Position.x += 0.2;
                    EmitVertex();
                }
                t += 1;
            }
        }
    }
"""

fragment_shader = """
    #version 330
    layout(location = 0) out vec4 fragment_output;
    void main() {
        float v = 1;
        fragment_output = vec4(v, v, v, 1);
    }
"""

composite_vertex_shader = """
    #version 330
    uniform float x_scale;
    uniform float y_scale;
    layout(location = 0) in vec2 pos;
    out vec2 tex_coord;
    void main() {
        tex_coord = vec2(1.0 - pos.y, pos.x);
        gl_Position = vec4(pos * 2.0 - 1.0, 0, 1);
        gl_Position.x *= x_scale;
        gl_Position.y *= y_scale;
    }
"""

composite_fragment_shader = """
    #version 330
    uniform sampler2D texCounter;
    uniform sampler2D texColormap;
    uniform float max_counter;
    in vec2 tex_coord;
    layout(location = 0) out vec4 fragment_output;
    void main() {
        vec4 counter = texture(texCounter, tex_coord);
        float v = counter.r / max_counter;
        fragment_output = texture(texColormap, vec2(v, 0.5));
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

compileShaders3 = (vertex_shader, geometry_shader, fragment_shader) ->
    shader_v = GL.createShader(GL.VERTEX_SHADER)
    GL.shaderSource(shader_v, [vertex_shader])
    if geometry_shader?
        shader_g = GL.createShader(GL.GEOMETRY_SHADER)
        GL.shaderSource(shader_g, [geometry_shader])
    shader_f = GL.createShader(GL.FRAGMENT_SHADER)
    GL.shaderSource(shader_f, [fragment_shader])
    program = GL.createProgram()

    GL.compileShader(shader_v)
    log = getShaderInfoLog(shader_v)
    if log?
        console.log(log)
    if geometry_shader?
        GL.compileShader(shader_g)
        log = getShaderInfoLog(shader_g)
        if log?
            console.log(log)
    GL.compileShader(shader_f)
    log = getShaderInfoLog(shader_f)
    if log?
        console.log(log)

    GL.attachShader(program, shader_v)
    if geometry_shader?
        GL.attachShader(program, shader_g)
    GL.attachShader(program, shader_f)

    GL.linkProgram(program)
    log = getProgramInfoLog(program)
    if log?
        console.log(log)

    program

setupBuffers = () ->
    @vertex_buffer = new GL.Buffer()
    @vertex_array = new GL.VertexArray()
    @vertices = 0;

    samples_x = 200
    samples_y = 200
    @xmin = -2
    @xmax = +2
    @ymin = -2
    @ymax = +2
    @cell_size_x = (xmax - xmin) / samples_x
    @cell_size_y = (ymax - ymin) / samples_y

    data = []
    for i in [0 .. samples_x - 1]
        for j in [0 .. samples_y - 1]
            x = i / (samples_x - 1) * (xmax - xmin) + xmin
            y = j / (samples_y - 1) * (ymax - ymin) + ymin
            data.push(x)
            data.push(y)
            vertices += 1
    console.log("Number of vertices:", vertices)

    buffer = new Buffer(4 * data.length)
    for i in [0 .. data.length - 1]
        buffer.writeFloatLE(data[i], i * 4)

    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    GL.bufferData(GL.ARRAY_BUFFER, buffer.length, buffer, GL.STATIC_DRAW)

    GL.bindVertexArray(vertex_array)
    GL.enableVertexAttribArray(0)
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    GL.vertexAttribPointer(0, 2, GL.FLOAT, GL.FALSE, 8, 0)
    GL.bindBuffer(GL.ARRAY_BUFFER, 0)
    GL.bindVertexArray(0)

    @framebuffer = new GL.Framebuffer()
    @framebuffer_texture = new GL.Texture()
    @framebuffer_size = 2048

    GL.bindTexture(GL.TEXTURE_2D, framebuffer_texture)
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR_MIPMAP_LINEAR)
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MAG_FILTER, GL.LINEAR)
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_S, GL.CLAMP_TO_EDGE)
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_T, GL.CLAMP_TO_EDGE)
    GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA32F, framebuffer_size, framebuffer_size, 0, GL.RGBA, GL.FLOAT, 0)
    GL.bindTexture(GL.TEXTURE_2D, 0)

    GL.bindFramebuffer(GL.FRAMEBUFFER, framebuffer)
    GL.framebufferTexture2D(GL.FRAMEBUFFER, GL.COLOR_ATTACHMENT0, GL.TEXTURE_2D, framebuffer_texture, 0)
    s = GL.checkFramebufferStatus(GL.FRAMEBUFFER)
    if s != GL.FRAMEBUFFER_COMPLETE
        console.log("Framebuffer incomplete:", s)
    GL.bindFramebuffer(GL.FRAMEBUFFER, 0)

    @quad_buffer = new GL.Buffer()
    @quad_array = new GL.VertexArray()

    buffer = new Buffer(4 * 8)
    buffer.writeFloatLE(0, 0 * 4)
    buffer.writeFloatLE(0, 1 * 4)
    buffer.writeFloatLE(0, 2 * 4)
    buffer.writeFloatLE(1, 3 * 4)
    buffer.writeFloatLE(1, 4 * 4)
    buffer.writeFloatLE(0, 5 * 4)
    buffer.writeFloatLE(1, 6 * 4)
    buffer.writeFloatLE(1, 7 * 4)

    GL.bindBuffer(GL.ARRAY_BUFFER, quad_buffer)
    GL.bufferData(GL.ARRAY_BUFFER, buffer.length, buffer, GL.STATIC_DRAW)

    GL.bindVertexArray(quad_array)
    GL.enableVertexAttribArray(0)
    GL.bindBuffer(GL.ARRAY_BUFFER, quad_buffer)
    GL.vertexAttribPointer(0, 2, GL.FLOAT, GL.FALSE, 8, 0)
    GL.bindBuffer(GL.ARRAY_BUFFER, 0)
    GL.bindVertexArray(0)

    @colormap_image = graphics.loadImageData(require("fs").readFileSync(__dirname + "/buddhabrot_colormap.png"));
    colormap_image.uploadTexture()


setupRender = () ->
    @program = compileShaders3(vertex_shader, geometry_shader, fragment_shader)
    @program_composite = compileShaders3(composite_vertex_shader, undefined, composite_fragment_shader)
    GL.useProgram(program_composite)
    GL.uniform1i(GL.getUniformLocation(program_composite, "texCounter"), 0)
    GL.uniform1i(GL.getUniformLocation(program_composite, "texColormap"), 1)
    GL.useProgram(0)
    setupBuffers()

render_fractal_begin = () ->
    GL.bindFramebuffer(GL.FRAMEBUFFER, framebuffer)
    GL.viewport(0, 0, framebuffer_size, framebuffer_size)
    GL.clear(GL.COLOR_BUFFER_BIT)
    GL.bindFramebuffer(GL.FRAMEBUFFER, 0)

render_fractal = () ->
    x = (Math.random() - 0.5) * cell_size_x
    y = (Math.random() - 0.5) * cell_size_y
    rotation = Math.random() * Math.PI * 2

    GL.disable(GL.DEPTH_TEST)
    GL.depthMask(GL.FALSE)

    GL.bindFramebuffer(GL.FRAMEBUFFER, framebuffer)
    GL.viewport(0, 0, framebuffer_size, framebuffer_size)
    GL.enable(GL.BLEND);
    GL.blendFunc(GL.ONE, GL.ONE);
    GL.pointSize(1);
    GL.useProgram(program)
    GL.uniform2f(GL.getUniformLocation(program, "offset"), x, y)
    GL.uniform1f(GL.getUniformLocation(program, "rotation"), rotation)
    GL.bindVertexArray(vertex_array)
    GL.drawArrays(GL.POINTS, 0, vertices)
    GL.bindVertexArray(0)
    GL.useProgram(0)
    GL.bindFramebuffer(GL.FRAMEBUFFER, 0)
    GL.bindTexture(GL.TEXTURE_2D, framebuffer_texture)
    GL.generateMipmap(GL.TEXTURE_2D)
    GL.bindTexture(GL.TEXTURE_2D, 0)

n = 0

render = () ->
    render_fractal()
    n += 1

    sz = w.getFramebufferSize()
    GL.viewport(0, 0, sz[0], sz[1])
    GL.useProgram(program_composite)

    GL.uniform1f(GL.getUniformLocation(program_composite, "max_counter"), n / 10)
    if sz[0] < sz[1]
        y_scale = sz[0] / sz[1]
        x_scale = 1
    else
        x_scale = sz[1] / sz[0]
        y_scale = 1
    GL.uniform1f(GL.getUniformLocation(program_composite, "y_scale"), y_scale)
    GL.uniform1f(GL.getUniformLocation(program_composite, "x_scale"), x_scale)
    GL.disable(GL.BLEND)
    GL.clear(GL.COLOR_BUFFER_BIT)
    GL.bindVertexArray(quad_array)
    GL.activeTexture(GL.TEXTURE0)
    GL.bindTexture(GL.TEXTURE_2D, framebuffer_texture)
    colormap_image.bindTexture(1)
    GL.drawArrays(GL.TRIANGLE_STRIP, 0, 4)
    colormap_image.unbindTexture(1)
    GL.activeTexture(GL.TEXTURE0)
    GL.bindTexture(GL.TEXTURE_2D, 0)
    GL.bindVertexArray(0)
    GL.useProgram(0)

    err = GL.getError()
    if err != 0
        console.log(err)

    w.swapBuffers()

setupRender()
render_fractal_begin()
render_fractal(0)
render()
w.onRefresh(render)

timer = setInterval () ->
    render()
    w.pollEvents()

w.onClose () ->
    clearInterval(timer)


