math.Quaternion = function(v, w) {
    this.v = v !== undefined ? v : new math.Vector3(0, 0, 0);
    this.w = w === undefined ? 0 : w;
};
math.Quaternion.prototype.conj = function() {
    return new math.Quaternion(this.v.scale(-1), this.w);
};
math.Quaternion.prototype.mul = function(q2) {
    var w = this.w * q2.w - this.v.dot(q2.v);
    var v = q2.v.scale(this.w).add(this.v.scale(q2.w)).add(this.v.cross(q2.v));
    return new math.Quaternion(v, w);
};
math.Quaternion.prototype.rotate = function(vector) {
    var vq = new math.Quaternion(vector, 0);
    return this.mul(vq).mul(this.conj()).v;
};
math.Quaternion.rotation = function(axis, angle) {
    return new math.Quaternion(axis.normalize().scale(Math.sin(angle / 2)), Math.cos(angle / 2));
};
math.Quaternion.prototype.normalize = function() {
    var s = 1.0 / Math.sqrt(this.v.x * this.v.x + this.v.y * this.v.y + this.v.z * this.v.z + this.w * this.w);
    return new math.Quaternion(this.v.scale(s), this.w * s);
};
math.Quaternion.slerp = function(q1, q2, t) {
    var acos_arg = q1.v.x * q2.v.x + q1.v.y * q2.v.y + q1.v.z * q2.v.z + q1.w * q2.w;
    if(acos_arg > 1) acos_arg = 1;
    if(acos_arg < -1) acos_arg = -1;
    var omega = Math.acos(acos_arg);
    var st0, st1;
    if(Math.abs(omega) < 1e-10) {
        st0 = 1 - t;
        st1 = t;
    } else {
        var som = Math.sin(omega);
        st0 = Math.sin((1 - t) * omega) / som;
        st1 = Math.sin(t * omega) / som;
    }
    return new math.Quaternion(
        new math.Vector3(
            q1.v.x * st0 + q2.v.x * st1,
            q1.v.y * st0 + q2.v.y * st1,
            q1.v.z * st0 + q2.v.z * st1
        ),
        q1.w * st0 + q2.w * st1
    );
};
math.Quaternion.prototype.slerp = function(q2, t) {
    return math.Quaternion.slerp(this, q2, t);
};
