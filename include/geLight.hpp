/*
 * Eduardo Fernandes
 *
 * Light class.
 */
#ifndef LIGHTS_HPP_
#define LIGHTS_HPP_

#include "includes.hpp"

namespace ge {

class geLight {
protected:
    std::string id;
    int openGLid;
    bool enabled;

    GLfloat location[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];

    /* Internal */
    GLUquadric* gluQuadric;
    float kc, kl, kq;

public:
    /* Sets */
    void setLocation(GLfloat iX, GLfloat iY, GLfloat iZ);
    void setLocation(gePoint input);
    void setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setAmbient(geColor input);
    void setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setDiffuse(geColor input);
    void setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setSpecular(geColor input);

    void toggleEnable();

    /* Gets */
    bool getLightEnableStatus();
    std::string getID();

    virtual void draw() = 0;
    virtual void update() = 0;
    void enable();
    void disable();

    virtual ~geLight() {
    }
};

class geOmniLight: public geLight {
protected:

public:
    geOmniLight(const std::string& lightID, int openGLid, bool iEnable, gePoint iLocation, geColor iAmbient, geColor iDiffuse, geColor iSpecular);

    void draw();
    void update();
    virtual ~geOmniLight() {
    }
};

class geSpotLight: public geLight {
protected:
    GLfloat angle;
    GLfloat exponent[1];
    GLfloat direction[4];

public:
    geSpotLight(const std::string& lightID, int openGLid, bool iEnable, gePoint iLocation, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iAngle, GLfloat iExponent, gePoint iDirection);

    void setAngle(GLfloat input);
    void setExponent(GLfloat input);
    void setDirection(GLfloat iX, GLfloat iY, GLfloat iZ);
    void setDirection(gePoint input);

    void draw();
    void update();
    virtual ~geSpotLight() {
    }
};

}
#endif
