/*
 * Eduardo Fernandes
 *
 * Shader class methods, stolen from CGFlib.
 */

#include "includes.hpp"
#include "geShader.hpp"

namespace ge {

static char* textFileRead(const char *fileName) {
    char* text = NULL;

    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");

        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            size_t count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*) malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';
            }

            fclose(file);
        }
    }

    return text;
}

static void validateShader(GLuint shader, const char* file = 0) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if (length > 0) {
        std::cerr << "Shader " << shader << " (" << (file ? file : "") << ") compile error: " << buffer << std::endl;
    }
}

static void validateProgram(GLuint program) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    memset(buffer, 0, BUFFER_SIZE);
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        std::cerr << "Program " << program << " link error: " << buffer << std::endl;
    }

    glValidateProgram(program);

    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Error validating shader " << program << std::endl;
    }
}

geShader::geShader(const char *vsFile, const char *fsFile) {
    vertexShaderPointer = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderPointer = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vsText = textFileRead(vsFile);
    const char* fsText = textFileRead(fsFile);

    if (vsText == NULL) {
        std::cerr << "Problem reading vertex shader file: " << vsFile << std::endl;
        return;
    }

    if (fsText == NULL) {
    	std::cerr << "Problem reading fragment shader file: " << fsFile << std::endl;
        return;
    }

    glShaderSource(vertexShaderPointer, 1, &vsText, 0);
    glShaderSource(fragmentShaderPointer, 1, &fsText, 0);

    glCompileShader(vertexShaderPointer);
    validateShader(vertexShaderPointer, vsFile);
    glCompileShader(fragmentShaderPointer);
    validateShader(fragmentShaderPointer, fsFile);

    ID = glCreateProgram();
    glAttachShader(ID, fragmentShaderPointer);
    glAttachShader(ID, vertexShaderPointer);
    glLinkProgram(ID);
    validateProgram(ID);

    timeloc = glGetUniformLocation(ID, "time");
}

geShader::~geShader() {
    glDetachShader(ID, fragmentShaderPointer);
    glDetachShader(ID, vertexShaderPointer);

    glDeleteShader(fragmentShaderPointer);
    glDeleteShader(vertexShaderPointer);
    glDeleteProgram(ID);
}

unsigned int geShader::getId() {
    return ID;
}

void geShader::bind() {
    glUseProgram(ID);
}

void geShader::unbind() {
    glUseProgram(0);
}

void geShader::update(float time) {
    if (timeloc != -1) {
        glUniform1f(timeloc, (float) time);
    }
}

}
