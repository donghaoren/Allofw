allofw = require "allofw"
allofwutils = require "allofwutils"

GL = allofw.GL3

w = new allofw.OpenGLWindow();
w.makeContextCurrent();
omni = new allofw.OmniStereo("allofw.yaml");
nav = new allofwutils.WindowNavigation(w, omni)

sphereShader_GeometryCode = "#version 330\n" + omni.getShaderCode() + """
layout(points) in;
layout(triangle_strip, max_vertices = 50) out;
in vec4 colors[1];
in float radiuses[1];
in vec3 positions[1];

out vec4 color;
out float radius;
out vec3 center;
out vec3 p_prime;

void main() {
    color = colors[0];
    radius = radiuses[0];
    center = positions[0];

    int sides = 24;

    float d = length(center);
    if(d <= radius) return;

    float x = radius * radius / d;
    vec3 center_prime = center - center * (x / d);
    float radius_prime = sqrt(radius * radius - x * x);
    radius_prime /= cos(3.1415926535897932 / sides);
    radius_prime *= 1.01;
    vec3 up = vec3(0, 1, 1);
    vec3 ex = normalize(cross(center, up));
    vec3 ey = normalize(cross(ex, center));
    ex *= radius_prime;
    ey *= radius_prime;

    vec3 p0 = center_prime + ex;

    for(int i = 0; i <= sides; i++) {
        float t = float(i) / sides * 3.1415926535897932 * 2;
        vec3 p1 = center_prime + ex * cos(t) + ey * sin(t);

        p_prime = center_prime; gl_Position = omni_render(p_prime); EmitVertex();
        p_prime = p1; gl_Position = omni_render(p_prime); EmitVertex();
    }
    EndPrimitive();
}
"""

sphereShader_VertexCode = "#version 330\n" + omni.getShaderCode() + """
layout(location = 0) in vec4 xyz_radius;

out vec4 colors;
out float radiuses;
out vec3 positions;

void main() {
    colors = vec4(1, 1, 1, 1);
    positions = omni_transform(xyz_radius.xyz);
    radiuses = xyz_radius.w;
}
"""

sphereShader_FragmentCode = "#version 330\n" + omni.getShaderCode() + """
uniform float specular_term = 20;
uniform vec3 light_position = vec3(0, 0, 0);
uniform vec4 light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 light_diffuse = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

in vec4 color;
in float radius;
in vec3 center;
in vec3 p_prime;

layout(location = 0) out vec4 fragment_color;

void main() {
    float qa = dot(p_prime, p_prime);
    float qb = -2.0 * dot(p_prime, center);
    float qc = dot(center, center) - radius * radius;
    float qd = qb * qb - 4.0 * qa * qc;
    if(qd <= 0.0) discard;
    float t = (-qb - sqrt(qd)) / qa / 2.0;

    vec3 p = p_prime * t;

    vec3 N = normalize(p - center);
    vec3 L = normalize(omni_transform(light_position) - p);
    vec3 R = reflect(-L, N);

    vec4 colorMixed = color;
    vec4 final_color = colorMixed * light_ambient;

    float lambertTerm = max(dot(N, L), 0.0);
    final_color += light_diffuse * colorMixed * lambertTerm;
    vec3 E = normalize(-p);
    float spec = pow(max(dot(R, E), 0.0), specular_term);
    final_color += light_specular * spec;
    final_color.a = color.a;
    final_color.rgb *= final_color.a;
    fragment_color = final_color;

    vec4 clip_position = omni_render(p);
    vec3 pixel_position;
    pixel_position.xy = clip_position.xy;
    pixel_position.z = -clip_position.w;
    pixel_position = pixel_position * (length(p) / length(pixel_position));
    float z2 = pixel_position.z * omni_viewport_projection.z + omni_viewport_projection.w;
    gl_FragDepth = (z2 / -pixel_position.z * 0.5 + 0.5);
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
    GL.shaderSource(shader_v, [sphereShader_VertexCode])
    shader_g = GL.createShader(GL.GEOMETRY_SHADER)
    GL.shaderSource(shader_g, [sphereShader_GeometryCode])
    shader_f = GL.createShader(GL.FRAGMENT_SHADER)
    GL.shaderSource(shader_f, [sphereShader_FragmentCode])
    @program = GL.createProgram()

    GL.compileShader(shader_v)
    log = getShaderInfoLog(shader_v)
    if log?
        console.log(log)
    GL.compileShader(shader_g)
    log = getShaderInfoLog(shader_g)
    if log?
        console.log(log)
    GL.compileShader(shader_f)
    log = getShaderInfoLog(shader_f)
    if log?
        console.log(log)

    GL.attachShader(program, shader_v)
    GL.attachShader(program, shader_g)
    GL.attachShader(program, shader_f)

    GL.linkProgram(program)
    log = getProgramInfoLog(program)
    if log?
        console.log(log)

setupBuffers = () ->
    @vertex_buffer = new GL.Buffer()
    @vertex_array = new GL.VertexArray()

    vertices = []

    idx = 0

    for x in [-5..-2]
        for y in [-5..-2]
            for z in [-5..-2]
                if x != 0 || y != 0 || z != 0
                    vertices[idx++] = x
                    vertices[idx++] = y
                    vertices[idx++] = z
                    vertices[idx++] = 0.8

    @total_vertices = vertices.length / 4

    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    vertices_array = new Float32Array(vertices)
    GL.bufferData(GL.ARRAY_BUFFER, 4 * vertices_array.length, vertices_array, GL.STATIC_DRAW)

    GL.bindVertexArray(vertex_array)
    GL.enableVertexAttribArray(0)
    GL.bindBuffer(GL.ARRAY_BUFFER, vertex_buffer)
    GL.vertexAttribPointer(0, 4, GL.FLOAT, GL.FALSE, 16, 0)
    GL.bindBuffer(GL.ARRAY_BUFFER, 0)
    GL.bindVertexArray(0)


setupRender = () ->
    compileShaders()
    setupBuffers()

omni.onCaptureViewport () ->
    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
    GL.useProgram(program.id())
    omni.setUniforms(program.id())
    GL.bindVertexArray(vertex_array)
    GL.drawArrays(GL.POINTS, 0, total_vertices)

render = () ->
    omni.capture()
    sz = w.getFramebufferSize()
    omni.composite(0, 0, sz[0], sz[1])
    w.swapBuffers();

setupRender()
render()
w.onRefresh(render)

timer = setInterval () ->
    nav.update()
    render()
    w.pollEvents()

w.onClose () ->
    clearInterval(timer)


