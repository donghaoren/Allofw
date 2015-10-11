var SphereObject = function() {
    ShapeObject.call(this);
};

SphereObject.prototype = Object.create(ShapeObject.prototype);
SphereObject.prototype._vertexShader = function() { return `
    vec3 center;
    float radius;
    out vec4 _colors;
    out float _radiuses;
    out vec3 _positions;
    __ATTRIBUTE_LINES__
    void main() {
        computeAttributes();
        _colors = vec4(1, 1, 1, 1);
        _positions = omni_transform(center);
        _radiuses = radius;
    }
`; };

SphereObject.prototype._geometryShader = function() { return `
    layout(points) in;
    layout(triangle_strip, max_vertices = 50) out;
    in vec4 _colors[1];
    in float _radiuses[1];
    in vec3 _positions[1];

    out vec4 color;
    out float radius;
    out vec3 center;
    out vec3 p_prime;

    void main() {
        color = _colors[0];
        radius = _radiuses[0];
        center = _positions[0];

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
`; };

SphereObject.prototype._fragmentShader = function() { return `
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
`; };

SphereObject.prototype.constructor = SphereObject;

shape3d.spheres = function() {
    return new SphereObject();
};

