/*
 * Eduardo Fernandes
 *
 * Appearance class.
 */

#ifndef GEAPPEARANCE_HPP_
#define GEAPPEARANCE_HPP_

#include <Texture.hpp>
#include "includes.hpp"

#define MATERIAL_TYPE_COLOR 0
#define MATERIAL_TYPE_MATERIAL 1

namespace ge {

class Appearance {
private:
    std::string id;
    GLdouble emissive[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
    GLdouble colour[4];

    bool isTextured;
    std::string textureRef;
    Texture* texture;

    GLfloat sWrap, tWrap;

public:
    Appearance(const std::string& iID, color iEmissive, color iAmbient,
            color iDiffuse, color iSpecular, GLfloat iShininess);
    Appearance(const std::string& iID, color iEmissive, color iAmbient,
            color iDiffuse, color iSpecular, GLfloat iShininess,
            const std::string& iTextureRef, GLfloat iTexlength_s,
            GLfloat iTexlength_t);
    virtual ~Appearance();

    void apply();

    void setEmissive(color input);
    void setEmissive(GLdouble iR, GLdouble iG, GLdouble iB, GLdouble iAlpha);
    void setAmbient(color input);
    void setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setDiffuse(color input);
    void setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setSpecular(color input);
    void setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setShininess(GLfloat input);
    void setColour(color input);
    void setColour(GLdouble iR, GLdouble iG, GLdouble iB, GLdouble iAlpha);

    /* For already loaded textures */
    void setTexture(Texture* input);
    void setTextureWrap(GLfloat iS, GLfloat iT);

    std::string getTextureReference();
    std::string getAppearanceID();

    GLdouble getTextureSWrap();
    GLdouble getTextureTWrap();
};

} // namespace ge

#endif
