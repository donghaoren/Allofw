#include "allofw/opengl_utils.h"
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace ALLOFW_NS { namespace glutils {

GLuint compileShaderProgram(const char* source_vertex, const char* source_fragment, Logger* logger) {
    GLuint program = glCreateProgram();
    GLuint shader_v = glCreateShader(GL_VERTEX_SHADER);
    GLuint shader_f = glCreateShader(GL_FRAGMENT_SHADER);

    bool success = true;

    glShaderSource(shader_v, 1, &source_vertex, NULL);
    glShaderSource(shader_f, 1, &source_fragment, NULL);

    glCompileShader(shader_v);
    glCompileShader(shader_f);

    GLint length, status;
    glGetShaderiv(shader_v, GL_COMPILE_STATUS, &status);
    glGetShaderiv(shader_v, GL_INFO_LOG_LENGTH, &length);
    if(status == GL_FALSE) success = false;
    if(length > 1) {
        char* infolog = new char[length];
        glGetShaderInfoLog(shader_v, length, NULL, infolog);
printf("length = %d, infolog = %s, status = %d\n", length, infolog, status);
        logger->print(Logger::kWarning, "glutils::compileShaderProgram: Vertex Shader Compile Info");
        logger->pushScope("> ");
        logger->print(Logger::kWarning, infolog);
        logger->popScope();
        delete [] infolog;
    }

    glGetShaderiv(shader_f, GL_COMPILE_STATUS, &status);
    glGetShaderiv(shader_f, GL_INFO_LOG_LENGTH, &length);
    if(status == GL_FALSE) success = false;
    if(length > 1) {
        char* infolog = new char[length];
        glGetShaderInfoLog(shader_f, length, NULL, infolog);
        logger->print(Logger::kWarning, "glutils::compileShaderProgram: Fragment Shader Compile Info");
        logger->pushScope("> ");
        logger->print(Logger::kWarning, infolog);
        logger->popScope();
        delete [] infolog;
    }

    glAttachShader(program, shader_v);
    glAttachShader(program, shader_f);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if(status == GL_FALSE) success = false;
    if (length > 1) {
        char* infolog = new char[length];
        glGetProgramInfoLog(program, length, NULL, infolog);
        logger->print(Logger::kWarning, "glutils::compileShaderProgram: Program Link Info");
        logger->pushScope("> ");
        logger->print(Logger::kWarning, infolog);
        logger->popScope();
        delete [] infolog;
    }

    glDeleteShader(shader_v);
    glDeleteShader(shader_f);

    if(!success) {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void checkGLErrors(const char* info, Logger* logger) {
    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        logger->printf(Logger::kError, "%s: glGetError = %d\n", info, err);
    }
}

} }
