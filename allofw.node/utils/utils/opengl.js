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

    var isShaderCompiled = function(shader) {
        var buffer = new Buffer(4);
        GL.getShaderiv(shader, GL.COMPILE_STATUS, buffer);
        var success = buffer.readUInt32LE(0);
        return success != 0;
    };
    var isProgramLinked = function(program) {
        var buffer = new Buffer(4);
        GL.getProgramiv(program, GL.LINK_STATUS, buffer);
        var success = buffer.readUInt32LE(0);
        return success != 0;
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
                console.log(log);
            }
            if(!isShaderCompiled(shader_v)) {
                throw new ShaderException("Vertex");
            }
        }
        if(shaders.fragment) {
            shader_f = GL.createShader(GL.FRAGMENT_SHADER);
            GL.shaderSource(shader_f, [shaders.fragment]);
            GL.compileShader(shader_f);
            var log = getShaderInfoLog(shader_f);
            if(log && log.trim() != "") {
                console.log(log);
            }
            if(!isShaderCompiled(shader_f)) {
                throw new ShaderException("Fragment");
            }
        }
        if(shaders.geometry) {
            shader_g = GL.createShader(GL.GEOMETRY_SHADER);
            GL.shaderSource(shader_g, [shaders.geometry]);
            GL.compileShader(shader_g);
            if(log && log.trim() != "") {
                console.log(log);
            }
            if(!isShaderCompiled(shader_g)) {
                throw new ShaderException("Geometry");
            }
        }

        var program = GL.createProgram();

        if(shader_v) GL.attachShader(program, shader_v);
        if(shader_f) GL.attachShader(program, shader_f);
        if(shader_g) GL.attachShader(program, shader_g);

        GL.linkProgram(program);
        var log = getProgramInfoLog(program);
        if(log && log.trim() != "") {
            console.log(log);
        }
        if(!isProgramLinked(program)) {
            throw new ShaderException("Link");
        }
        return program;
    };

    utils.compileShaders = compileShaders;

    utils.checkGLError = function(prefix) {
        var err = GL.getError();
        if(err != 0) {
            if(prefix) {
                allofw.log(allofw.kInfo, "OpenGL Error #" + err + " at " + prefix);
            } else {
                allofw.log(allofw.kInfo, "OpenGL Error #" + err);
            }
        }
    };

})();
