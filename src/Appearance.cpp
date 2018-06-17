
/*
 * Eduardo Fernandes
 *
 * Appearance class methods.
 */
extern "C" {
#include "imagetools.h"
}

#include <iostream>
#include <Appearance.hpp>

Appearance::Appearance(string iID, geColor iEmissive, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iShininess) {
    this->id = iID;

    setEmissive(iEmissive);
    setAmbient(iAmbient);
    setDiffuse(iDiffuse);
    setSpecular(iSpecular);
    setShininess(iShininess);

    this->isTextured = false;

    this->textureRef = "";

    this->texture = NULL;

    this->sWrap = -1;
    this->tWrap = -1;
}

Appearance::Appearance(string iID, geColor iEmissive, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iShininess, string iTextureRef, GLfloat iTexlength_s, GLfloat iTexlength_t) {
    this->id = iID;

    setEmissive(iEmissive);
    setAmbient(iAmbient);
    setDiffuse(iDiffuse);
    setSpecular(iSpecular);
    setShininess(iShininess);

    this->isTextured = true;

    this->textureRef = iTextureRef;

    this->texture = NULL;

    this->sWrap = iTexlength_s;
    this->tWrap = iTexlength_t;
}

void Appearance::apply() {

    glDisable(GL_COLOR_MATERIAL);
    glMaterialf(GL_FRONT, GL_SHININESS, this->shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);

    if (texture != NULL) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->sWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->tWrap);
        this->texture->apply();

    } else {
        glDisable(GL_TEXTURE_2D);

    }
}

void Appearance::setEmissive(geColor input) {
    setEmissive(input.r, input.g, input.b, input.a);
}

void Appearance::setEmissive(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->emissive[0] = iR;
    this->emissive[1] = iG;
    this->emissive[2] = iB;
    this->emissive[3] = iAlpha;
}

void Appearance::setAmbient(geColor input) {
    setAmbient(input.r, input.g, input.b, input.a);

}

void Appearance::setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->ambient[0] = iR;
    this->ambient[1] = iG;
    this->ambient[2] = iB;
    this->ambient[3] = iAlpha;
}

void Appearance::setDiffuse(geColor input) {
    setDiffuse(input.r, input.g, input.b, input.a);
}

void Appearance::setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->diffuse[0] = iR;
    this->diffuse[1] = iG;
    this->diffuse[2] = iB;
    this->diffuse[3] = iAlpha;
}

void Appearance::setSpecular(geColor input) {
    setSpecular(input.r, input.g, input.b, input.a);
}

void Appearance::setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->specular[0] = iR;
    this->specular[1] = iG;
    this->specular[2] = iB;
    this->specular[3] = iAlpha;
}

void Appearance::setColour(geColor input) {
    setColour(input.r, input.g, input.b, input.a);
}

void Appearance::setColour(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->colour[0] = iR;
    this->colour[1] = iG;
    this->colour[2] = iB;
    this->colour[3] = iAlpha;
}

void Appearance::setShininess(GLfloat input) {
    this->shininess = input;
    // this->materialType = MATERIAL_TYPE_MATERIAL;
}

void Appearance::setTexture(geTexture* input) {
    if (input != NULL) {
        this->isTextured = true;
        this->texture = input;
    }
} // FOR PRELOADED TEXTURES

void Appearance::setTextureWrap(GLfloat iS, GLfloat iT) {
    this->sWrap = iS;
    this->tWrap = iT;
}

string Appearance::getTextureReference() {
    return this->textureRef;
}

string Appearance::getAppearanceID() {
    return this->id;
}

GLfloat Appearance::getTextureSWrap() {
    return this->sWrap;
}

GLfloat Appearance::getTextureTWrap() {
    return this->tWrap;
}

Appearance::~Appearance() {

}
