/*
 * Eduardo Fernandes
 *
 * Camera class, UI camera stolen from CGFlib.
 */

#ifndef GECAMERA_HPP_
#define GECAMERA_HPP_

#include "includes.hpp"

using namespace std;

namespace ge {

class CameraInterface {
protected:
    string id;
    GLfloat geNear, geFar;
    GLfloat position[3];
    GLfloat rotation[3];

public:
    CameraInterface();
    CameraInterface(string iid, GLfloat iNear, GLfloat iFar);
    string getID();

    GLfloat getNear();
    GLfloat getFar();

    virtual void applyView(double aspectRatio) = 0;
    virtual void updateProjectionMatrix(int width, int height);

    void setPosition(gePoint in);
    void setX(GLfloat x);
    void setY(GLfloat y);
    void setZ(GLfloat z);

    virtual void rotateTo(int axis, float angle) = 0;
    virtual void rotate(int axis, float angle) = 0;
    virtual void setRotation(int axis, float angle) = 0;
    virtual void moveTo(int axis, float value, float increment) = 0;
    virtual void translate(int axis, float value) = 0;

    virtual ~CameraInterface();
};

class OrthoCamera: public CameraInterface {
private:
    GLfloat left, right, top, bottom;

public:
    OrthoCamera(string iId, GLfloat iNear, GLfloat iFar, GLfloat iLeft, GLfloat iRight, GLfloat iTop, GLfloat iBottom);

    virtual void applyView(double aspectRatio);
    virtual ~OrthoCamera();

    virtual void rotateTo(int, float);
    virtual void rotate(int, float);
    virtual void setRotation(int, float);
    virtual void moveTo(int, float, float);
    virtual void translate(int, float);
};

class PerspectiveCamera: public CameraInterface {
private:
    GLfloat angle;
    GLfloat from[3], to[3];
    /* If examine mode is true, camera has been manipulated by mouse */
    bool examineMode;

public:
    /* Special camera for GUI */
    PerspectiveCamera();

    /* Regular camera for xml scene */
    PerspectiveCamera(string iId, GLfloat iNear, GLfloat iFar, GLfloat iAngle, gePoint iFrom, gePoint iTo);

    virtual void applyView(double aspectRatio);

    virtual void rotateTo(int axis, float angle);   ///< Rotates the camera around _axis_ by _increment_ degrees, unless it has reached _angle_ degrees. Useful for stepping a rotation in an animation.
    virtual void rotate(int axis, float angle);                             ///< Rotates the camera around _axis_ by _angle_ degrees.
    virtual void setRotation(int axis, float angle);                        ///< Sets the rotation around _axis_ to be _angle_ degrees
    virtual void moveTo(int axis, float value, float increment = 0.5f);       ///< Moves the camera along _axis_ by _increment_ units, unless _value_ has been reached.
    virtual void translate(int axis, float value);                          ///< Moves the camera along _axis_ by _value_ units.

    void resetCamera();

    virtual ~PerspectiveCamera();
};

}

#endif
