math.Vector3 = function(x, y, z) {
    this.x = x === undefined ? 0 : x;
    this.y = y === undefined ? 0 : y;
    this.z = z === undefined ? 0 : z;
};
math.Vector3.prototype = {
    clone: function() {
        return new math.Vector3(this.x, this.y, this.z);
    },
    add: function(v) {
        return new math.Vector3(v.x + this.x, v.y + this.y, v.z + this.z);
    },
    sub: function(v) {
        return new math.Vector3(this.x - v.x, this.y - v.y, this.z - v.z);
    },
    dot: function(v) {
        return this.x * v.x + this.y * v.y + this.z * v.z;
    },
    scale: function(s) {
        return new math.Vector3(this.x * s, this.y * s, this.z * s);
    },
    cross: function(v) {
        return new math.Vector3(
            this.y * v.z - this.z * v.y,
            this.z * v.x - this.x * v.z,
            this.x * v.y - this.y * v.x
        );
    },
    length: function() { return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z); },
    normalize: function() {
        var l = this.length();
        return new math.Vector3(this.x / l, this.y / l, this.z / l);
    },
    distance2: function(p) {
        return (this.x - p.x) * (this.x - p.x) + (this.y - p.y) * (this.y - p.y) + (this.z - p.z) * (this.z - p.z);
    },
    distance: function(p) {
        return Math.sqrt(this.distance2(p));
    },
    interp: function(v, t) {
        return new math.Vector3(this.x + (v.x - this.x) * t,
                              this.y + (v.y - this.y) * t,
                              this.z + (v.z - this.z) * t);
    },
    serialize: function() {
        return { de: "Vector3", x: this.x, y: this.y, z: this.z };
    }
};
