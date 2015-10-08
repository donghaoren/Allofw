var ShapeObject = require("./general_object.js").ShapeObject;

var CubeObject = function() {
    ShapeObject.call(this);
};

CubeObject.prototype = Object.create(ShapeObject.prototype);
CubeObject.prototype._vertexShader = function() { return `
    vec3 center;
    vec3 size;

    out vec4 _colors;
    out vec3 _sizes;
    out vec3 _centers;
    __ATTRIBUTE_LINES__
    void main() {
        computeAttributes();
        _colors = vec4(1, 1, 1, 1);
        _centers = center;
        _sizes = size;
    }
`; };

CubeObject.prototype._geometryShader = function() { return `
    layout(points) in;
    layout(triangle_strip, max_vertices = 50) out;
    in vec4 _colors[1];
    in vec3 _sizes[1];
    in vec3 _centers[1];

    out vec4 color;
    out vec3 normal;
    out vec3 position;

    void main() {
        color = _colors[0];
        vec3 halfsize = _sizes[0] / 2.0;
        vec3 center = _centers[0];

        normal = omni_transform_normal(vec3(1, 0, 0));
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

        normal = omni_transform_normal(vec3(-1, 0, 0));
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

        normal = omni_transform_normal(vec3(0, 1, 0));
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

        normal = omni_transform_normal(vec3(0, -1, 0));
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

        normal = omni_transform_normal(vec3(0, 0, 1));
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z + halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

        normal = omni_transform_normal(vec3(0, 0, -1));
        position = omni_transform(vec3(center.x + halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x + halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y - halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        position = omni_transform(vec3(center.x - halfsize.x, center.y + halfsize.y, center.z - halfsize.z));
        gl_Position = omni_render(position); EmitVertex();
        EndPrimitive();

    }
`; };

CubeObject.prototype._fragmentShader = function() { return `
    uniform float specular_term = 20;
    uniform vec3 light_position = vec3(0, 0, 0);
    uniform vec4 light_ambient = vec4(0.3, 0.3, 0.3, 1.0);
    uniform vec4 light_diffuse = vec4(0.7, 0.7, 0.7, 1.0);
    uniform vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

    in vec4 color;
    in vec3 normal;
    in vec3 position;

    layout(location = 0) out vec4 fragment_color;

    void main() {
        vec3 N = normalize(normal);
        vec3 L = normalize(omni_transform(light_position) - position);
        vec3 R = reflect(-L, N);

        vec4 colorMixed = color;
        vec4 final_color = colorMixed * light_ambient;

        float lambertTerm = max(dot(N, L), 0.0);
        final_color += light_diffuse * colorMixed * lambertTerm;
        vec3 E = normalize(-position);
        float spec = pow(max(dot(R, E), 0.0), specular_term);
        final_color += light_specular * spec;
        final_color.a = color.a;
        final_color.rgb *= final_color.a;
        fragment_color = final_color;
    }
`; };

CubeObject.prototype.constructor = CubeObject;

exports.cubes = function() {
    return new CubeObject();
};

