/*
 * Eduardo Fernandes
 *
 * Camera class, UI camera stolen from CGFlib.
 */

#ifndef GECAMERA_HPP_
#define GECAMERA_HPP_

#include "includes.hpp"

using namespace std;

class geCamera {
protected:
    string id;
    GLfloat geNear, geFar;
    GLfloat position[3];
    GLfloat rotation[3];

public:
    geCamera() {
    }
    geCamera(string iid, GLfloat iNear, GLfloat iFar);
    string getID();

    GLfloat getNear();
    GLfloat getFar();

    /* applyView must be overridden, else nothing will happen */
    virtual void applyView(double aspectRatio);
    virtual void updateProjectionMatrix(int width, int height);

    void setPosition(gePoint in);
    void setX(GLfloat x);
    void setY(GLfloat y);
    void setZ(GLfloat z);

    virtual void rotateTo(int axis, float angle) {
    }
    virtual void rotate(int axis, float angle) {
    }
    virtual void setRotation(int axis, float angle) {
    }
    virtual void moveTo(int axis, float value, float increment) {
    }
    virtual void translate(int axis, float value) {
    }

    virtual ~geCamera();
};

class geCameraOrtho: public geCamera {
private:
    GLfloat left, right, top, bottom;

public:
    geCameraOrtho(string iId, GLfloat iNear, GLfloat iFar, GLfloat iLeft, GLfloat iRight, GLfloat iTop, GLfloat iBottom);

    virtual void applyView(double aspectRatio);
    virtual ~geCameraOrtho();
};

class geCameraPerspective: public geCamera {
private:
    GLfloat angle;
    GLfloat from[3], to[3];
    /* If examine mode is true, camera has been manipulated by mouse */
    bool examineMode;

public:
    /* Special camera for GUI */
    geCameraPerspective();

    /* Regular camera for xml scene */
    geCameraPerspective(string iId, GLfloat iNear, GLfloat iFar, GLfloat iAngle, gePoint iFrom, gePoint iTo);

    virtual void applyView(double aspectRatio);

    virtual void rotateTo(int axis, float angle);   ///< Rotates the camera around _axis_ by _increment_ degrees, unless it has reached _angle_ degrees. Useful for stepping a rotation in an animation.
    virtual void rotate(int axis, float angle);                             ///< Rotates the camera around _axis_ by _angle_ degrees.
    virtual void setRotation(int axis, float angle);                        ///< Sets the rotation around _axis_ to be _angle_ degrees
    virtual void moveTo(int axis, float value, float increment = 0.5f);       ///< Moves the camera along _axis_ by _increment_ units, unless _value_ has been reached.
    virtual void translate(int axis, float value);                          ///< Moves the camera along _axis_ by _value_ units.

    void resetCamera();

    virtual ~geCameraPerspective();
};

#endif
