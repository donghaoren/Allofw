allofw.node
====

Node.js bindings to liballofw.


Importing
----

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

**Window manager events:**

- `onResize(f)`            : Window resize: `f = function(width, height)`
- `onClose(f)`             : Window close: `f = function()`
- `onRefresh(f)`           : Window refresh: `f = function()`
- `onFocus(f)`             : Focus state changed: `f = function(bool is_focused)`
- `onIconify(f)`           : Iconify state changed: `f = function(bool is_iconified)`
- `onFramebufferSize(f)`   : Framebuffer size changed: `f = function(int width, int height)`
    
**User input events:**

- `onMove(f)`              : Mouse move: `f = function(int x, int y)`
- `onKeyboard(f)`          : Keyboard event: `f = function(string key, string action, string modifiers, int scancode)`
    
**Methods:**

- `makeContextCurrent()`   : Make the OpenGL context current.
- `swapBuffers()`          : Swap buffers.
- `pollEvents()`           : Poll for events (return if no event).
- `waitEvents()`           : Wait for events (block if no event).
- `getFramebufferSize()`   : Get the framebuffer size, return `[ width, height ]`.
- `shouldClose()`          : Should we close the window, return `true`/`false`.
- `close()`                : Mark the window as closing (`shouldClose()` will return true after this).

## class allofw.OmniStereo



## allofw.GL3

OpenGL 3 Bindings.

## allofw.graphics

Bindings to the Skia graphics library for 2D graphics rendering.


Logging
----

### allofw.log(level, string)

    allofw.log(level, string)

Add a line of log. `level` can be one of `allofw.kInfo`, `allofw.kWarning`, `allofw.kError`, `allofw.kFatal`.

Shared Memory
----

### class allofw.SharedMemory

Create the `SharedMemory` object with a key or existing shmid and semid:

    var shm = new allofw.SharedMemory(key, size, is_create)
    var shm = new allofw.SharedMemory(shmid, semid, size, is_create)
    
#### Instance Methods

	shm.size()	         : Get the byte size of the shm object.
	shm.shmid()          : Get the shmid.
	shm.semid()          : Get the semid.
	shm.buffer()         : Get a read/write Buffer object to the shared memory.
	shm.delete()         : Delete the shared memory (pair with the first constructor).
	shm.close()          : Close the shared memory (pair with the second constructor).
	shm.writeLock()      : Lock for writing.
	shm.writeUnlock()    : Unlock for writing.
	shm.readLock()       : Lock for reading.
	shm.readUnlock()     : Unlock for reading.