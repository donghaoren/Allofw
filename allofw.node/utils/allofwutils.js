var allofw = require("allofw");

var math = require("./math/math.js");
var Vector3 = math.Vector3;
var Quaternion = math.Quaternion;

exports.WindowNavigation = function(window, omnistereo) {
    var pose = {
        position: new Vector3(0, 0, 0),
        rotation: new Quaternion(new Vector3(0, 0, 0), 1)
    };
    var velocity = [ 0, 0, 0, 0, 0 ];
    var keys = {
        "W"    : [ 0,  0,  0, -1, 0, 0 ],
        "X"    : [ 0,  0,  0, +1, 0, 0 ],
        "A"    : [ 0, -1,  0,  0, 0, 0 ],
        "D"    : [ 0, +1,  0,  0, 0, 0 ],
        "Q"    : [ 0,  0, +1,  0, 0, 0 ],
        "Z"    : [ 0,  0, -1,  0, 0, 0 ],
        "UP"     : [ 0,  0,  0,  0, 0, +1 ],
        "DOWN"   : [ 0,  0,  0,  0, 0, -1 ],
        "LEFT"   : [ 0,  0,  0,  0, +1, 0 ],
        "RIGHT"  : [ 0,  0,  0,  0, -1, 0 ]
    };
    var velocity_prev = [ 0, 0, 0, 0, 0 ];

    var pose_target = null;

    var update = function(dt) {
        var vs = [ 0, 0, 0, 0, 0 ];
        for(var key in keys) {
            for(var i = 0; i < 5; i++) {
                vs[i] += keys[key][i + 1] * keys[key][0];
            }
        }
        var blend = Math.pow(1 - 0.5, dt * 10);
        for(var i = 0; i < 5; i++) {
            vs[i] = velocity_prev[i] * blend + vs[i] * (1 - blend);
            velocity_prev[i] = vs[i];
        }
        var speed = 5;
        pose.position = pose.position.add(pose.rotation.rotate(new Vector3(vs[0], vs[1], vs[2])).scale(speed * dt))
        pose.rotation = Quaternion.rotation(new Vector3(0, 1, 0), vs[3] * dt).mul(pose.rotation)
        pose.rotation = pose.rotation.mul(Quaternion.rotation(new Vector3(1, 0, 0), vs[4] * dt))

        if(pose_target) {
            pose.position = pose.position.interp(pose_target.position, 1 - blend);
            pose.rotation = pose.rotation.slerp(pose_target.rotation, 1 - blend);
        }

        omnistereo.setPose(pose.position.x, pose.position.y, pose.position.z,
                           pose.rotation.v.x, pose.rotation.v.y, pose.rotation.v.z, pose.rotation.w);
    };

    var t0 = new Date().getTime() / 1000;
    this.update = function() {
        var t = new Date().getTime() / 1000;
        update(t - t0);
        t0 = t;
    };

    window.onKeyboard(function(key, action, modifiers, scancode) {
        if(key == "O") {
            pose_target = {
                position: new Vector3(0, 0, 0),
                rotation: new Quaternion(new Vector3(0, 0, 0), 1)
            };
            velocity_prev = [ 0, 0, 0, 0, 0 ];
        }
        if(action == "PRESS") {
            pose_target = null;
            if(keys[key]) {
                keys[key][0] = 1;
            }
        }
        if(action == "RELEASE") {
            if(keys[key]) {
                keys[key][0] = 0;
            }
        }
    });

};
