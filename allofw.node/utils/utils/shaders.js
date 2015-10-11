(function() {

    var GL = allofw.GL3;

    var getShaderInfoLog = function(shader) {
        var buffer = new Buffer(4);
        GL.getShaderiv(shader, GL.INFO_LOG_LENGTH, buffer);
        var length = buffer.readUInt32LE(0);
        if(length > 0) {
            var buf = new Buffer(length);
            GL.getShaderInfoLog(shader, length, buffer, buf);
            return buf.toString("utf-8");
        }
    };

    var getProgramInfoLog = function(program) {
        var buffer = new Buffer(4);
        GL.getProgramiv(program, GL.INFO_LOG_LENGTH, buffer);
        var length = buffer.readUInt32LE(0);
        if(length > 0) {
            var buf = new Buffer(length);
            GL.getProgramInfoLog(program, length, buffer, buf);
            return buf.toString("utf-8");
        }
    };

    function ShaderException(type, message) {
       this.message = "CompileShaders: " + type + ": " + message;
       this.name = "ShaderException";
    }

    var compileShaders = function(shaders) {
        var shader_v, shader_f, shader_g;
        if(shaders.vertex) {
            shader_v = GL.createShader(GL.VERTEX_SHADER);
            GL.shaderSource(shader_v, [shaders.vertex]);
            GL.compileShader(shader_v);
            var log = getShaderInfoLog(shader_v);
            if(log && log.trim() != "") {
                throw new ShaderException("Vertex", log);
            }
        }
        if(shaders.fragment) {
            shader_f = GL.createShader(GL.FRAGMENT_SHADER);
            GL.shaderSource(shader_f, [shaders.fragment]);
            GL.compileShader(shader_f);
            var log = getShaderInfoLog(shader_f);
            if(log && log.trim() != "") {
                throw new ShaderException("Fragment", log);
            }
        }
        if(shaders.geometry) {
            shader_g = GL.createShader(GL.GEOMETRY_SHADER);
            GL.shaderSource(shader_g, [shaders.geometry]);
            GL.compileShader(shader_g);
            var log = getShaderInfoLog(shader_g);
            if(log && log.trim() != "") {
                throw new ShaderException("Geometry", log);
            }
        }

        var program = GL.createProgram();

        if(shader_v) GL.attachShader(program, shader_v);
        if(shader_f) GL.attachShader(program, shader_f);
        if(shader_g) GL.attachShader(program, shader_g);

        GL.linkProgram(program);
        var log = getProgramInfoLog(program);

        if(log && log.trim() != "") {
            throw new ShaderException("Link", log);
        }

        return program;
    };

    utils.compileShaders = compileShaders;

})();
