/*
 * Eduardo Fernandes
 *
 * Light class methods.
 */

#include <Primitives.hpp>
#include "geLight.hpp"

namespace ge {
geOmniLight::geOmniLight(const std::string& lightID, int openGLid, bool iEnable, xyzPointFloat iLocation, color iAmbient, color iDiffuse, color iSpecular) {

    this->id = lightID;
    this->openGLid = GL_LIGHT0 + openGLid;
    this->enabled = iEnable;

    setLocation(iLocation);
    setAmbient(iAmbient);
    setDiffuse(iDiffuse);
    setSpecular(iSpecular);

    gluQuadric = gluNewQuadric();
}

geSpotLight::geSpotLight(const std::string& lightID, int openGLid, bool iEnable, xyzPointFloat iLocation, color iAmbient, color iDiffuse, color iSpecular, GLfloat iSpotAngle, GLfloat iSpotExponent,
        xyzPointFloat iSpotDirection) {

    this->id = lightID;
    this->openGLid = GL_LIGHT0 + openGLid;
    this->enabled = iEnable;

    setLocation(iLocation);
    setAmbient(iAmbient);
    setDiffuse(iDiffuse);
    setSpecular(iSpecular);

    setAngle(iSpotAngle);
    setExponent(iSpotExponent);
    setDirection(iSpotDirection);

    gluQuadric = gluNewQuadric();
}

/* Sets */
void geLight::setLocation(GLfloat iX, GLfloat iY, GLfloat iZ) {
    this->location[0] = iX;
    this->location[1] = iY;
    this->location[2] = iZ;
    this->location[3] = 1.0f;
}

void geLight::setLocation(xyzPointFloat input) {
    setLocation(input.x, input.y, input.z);
}

void geLight::setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->ambient[0] = iR;
    this->ambient[1] = iG;
    this->ambient[2] = iB;
    this->ambient[3] = iAlpha;
}

void geLight::setAmbient(color input) {
    setAmbient(input.r, input.g, input.b, input.a);
}

void geLight::setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->diffuse[0] = iR;
    this->diffuse[1] = iG;
    this->diffuse[2] = iB;
    this->diffuse[3] = iAlpha;
}

void geLight::setDiffuse(color input) {
    setDiffuse(input.r, input.g, input.b, input.a);
}

void geLight::setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha) {
    this->specular[0] = iR;
    this->specular[1] = iG;
    this->specular[2] = iB;
    this->specular[3] = iAlpha;
}

void geLight::setSpecular(color input) {
    setSpecular(input.r, input.g, input.b, input.a);

}

void geSpotLight::setAngle(GLdouble input) {
    this->angle = input;
}

void geSpotLight::setExponent(GLfloat input) {
    this->exponent[0] = input;
}

void geSpotLight::setDirection(GLfloat iX, GLfloat iY, GLfloat iZ) {
    this->direction[0] = iX;
    this->direction[1] = iY;
    this->direction[2] = iZ;
    this->direction[3] = 0.0f;
}

void geSpotLight::setDirection(xyzPointFloat input) {
    setDirection(input.x, input.y, input.z);
}

void geLight::toggleEnable() {
    this->enabled = !(this->enabled);
}

/* Gets */
bool geLight::getLightEnableStatus() {
    return enabled;
}

std::string geLight::getID() {
    return id;
}

void geOmniLight::draw() {
    update();
#ifdef LIGHT_SPHERE_ENABLED
    /* Draw sphere in the light location */
    glPushMatrix ();
    glTranslatef (location [0], location [1], location [2]);
    gluSphere (gluQuadric, 0.1, 16, 16);
    glPopMatrix ();
#endif
}

void geSpotLight::draw() {
    update();
#ifdef LIGHT_SPHERE_ENABLED
    /* Draw sphere in the light location */
    glPushMatrix ();
    glTranslatef (location [0], location [1], location [2]);
    gluSphere (gluQuadric, 0.1, 16, 16);
    glPopMatrix ();
#endif
}

void geOmniLight::update() {
    if (enabled) {
        glEnable(openGLid);

        glLightfv(openGLid, GL_AMBIENT, ambient);
        glLightfv(openGLid, GL_DIFFUSE, diffuse);
        glLightfv(openGLid, GL_SPECULAR, specular);
        glLightfv(openGLid, GL_POSITION, location);

    } else {
        glDisable(openGLid);
    }

}

void geSpotLight::update() {
    if (enabled) {
        glEnable(openGLid);

        glLightfv(openGLid, GL_AMBIENT, ambient);
        glLightfv(openGLid, GL_DIFFUSE, diffuse);
        glLightfv(openGLid, GL_SPECULAR, specular);
        glLightfv(openGLid, GL_POSITION, location);
        glLightfv(openGLid, GL_SPOT_DIRECTION, direction);
        glLightfv(openGLid, GL_SPOT_EXPONENT, exponent);

    } else {
        glDisable(openGLid);
    }

}

void geLight::enable() {
    this->enabled = true;
    update();
}

void geLight::disable() {
    this->enabled = false;
    update();
}

}
