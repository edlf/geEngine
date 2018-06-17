/*
 * Eduardo Fernandes
 *
 * Texture class, part stolen from CGFlib.
 */

#ifndef GELTEXTURE_HPP_
#define GELTEXTURE_HPP_

#include "includes.hpp"

#include <iostream>
#include <string>

#include <imagetools.hpp>

namespace ge {

class geTexture {
private:
    // Attributes
    GLuint idOpenGL;
    int width, height;
    std::string fileName;
    std::string xmlId;

    bool loaded;

public:
    geTexture(std::string& xmlIdIn, std::string& input);

    GLuint getIdOpenGL();
    int getWidth();
    int getHeight();

    void loadTexture();
    void apply();

    std::string getXmlId();

    virtual ~geTexture();
};

}

#endif
