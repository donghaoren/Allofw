#ifndef ALLOFW_OPENGL_UTILS_H
#define ALLOFW_OPENGL_UTILS_H

#include "opengl.h"
#include "logger.h"

namespace allofw {
    namespace glutils {
        //! Print glGetError() to the logger.
        //! \param info: The description.
        //! \param logger: The logger object.
        void checkGLErrors(const char* info, Logger* logger = Logger::Default());

        //! Compile shader program.
        //! @param source_vertex: The source of the vertex shader.
        //! @param source_fragment: The source of the fragment shader.
        //! @param logger: The logger object.
        //! @return The program ID, or 0 if failed.
        GLuint compileShaderProgram(const char* source_vertex, const char* source_fragment, Logger* logger = Logger::Default());

        //! Compile shader program with gemoetry shader.
        //! @param source_vertex: The source of the vertex shader.
        //! @param source_geometry: The source of the geometry shader.
        //! @param source_fragment: The source of the fragment shader.
        //! @param logger: The logger object.
        //! @return The program ID, or 0 if failed.
        GLuint compileShaderProgram(const char* source_vertex, const char* source_geometry, const char* source_fragment, Logger* logger = Logger::Default());
    }
}

#endif
