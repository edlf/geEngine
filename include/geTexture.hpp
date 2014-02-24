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

#include "imagetools.h"

using namespace std;

class geTexture {
private:
    // Attributes
    GLuint idOpenGL;
    int width, height;
    string fileName;
    string xmlId;

    bool loaded;

public:
    geTexture(string xmlIdIn, string input);

    GLuint getIdOpenGL();
    int getWidth();
    int getHeight();

    void loadTexture();
    void apply();

    string getXmlId();

    virtual ~geTexture();
};

#endif
