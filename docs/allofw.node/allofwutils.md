---
title: allofwutils - Allofw Documentation
layout: post
---

allofwutils
====

Utilities for allofw.node.

## Importing

```javascript
var allofwutils = require("allofwutils);
```

## allofwutils.WindowNavigation

A keyboard navigation class. Use up/down/left/right to rotate and W/X/A/D/Q/Z to move. Press "O" to reset, and "ESC" to exit.

Construct it with a OpenGLWindow and OmniStereo:

```javascript
var nav = new allofwutils.WindowNavigation(window, omni)
```

Then, in the timer function, call `nav.update()`.

## allofwutils.shape3d

D3-like 3D shape library.

### Spheres

```javascript
var spheres = shape3d.spheres();
spheres
  .attr("vec3", "center", "pos")
  .attr("float", "radius", "0.2")
  .variable("vec3", "pos", (d) -> [ d.x, d.y, d.z ])
  .compile(omni)
  .data(data)
```

### Cubes

### Images

### Texts

## allofwutils.math
