/*
 * Eduardo Fernandes
 *
 * Texture class methods.
 */

#include "geTexture.hpp"

namespace ge {

geTexture::geTexture(std::string& xmlIdIn, std::string& input) {
    this->loaded = false;
    this->xmlId = xmlIdIn;
    this->fileName = input;
}

GLuint geTexture::getIdOpenGL() {
    return this->idOpenGL;
}

int geTexture::getWidth() {
    return this->width;
}

int geTexture::getHeight() {
    return this->height;
}

void geTexture::loadTexture() {

    if (!this->fileName.empty()) {

        if (!this->loaded) {
            glGenTextures(1, &idOpenGL);
            this->loaded = true;
        }

        std::string pathFileName = std::string(TEXTURE_FOLDER) + this->fileName;

        unsigned char *data = loadRGBImage(pathFileName.c_str(), &this->width, &this->height);

        if (data) {

            glPixelStorei(GL_UNPACK_ROW_LENGTH, this->width);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glBindTexture(GL_TEXTURE_2D, this->idOpenGL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, data);

            free(data);

        } else {
            throw geException("The texture could not be loaded: [" + this->fileName + "]", true);
        }
    }
}

void geTexture::apply() {
    if (this->loaded) {
        glBindTexture(GL_TEXTURE_2D, idOpenGL);
    }
}

std::string geTexture::getXmlId() {
    return this->xmlId;
}

geTexture::~geTexture() {

}

}
