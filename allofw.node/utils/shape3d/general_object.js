var GL = require("allofw").GL3;

var AttributeValue = function(type, repr) {
    this.type = type;
    this.repr = repr;
};

var VariableValue = function(type, repr) {
    this.type = type;
    if(typeof(repr) == "function") this.f = repr;
    else this.v = repr;
    this.layout_index = -1;
    this.byte_offset = 0;
};

VariableValue.prototype.update = function(repr) {
    if(typeof(repr) == "function") this.f = repr;
    else this.v = repr;
};

VariableValue.prototype.get = function(d) {
    return this.v !== undefined ? this.v : this.f(d);
};

VariableValue.prototype.writeBuffer = function(buf, offset, d) {
    offset += this.byte_offset;
    var val = this.get(d);
    if(this.type == "int") buf.writeInt32LE(val, offset);
    if(this.type == "float") buf.writeFloatLE(val, offset);
    if(this.type == "vec2") {
        buf.writeFloatLE(val[0], offset);
        buf.writeFloatLE(val[1], offset + 4);
    }
    if(this.type == "vec3") {
        buf.writeFloatLE(val[0], offset);
        buf.writeFloatLE(val[1], offset + 4);
        buf.writeFloatLE(val[2], offset + 8);
    }
    if(this.type == "vec4") {
        buf.writeFloatLE(val[0], offset);
        buf.writeFloatLE(val[1], offset + 4);
        buf.writeFloatLE(val[2], offset + 8);
        buf.writeFloatLE(val[3], offset + 12);
    }
};

VariableValue.prototype.setUniform = function(name, program) {
    var val = this.get(undefined);
    if(this.type == "int")
        GL.uniform1i(GL.getUniformLocation(program, name), val);
    if(this.type == "float")
        GL.uniform1f(GL.getUniformLocation(program, name), val);
    if(this.type == "vec2")
        GL.uniform2f(GL.getUniformLocation(program, name), val[0], val[1]);
    if(this.type == "vec3")
        GL.uniform3f(GL.getUniformLocation(program, name), val[0], val[1], val[2]);
    if(this.type == "vec4")
        GL.uniform4f(GL.getUniformLocation(program, name), val[0], val[1], val[2], val[3]);
};

VariableValue.prototype.getByteLength = function() {
    return {
        "char": 1,
        "int": 4,
        "float" : 4,
        "vec2": 8, "vec3": 12, "vec4": 16,
        "mat3": 36, "mat4": 64
    }[this.type];
};

var ShapeObject = function() {
    this.vars = { };
    this.attrs = { };
    this.uniforms = { };
    this.vertex_array = new GL.VertexArray();
    this.vertex_buffer = new GL.Buffer();
};

ShapeObject.prototype.attr = function(type, name, value) {
    this.attrs[name] = new AttributeValue(type, value);
    return this;
};

ShapeObject.prototype.attrorder = function(names) {
    this.attrs_order = names;
    return this;
};

ShapeObject.prototype.variable = function(type, name, value) {
    if(this.vars[name]) this.vars[name].update(value);
    else this.vars[name] = new VariableValue(type, value);
    return this;
};

ShapeObject.prototype.uniform = function(type, name, value) {
    if(this.uniforms[name]) this.uniforms[name].update(value);
    else this.uniforms[name] = new VariableValue(type, value);
    return this;
};

function getShaderInfoLog(shader) {
    var buffer = new Buffer(4);
    GL.getShaderiv(shader, GL.INFO_LOG_LENGTH, buffer);
    var length = buffer.readUInt32LE(0);
    if(length > 0) {
        var buf = new Buffer(length);
        GL.getShaderInfoLog(shader, length, buffer, buf);
        return buf.toString("utf-8");
    }
}

function getProgramInfoLog(program) {
    var buffer = new Buffer(4);
    GL.getProgramiv(program, GL.INFO_LOG_LENGTH, buffer);
    var length = buffer.readUInt32LE(0);
    if(length > 0) {
        var buf = new Buffer(length);
        GL.getProgramInfoLog(program, length, buffer, buf);
        return buf.toString("utf-8");
    }
}

