/*
 * Eduardo Fernandes
 *
 * Shader class.
 */

#ifndef GESHADER_HPP
#define GESHADER_HPP

#include <GL/glew.h>

namespace ge {

class Shader {
private:
    GLuint ID;
    GLuint vertexShaderPointer;
    GLuint fragmentShaderPointer;

    int timeloc;

    unsigned int totalTimePassed;

public:
    Shader(const char *vsFile, const char *fsFile);
    virtual ~Shader();

    virtual void update(float time);

    /* Activates the shader */
    virtual void bind();
    /* Deactivates the shader, and reactivates the fixed-function pipeline */
    virtual void unbind();

    unsigned int getId();
};

}

#endif
