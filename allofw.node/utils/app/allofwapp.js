function RunAllofwApp(info) {
    var allofw = require("allofw");
    var allofwutils = require("allofwutils");

    var config_file = info.config ? info.config : "allofw.yaml";
    var config = require("js-yaml").load(require("fs").readFileSync(config_file, "utf-8"));
    if(info.role) {
        var role = info.role;
    } else {
        var role = config.role;
    }
    var app_module = info.module;

    function StartRenderer() {
        var GL = allofw.GL3;

        var window = new allofw.OpenGLWindow({ config: config_file });
        window.makeContextCurrent();
        var omni = new allofw.OmniStereo(config_file);
        var networking = new allofwutils.Networking(config, "renderer");
        var nav = new allofwutils.WindowNavigation(window, omni);

        var app = { GL: GL, window: window, navigation: nav, omni: omni, config: config, networking: networking };
        var renderer = new app_module.renderer(app);

        // Main rendering code.
        omni.onCaptureViewport(function() {
            GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT);
            if(renderer.render) renderer.render();
        });

        // Main loop (called by timer below).
        function render() {
            omni.capture();
            sz = window.getFramebufferSize();
            omni.composite(0, 0, sz[0], sz[1]);
            window.swapBuffers();
        }

        timer = setInterval(function() {
            nav.update();
            render();
            window.pollEvents();

            if(window.shouldClose()) {
                clearInterval(timer);
            }
        });

        window.onClose(function() {
            clearInterval(timer);
        });
    }

    function StartSimulator() {
        var allofwutils = require("allofwutils");
        var networking = new allofwutils.Networking(config, "simulator");
        if(config.http) {
            var server = new allofwutils.HTTPServer(config);
        }
        var app = { server: server, config: config, networking: networking };
        var simulator = new app_module.simulator(app);
    }

    if(role == "renderer") {
        StartRenderer();
    }

    if(role == "simulator") {
        StartSimulator();
    }

    if(role == "both") {
        StartRenderer();
        StartSimulator();
    }
}

allofwutils.RunAllofwApp = RunAllofwApp;
