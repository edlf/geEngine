/*
 * Eduardo Fernandes
 *
 * Camera class methods.
 */

#include <Camera.hpp>

CameraInterface::CameraInterface(){

}

CameraInterface::CameraInterface(string iId, GLfloat iNear, GLfloat iFar) {
    this->id = iId;
    this->geNear = iNear;
    this->geFar = iFar;
}

string CameraInterface::getID() {
    return this->id;
}

GLfloat CameraInterface::getNear() {
    return this->geNear;
}

GLfloat CameraInterface::getFar() {
    return this->geFar;
}

void CameraInterface::updateProjectionMatrix(int width, int height) {
    float aspect = (float) width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspect * .04, aspect * .04, -.04, .04, .4, 500.0);
}

void CameraInterface::setPosition(gePoint in) {
    setX(in.x);
    setY(in.y);
    setZ(in.z);
}

void CameraInterface::setX(GLfloat x) {
    this->position[0] = x;
}

void CameraInterface::setY(GLfloat y) {
    this->position[1] = y;
}

void CameraInterface::setZ(GLfloat z) {
    this->position[2] = z;
}

CameraInterface::~CameraInterface() {

}

OrthoCamera::OrthoCamera(string iId, GLfloat iNear, GLfloat iFar, GLfloat iLeft, GLfloat iRight, GLfloat iTop, GLfloat iBottom) :
        CameraInterface(iId, iNear, iFar) {

    this->left = iLeft;
    this->right = iRight;
    this->top = iTop;
    this->bottom = iBottom;
}

void OrthoCamera::applyView(double) {
    glOrtho(this->left, this->right, this->bottom, this->top, this->geNear, this->geFar);
}

OrthoCamera::~OrthoCamera() {

}

void OrthoCamera::rotateTo(int, float){

}

void OrthoCamera::rotate(int, float) {
}

void OrthoCamera::setRotation(int, float) {
}

void OrthoCamera::moveTo(int, float, float) {

}

void OrthoCamera::translate(int, float) {

}

PerspectiveCamera::PerspectiveCamera() {
    angle = 0;

    position[0] = 0;
    position[1] = 0;
    position[2] = -25;

    rotation[0] = 0;
    rotation[1] = 0;
    rotation[2] = 0;

    to[0] = 0;
    to[1] = 0;
    to[2] = 0;

    this->examineMode = true;
}

PerspectiveCamera::PerspectiveCamera(string iId, GLfloat iNear, GLfloat iFar, GLfloat iAngle, gePoint iFrom, gePoint iTo) :
        CameraInterface(iId, iNear, iFar) {

    this->angle = iAngle;
    this->from[0] = iFrom.x;
    this->from[1] = iFrom.y;
    this->from[2] = iFrom.z;
    this->to[0] = iTo.x;
    this->to[1] = iTo.y;
    this->to[2] = iTo.z;

    this->examineMode = false;
}

void PerspectiveCamera::applyView(double aspectRatio) {
    if (this->examineMode) {
        glFrustum(-aspectRatio * .04, aspectRatio * .04, -.04, .04, .1, 500.0);

        glTranslatef(position[0], position[1], position[2]);

        glRotatef(rotation[0], 1.f, 0.f, 0.f);
        glRotatef(rotation[1], 0.f, 1.f, 0.f);
        glRotatef(rotation[2], 0.f, 0.f, 1.f);

    } else {

        gluPerspective(this->angle, aspectRatio, this->geNear, this->geFar);
        gluLookAt(this->from[0], this->from[1], this->from[2], this->to[0], this->to[1], this->to[2], 0, 1, 0);

    }
}

void PerspectiveCamera::rotateTo(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        if (rotation[axis] < angle) {
            rotation[axis] += 0.5f;
        }
    }
}

void PerspectiveCamera::rotate(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        rotation[axis] += angle;
    }
}

void PerspectiveCamera::setRotation(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        rotation[axis] = angle;
    }
}

void PerspectiveCamera::moveTo(int axis, float value, float increment) {
    if (axis >= 0 && axis <= 2) {
        if (position[axis] < value) {
            rotation[axis] += increment;
        }
    }
}

void PerspectiveCamera::translate(int axis, float value) {
    this->examineMode = true;
    ///< Moves the camera along _axis_ by _value_ units.
    if (axis >= 0 && axis <= 2) {
        position[axis] += value;
    }
}

void PerspectiveCamera::resetCamera() {
    angle = 0;

    position[0] = 0;
    position[1] = 0;
    position[2] = -25;

    rotation[0] = 0;
    rotation[1] = 0;
    rotation[2] = 0;

    to[0] = 0;
    to[1] = 0;
    to[2] = 0;
}

PerspectiveCamera::~PerspectiveCamera() {

}
