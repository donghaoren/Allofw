allofw.node
====

Node.js bindings to liballofw.

## Importing

    var allofw = require("allofw");

## class allofw.OpenGLWindow

`allofw.OpenGLWindow` is the class for OpenGL windows.

Create with liballofw config file `allofw.yaml`:

    var window = new allofw.OpenGLWindow({
        config: "allofw.yaml"
    });

Create with parameters:

    var window = new allofw.OpenGLWindow({
        title: "Window Title",
        width: [width],
        height: [height],
        active_stereo: [true/false],
        hide_cursor: [true/false],
        fullscreen: [true/false]
    });

Note: Fullscreen will use the current display resolution, regardless of the `width` and `height`.

**Window Manager Events**

- `onResize(f)`            : Window resize: `f = function(width, height)`.
- `onClose(f)`             : Window close: `f = function()`.
- `onRefresh(f)`           : Window refresh: `f = function()`.
- `onFocus(f)`             : Focus state changed: `f = function(bool is_focused)`.
- `onIconify(f)`           : Iconify state changed: `f = function(bool is_iconified)`.
- `onFramebufferSize(f)`   : Framebuffer size changed: `f = function(int width, int height)`.

**User Input Events**

- `onMove(f)`              : Mouse move: `f = function(int x, int y)`.
- `onKeyboard(f)`          : Keyboard event: `f = function(string key, string action, string modifiers, int scancode)`.

**Instance Methods**

- `makeContextCurrent()`   : Make the OpenGL context current.
- `swapBuffers()`          : Swap buffers.
- `pollEvents()`           : Poll for events (return if no event).
- `waitEvents()`           : Wait for events (block if no event).
- `getFramebufferSize()`   : Get the framebuffer size, return `[ width, height ]`.
- `shouldClose()`          : Should we close the window, return `true`/`false`.
- `close()`                : Mark the window as closing (`shouldClose()` will return true after this).

## allofw.GL3

OpenGL 3 Bindings. The following rules apply:

**Functions**: To call OpenGL functions, remove the "gl" prefix, and decapitalize the first letter.

    glViewport(x, y, w, h) -> GL.viewport(x, y, w, h)

**Constants**: To use OpenGL constants, remove the "GL_" prefix.

    glDisable(GL_BLEND) -> GL.disable(GL.BLEND)

**Objects**: OpenGL objects (such as shaders, textures) maps to Javascript objects.

    glCreateShader(GL_VERTEX_SHADER) -> new GL.Shader(GL.VERTEX_SHADER)
    glGenTextures(...) -> new GL.Texture()

Objects are garbage-collected. Call `obj.delete()` for manual deletion. Call `obj.id()` to get the OpenGL id for the object.

Binding functions that accepts OpenGL objects supports both `obj` and `obj.id()`. However, the OmniStereo class only accepts `obj.id()`.

**Pointers**: Javascript strings and Node.js Buffers can be used for string parameters and pointer parameters respectively.

## class allofw.OmniStereo

Create an OmniStereo object with/without config file:

    var omni = new allofw.OmniStereo();
    var omni = new allofw.OmniStereo("allofw.yaml");

**Instance Methods**

- `setPose(x, y, z, qx, qy, qz, qw)`: Set the pose with position `[x, y, z]` and quaternion `[qx, qy, qz, qw]`.
- `setLens(eye_separation, sphere_radius)`: Set the OmniStereo lens with `eye_separation` and `sphere_radius`.
- `setClipRange(near, far)`: Set the clip range to `[near, far]`.
- `getCubemapTexture()`: Get the cubemap texture. Return `[leftid, rightid]`.
- `getDepthCubemapTexture()`: Get the depth cubemap texture. Return `[leftid, rightid]`.
- `capture()`: Capture the scene (will call the callback in `onCaptureViewport` multiple times).
- `composite(vp_x, vp_y, vp_w, vp_h, info)`: Composite the scene into a viewport. info (optional): `{ panorama: [ leftid, rightid, type ("cubemap" / "equirectangular") }`.
- `setUniforms(shader_id)`: Set the uniforms for a shader.
- `getShaderCode()`: Get the sharder code prefix for OmniStereo render.
- `compositeCustomizeShader(code)`: Replace the composite shader code with `code`.
- `compositeRestoreShader()`: Restore the default composite shader code.
- `onCaptureViewport(f)`: Set capture callback. `f = function()`.

**Example**

An example with OpenGLWindow and OmniStereo.

    // Import the allofw library.
    var allofw = require("allofw");
    var GL = allofw.GL3;

    // Create the window.
    var w = new allofw.OpenGLWindow();
    w.makeContextCurrent();

    // Create the OmniStereo.
    var omni = new allofw.OmniStereo("allofw.yaml");

    // Set capture callback.
    omni.onCaptureViewport(function() {
        GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT)
        GL.enable(GL.BLEND);
        GL.blendFunc(GL.ONE, GL.ONE_MINUS_SRC_ALPHA);
        // Render something here.
    });

    // Render as fast as possible.
    var timer = setInterval(function() {
        // Capture the scene.
        omni.capture();
        // Composite the scene to the full window.
        var sz = w.getFramebufferSize();
        omni.composite(0, 0, sz[0], sz[1]);

        // Swap buffers.
        w.swapBuffers();

        // Poll window events.
        w.pollEvents();
        if(w.shouldClose()) {
            clearInterval(timer);
        }
    }, 0);

    w.onClose(function() {
        clearInterval(timer);
    });

## allofw.graphics

Bindings to the Skia graphics library for 2D graphics rendering.

## allofw.log(level, string)

    allofw.log(level, string)

Add a line of log. `level` can be one of `allofw.kInfo`, `allofw.kWarning`, `allofw.kError` and `allofw.kFatal`.

## class allofw.SharedMemory

Create the `SharedMemory` object with a key or existing shmid and semid:

    var shm = new allofw.SharedMemory(key, size, is_create)
    var shm = new allofw.SharedMemory(shmid, semid, size, is_create)

**Instance Methods**

- `size()`           : Get the byte size of the shm object.
- `shmid()`          : Get the shmid.
- `semid()`          : Get the semid.
- `buffer()`         : Get a read/write Buffer object to the shared memory.
- `delete()`         : Delete the shared memory (pair with the first constructor).
- `close()`          : Close the shared memory (pair with the second constructor).
- `writeLock()`      : Lock for writing.
- `writeUnlock()`    : Unlock for writing.
- `readLock()`       : Lock for reading.
- `readUnlock()`     : Unlock for reading.