ShapeObject.prototype.compile = function(omni) {
    var self = this;
    // Assign a name for each uniform.
    var uniform_defs = [];
    var input_defs = [];
    for(var name in this.uniforms) {
        var uniform = this.uniforms[name];
        uniform_defs.push("uniform " + uniform.type + " " + name + ";")
    }

    var layout_index = 0;
    var byte_offset = 0;
    for(var name in this.vars) {
        var variable = this.vars[name];
        variable.layout_index = layout_index;
        variable.byte_offset = byte_offset;
        // Define variable with its layout location.
        input_defs.push("layout(location = " + variable.layout_index + ") in "
                       + variable.type + " " + name + ";");
        layout_index += 1;
        byte_offset += variable.getByteLength();
    }
    this.vertex_byte_size = byte_offset;

    var attribute_lines = [];
    attribute_lines.push("void computeAttributes() {");
    var attr_names = [];
    for(var name in this.attrs) attr_names.push(name);
    attr_names.sort(function(a, b) {
        if(self.attrs_order) {
            var ia = self.attrs_order.indexOf(a);
            var ib = self.attrs_order.indexOf(b);
            return ia - ib;
        } else {
            return a < b ? -1 : (a == b ? 0 : 1);
        }
    });
    for(var i = 0; i < attr_names.length; i++) {
        var name = attr_names[i];
        var attr = this.attrs[name];
        attribute_lines.push("    " + name + " = " + attr.repr + ";");
    }
    attribute_lines.push("}");

    var shader_prefix = []
        .concat(uniform_defs)
        .concat(input_defs)
        .concat([""])
        .join("\n");

    var shader_v = null, shader_g = null, shader_f = null;

    var vertex_code =
        "#version 330\n" +
        shader_prefix +
        omni.getShaderCode() +
        this._vertexShader().replace("__ATTRIBUTE_LINES__", attribute_lines.join("\n"));

    shader_v = GL.createShader(GL.VERTEX_SHADER);
    GL.shaderSource(shader_v, [vertex_code]);

    if(this._geometryShader) {
        var geometry_code =
            "#version 330\n" +
            omni.getShaderCode() +
            this._geometryShader();

        shader_g = GL.createShader(GL.GEOMETRY_SHADER);
        GL.shaderSource(shader_g, [geometry_code]);
    }

    var fragment_code =
        "#version 330\n" +
        omni.getShaderCode() +
        uniform_defs.join("\n") +
        this._fragmentShader();

    shader_f = GL.createShader(GL.FRAGMENT_SHADER);
    GL.shaderSource(shader_f, [fragment_code]);

    this.program = GL.createProgram();
    GL.compileShader(shader_v);
    var log = getShaderInfoLog(shader_v);
    if(log) console.log(vertex_code + "\n" + log);
    if(shader_g) {
        GL.compileShader(shader_g);
        var log = getShaderInfoLog(shader_g);
        if(log) console.log(geometry_code + "\n" + log)
    }
    GL.compileShader(shader_f);
    var log = getShaderInfoLog(shader_f);
    if(log) console.log(fragment_code + "\n" + log);

    GL.attachShader(this.program, shader_v);
    GL.attachShader(this.program, shader_g);
    GL.attachShader(this.program, shader_f);

    GL.linkProgram(this.program);
    var log = getProgramInfoLog(this.program);
    if(log) console.log(log);

    GL.bindBuffer(GL.ARRAY_BUFFER, this.vertex_buffer);
    GL.bindVertexArray(this.vertex_array);
    for(var name in this.vars) {
        var variable = this.vars[name];
        GL.enableVertexAttribArray(variable.layout_index);
        if(variable.type == "float") {
            GL.vertexAttribPointer(variable.layout_index, 1, GL.FLOAT, GL.FALSE, this.vertex_byte_size, variable.byte_offset);
        }
        if(variable.type == "vec2") {
            GL.vertexAttribPointer(variable.layout_index, 2, GL.FLOAT, GL.FALSE, this.vertex_byte_size, variable.byte_offset);
        }
        if(variable.type == "vec3") {
            GL.vertexAttribPointer(variable.layout_index, 3, GL.FLOAT, GL.FALSE, this.vertex_byte_size, variable.byte_offset);
        }
        if(variable.type == "vec4") {
            GL.vertexAttribPointer(variable.layout_index, 4, GL.FLOAT, GL.FALSE, this.vertex_byte_size, variable.byte_offset);
        }
    }
    GL.bindBuffer(GL.ARRAY_BUFFER, 0);
    GL.bindVertexArray(0);

    GL.useProgram(this.program);
    for(var name in this.uniforms) {
        this.uniforms[name].setUniform(name, this.program.id());
    }
    GL.useProgram(0);

    return this;
};
ShapeObject.prototype.data = function(data) {
    if(!data) data = [ undefined ];
    this.vertex_count = data.length;

    GL.bindBuffer(GL.ARRAY_BUFFER, this.vertex_buffer);

    var buf = new Buffer(this.vertex_byte_size * data.length);

    for(var i = 0; i < data.length; i++) {
        offset = i * this.vertex_byte_size;
        for(var name in this.vars) {
            var variable = this.vars[name];
            variable.writeBuffer(buf, offset, data[i]);
        }
    }

    GL.bufferData(GL.ARRAY_BUFFER, buf.length, buf, GL.STATIC_DRAW)
    GL.bindBuffer(GL.ARRAY_BUFFER, 0);

    GL.useProgram(this.program.id());
    return this;
};

ShapeObject.prototype.render = function(omni) {
    GL.useProgram(this.program.id());

    for(var name in this.uniforms) {
        this.uniforms[name].setUniform(name, this.program.id());
    }
    omni.setUniforms(this.program.id());

    GL.bindVertexArray(this.vertex_array);
    GL.drawArrays(GL.POINTS, 0, this.vertex_count);

    GL.useProgram(0);

    var err = GL.getError();
    return this;
};

shape3d.ShapeObject = ShapeObject;
shape3d.VariableValue = VariableValue;
