/*
 * Eduardo Fernandes
 *
 * Light class.
 */
#ifndef LIGHTS_HPP_
#define LIGHTS_HPP_

#include "includes.hpp"

namespace ge {

class Light {
protected:
    std::string id;
    int openGLid;
    bool enabled;
    bool lightSphereEnabled;

    GLfloat location[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];

    /* Internal */
    GLUquadric* gluQuadric;
    float kc, kl, kq;

public:
    virtual ~Light();

    /* Sets */
    void setLocation(GLfloat iX, GLfloat iY, GLfloat iZ);
    void setLocation(xyzPointFloat input);
    void setAmbient(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setAmbient(color input);
    void setDiffuse(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setDiffuse(color input);
    void setSpecular(GLfloat iR, GLfloat iG, GLfloat iB, GLfloat iAlpha);
    void setSpecular(color input);

    void toggleEnable();

    bool getLightEnableStatus();
    std::string getID();

    virtual void draw() = 0;
    virtual void update() = 0;
    void enable();
    void disable();
};

class OmniLight: public Light {
protected:

public:
    OmniLight(const std::string& lightID,
              int openGLid,
              bool iEnable,
              xyzPointFloat iLocation,
              color iAmbient,
              color iDiffuse,
              color iSpecular);
    virtual ~OmniLight();

    void draw();
    void update();
};

class SpotLight: public Light {
protected:
    GLdouble angle;
    GLfloat exponent[1];
    GLfloat direction[4];

public:
    SpotLight(const std::string& lightID,
              int openGLid,
              bool iEnable,
              xyzPointFloat iLocation,
              color iAmbient,
              color iDiffuse,
              color iSpecular,
              GLfloat iAngle,
              GLfloat iExponent,
              xyzPointFloat iDirection);
    virtual ~SpotLight();

    void setAngle(GLdouble input);
    void setExponent(GLfloat input);
    void setDirection(GLfloat iX, GLfloat iY, GLfloat iZ);
    void setDirection(xyzPointFloat input);

    void draw();
    void update();
};

}
#endif
