/*
 * Eduardo Fernandes
 *
 * Light class.
 */

#include "includes.hpp"

using namespace std;

class geLight {
protected:
    string id;
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
    string getID();

    virtual void draw() {
    } ///< Draws a visible representation of the light (a sphere). It forces an update() of the light.
    virtual void update() {
    } ///< Updates the position and direction of the corresponding OpenGL's light. Has to be called after any camera transformations, so that the light is transformed in the proper referential.
    void enable();
    void disable();

    virtual ~geLight() {
    }
};

class geOmniLight: public geLight {
protected:

public:
    geOmniLight(string lightID, int openGLid, bool iEnable, gePoint iLocation, geColor iAmbient, geColor iDiffuse, geColor iSpecular);

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
    geSpotLight(string lightID, int openGLid, bool iEnable, gePoint iLocation, geColor iAmbient, geColor iDiffuse, geColor iSpecular, GLfloat iAngle, GLfloat iExponent, gePoint iDirection);

    void setAngle(GLfloat input);
    void setExponent(GLfloat input);
    void setDirection(GLfloat iX, GLfloat iY, GLfloat iZ);
    void setDirection(gePoint input);

    void draw();
    void update();
    virtual ~geSpotLight() {
    }
};
