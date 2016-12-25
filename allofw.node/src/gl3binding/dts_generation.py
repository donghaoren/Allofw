function_lines = []
constant_lines = []

def DTSGenerateCode():
    code = "\n".join(constant_lines);
    code += "\n"
    code += "\n".join(function_lines);
    return code

def DTSDefineConstant(name, c_name, value, type):
    constant_lines.append("export let %s: number;" % name);

def DTSDefineFunction(name, args, returnType):
    if returnType == None: returnType = "void"
    else: returnType = returnType.dts_type()
    function_lines.append(
        "function %s(%s): %s;" % (
            name,
            ", ".join(map(lambda (type, name): "%s: %s" % (name, type.dts_type()), args)),
            returnType
        )
    )
