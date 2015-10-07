function FPSCounter() {
    this.t_previous_frame = new Date().getTime();
    this.last_print = 0;
    this.fps_measures = [];
}
FPSCounter.prototype.update = function() {
    var t = new Date().getTime();
    var fps = 1000.0 / (t - this.t_previous_frame);
    this.fps_measures.push(fps);
    if(this.fps_measures.length > 100) {
        this.fps_measures.splice(0, 1);
    }
    if(t - this.last_print > 5000 && this.fps_measures.length > 0) {
        var fps_average = this.fps_measures.reduce(function(a, b) { return a + b; }) / this.fps_measures.length;
        console.log("FPS: " + fps_average);
        this.last_print = t;
    }
    this.t_previous_frame = t;
};

exports.FPSCounter = FPSCounter;
