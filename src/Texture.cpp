/*
 * Eduardo Fernandes
 *
 * Texture class methods.
 */

#include <Texture.hpp>

namespace ge {

const std::string TextureFolder = "textures/";

Texture::Texture(std::string& xmlIdIn, std::string& input) {
    this->loaded = false;
    this->xmlId = xmlIdIn;
    this->fileName = input;
}

GLuint Texture::getIdOpenGL() {
    return this->idOpenGL;
}

int Texture::getWidth() {
    return this->width;
}

int Texture::getHeight() {
    return this->height;
}

void Texture::loadTexture() {

    if (!this->fileName.empty()) {

        if (!this->loaded) {
            glGenTextures(1, &idOpenGL);
            this->loaded = true;
        }

        std::string pathFileName = TextureFolder + this->fileName;

        unsigned char *data = loadRGBImage(pathFileName.c_str(), &this->width,
                &this->height);

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
            throw Exception(
                    "The texture could not be loaded: [" + this->fileName + "]",
                    true);
        }
    }
}

void Texture::apply() {
    if (this->loaded) {
        glBindTexture(GL_TEXTURE_2D, idOpenGL);
    }
}

std::string Texture::getXmlId() {
    return this->xmlId;
}

Texture::~Texture() {

}

}
