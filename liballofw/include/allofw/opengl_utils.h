#ifndef ALLOFW_OPENGL_UTILS_H
#define ALLOFW_OPENGL_UTILS_H

#include "opengl.h"
#include "logger.h"

namespace allofw {
    namespace glutils {
        // Print glGetError()
        void checkGLErrors(const char* info, Logger* logger = Logger::Default());
        // Compile shader program, return 0 if failed.
        GLuint compileShaderProgram(const char* source_vertex, const char* source_fragment, Logger* logger = Logger::Default());
        GLuint compileShaderProgram(const char* source_vertex, const char* source_geometry, const char* source_fragment, Logger* logger = Logger::Default());
    }
}

#endif
