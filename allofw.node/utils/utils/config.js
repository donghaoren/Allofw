// Parse config file.

function overrideObject(dest, src) {
    for(var key in src) {
        if(!src.hasOwnProperty(key)) continue;
        if(!dest[key]) {
            dest[key] = src[key];
        } else {
            if(typeof(dest[key]) == "object") {
                overrideObject(dest[key], src[key]);
            } else {
                dest[key] = src[key];
            }
        }
    }
};

function loadConfigFile(path, hostname) {
    var config = require("js-yaml").safeLoad(require("fs").readFileSync(path, 'utf8'));
    if(!hostname) hostname = require("os").hostname();
    if(config[hostname]) {
        overrideObject(config, config[hostname]);
    }
    return config;
};

utils.loadConfigFile = loadConfigFile;
