var fs = require("fs");
var path = require("path");

function resolveImports(file) {
    var contents = fs.readFileSync(file, "utf-8");
    var root = path.dirname(file);

    return contents.replace(/^[ \t]*import[ \t]+\"(.*?)\"[; \t]*$/img, function(m, file) {
        file = path.join(root, file);
        return resolveImports(file);
    });
}

fs.writeFileSync("../build/Release/allofwutils.js", resolveImports("allofwutils.js"), "utf-8");
