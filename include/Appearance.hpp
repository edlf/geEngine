/*
 * Eduardo Fernandes
 *
 * Appearance class.
 */

#ifndef GEAPPEARANCE_HPP_
#define GEAPPEARANCE_HPP_

#include "includes.hpp"
#include "geTexture.hpp"

#define MATERIAL_TYPE_COLOR 0
#define MATERIAL_TYPE_MATERIAL 1

using namespace std;

class Appearance {
private:
    string id;
    GLfloat emissive[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
    GLfloat colour[4];

    bool isTextured;
    string textureRef;
    geTexture* texture;

    GLfloat sWrap, tWrap;

public:
    Appearance(string iID, geColor iEmissive, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iShininess);
    Appearance(string iID, geColor iEmissive, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iShininess, string iTextureRef, GLfloat iTexlength_s, GLfloat iTexlength_t);

    void apply();

    void setEmissive(geColor input);
    void setEmissive(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setAmbient(geColor input);
    void setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setDiffuse(geColor input);
    void setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setSpecular(geColor input);
    void setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setShininess(GLfloat input);
    void setColour(geColor input);
    void setColour(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);

    /* For already loaded textures */
    void setTexture(geTexture* input);
    void setTextureWrap(GLfloat iS, GLfloat iT);

    string getTextureReference();
    string getAppearanceID();

    GLfloat getTextureSWrap();
    GLfloat getTextureTWrap();

    virtual ~Appearance();
};

#endif
