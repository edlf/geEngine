/*
 * Eduardo Fernandes
 *
 * Camera class methods.
 */

#include "geCamera.hpp"

geCamera::geCamera(string iId, GLfloat iNear, GLfloat iFar) {
    this->id = iId;
    this->geNear = iNear;
    this->geFar = iFar;
}

string geCamera::getID() {
    return this->id;
}

GLfloat geCamera::getNear() {
    return this->geNear;
}

GLfloat geCamera::getFar() {
    return this->geFar;
}

void geCamera::applyView(double aspectRatio) {
    /* Empty function, look at ortho or perspective cameras */
}

void geCamera::updateProjectionMatrix(int width, int height) {
    float aspect = (float) width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspect * .04, aspect * .04, -.04, .04, .4, 500.0);
}

void geCamera::setPosition(gePoint in) {
    setX(in.x);
    setY(in.y);
    setZ(in.z);
}

void geCamera::setX(GLfloat x) {
    this->position[0] = x;
}

void geCamera::setY(GLfloat y) {
    this->position[1] = y;
}

void geCamera::setZ(GLfloat z) {
    this->position[2] = z;
}

geCamera::~geCamera() {

}

geCameraOrtho::geCameraOrtho(string iId, GLfloat iNear, GLfloat iFar, GLfloat iLeft, GLfloat iRight, GLfloat iTop, GLfloat iBottom) :
        geCamera(iId, iNear, iFar) {

    this->left = iLeft;
    this->right = iRight;
    this->top = iTop;
    this->bottom = iBottom;
}

void geCameraOrtho::applyView(double aspectRatio) {
    glOrtho(this->left, this->right, this->bottom, this->top, this->geNear, this->geFar);
}

geCameraOrtho::~geCameraOrtho() {

}

geCameraPerspective::geCameraPerspective() {
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

geCameraPerspective::geCameraPerspective(string iId, GLfloat iNear, GLfloat iFar, GLfloat iAngle, gePoint iFrom, gePoint iTo) :
        geCamera(iId, iNear, iFar) {

    this->angle = iAngle;
    this->from[0] = iFrom.x;
    this->from[1] = iFrom.y;
    this->from[2] = iFrom.z;
    this->to[0] = iTo.x;
    this->to[1] = iTo.y;
    this->to[2] = iTo.z;

    this->examineMode = false;
}

void geCameraPerspective::applyView(double aspectRatio) {
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

void geCameraPerspective::rotateTo(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        if (rotation[axis] < angle) {
            rotation[axis] += 0.5f;
        }
    }
}

void geCameraPerspective::rotate(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        rotation[axis] += angle;
    }
}

void geCameraPerspective::setRotation(int axis, float angle) {
    if (axis >= 0 && axis <= 2) {
        rotation[axis] = angle;
    }
}

void geCameraPerspective::moveTo(int axis, float value, float increment) {
    if (axis >= 0 && axis <= 2) {
        if (position[axis] < value) {
            rotation[axis] += increment;
        }
    }
}

void geCameraPerspective::translate(int axis, float value) {
    this->examineMode = true;
    ///< Moves the camera along _axis_ by _value_ units.
    if (axis >= 0 && axis <= 2) {
        position[axis] += value;
    }
}

void geCameraPerspective::resetCamera() {
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

geCameraPerspective::~geCameraPerspective() {

}
