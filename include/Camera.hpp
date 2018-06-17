/*
 * Eduardo Fernandes
 *
 * Camera class, UI camera stolen from CGFlib.
 */

#ifndef GECAMERA_HPP_
#define GECAMERA_HPP_

#include "includes.hpp"

namespace ge {

class CameraInterface {
protected:
    std::string id;
    GLdouble geNear, geFar;
    GLdouble position[3];
    GLdouble rotation[3];

public:
    CameraInterface();
    CameraInterface(std::string& iid, GLdouble iNear, GLdouble iFar);
    std::string getID();

    GLdouble getNear();
    GLdouble getFar();

    virtual void applyView(double aspectRatio) = 0;
    virtual void updateProjectionMatrix(int width, int height);

    void setPosition(xyzPointDouble in);
    void setX(GLdouble x);
    void setY(GLdouble y);
    void setZ(GLdouble z);

    virtual void rotateTo(int axis, double angle) = 0;
    virtual void rotate(int axis, double angle) = 0;
    virtual void setRotation(int axis, double angle) = 0;
    virtual void moveTo(int axis, double value, double increment) = 0;
    virtual void translate(int axis, double value) = 0;

    virtual ~CameraInterface();
};

class OrthoCamera: public CameraInterface {
private:
    GLdouble left, right, top, bottom;

public:
    OrthoCamera(std::string& iId, GLdouble iNear, GLdouble iFar, GLdouble iLeft,
            GLdouble iRight, GLdouble iTop, GLdouble iBottom);

    virtual void applyView(double aspectRatio);
    virtual ~OrthoCamera();

    virtual void rotateTo(int, double);
    virtual void rotate(int, double);
    virtual void setRotation(int, double);
    virtual void moveTo(int, double, double);
    virtual void translate(int, double);
};

/* Special camera for GUI */
class PerspectiveCamera: public CameraInterface {
private:
    GLdouble angle;
    GLdouble from[3], to[3];
    /* If examine mode is true, camera has been manipulated by mouse */
    bool examineMode;

public:
    PerspectiveCamera();
    virtual ~PerspectiveCamera();

    /* Regular camera for xml scene */
    PerspectiveCamera(std::string& iId, GLdouble iNear, GLdouble iFar,
            GLdouble iAngle, xyzPointDouble iFrom, xyzPointDouble iTo);

    virtual void applyView(double aspectRatio);

    virtual void rotateTo(int axis, double angle); ///< Rotates the camera around _axis_ by _increment_ degrees, unless it has reached _angle_ degrees. Useful for stepping a rotation in an animation.
    virtual void rotate(int axis, double angle); ///< Rotates the camera around _axis_ by _angle_ degrees.
    virtual void setRotation(int axis, double angle); ///< Sets the rotation around _axis_ to be _angle_ degrees
    virtual void moveTo(int axis, double value, double increment = 0.5f); ///< Moves the camera along _axis_ by _increment_ units, unless _value_ has been reached.
    virtual void translate(int axis, double value); ///< Moves the camera along _axis_ by _value_ units.

    void resetCamera();
};

}

#endif
