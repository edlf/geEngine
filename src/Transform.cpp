/*
 * Eduardo Fernandes
 *
 * Transform class implementation.
 */

#include "Transform.hpp"
#include <cmath>

namespace ge {

// Transform
Transform::Transform(int number) {
    transformNumber = number;
    setIndentityMatrix();
}

Transform::~Transform() { }

void Transform::setIndentityMatrix()
{
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;

    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;

    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = 0;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

GLdouble* Transform::getTransformationMatrix() {
    return matrix;
}

// TransformTranslate
TransformTranslate::TransformTranslate(xyzPointDouble in, int tN) :
        Transform::Transform(tN) {
    destination = in;
    updateMatrix();
}

TransformTranslate::TransformTranslate(GLdouble x, GLdouble y, GLdouble z, int tN) :
        Transform::Transform(tN) {
    destination.x = x;
    destination.y = y;
    destination.z = z;
    updateMatrix();
}

TransformTranslate::~TransformTranslate() { }

void TransformTranslate::updateMatrix()
{
    matrix[12] = destination.x;
    matrix[13] = destination.y;
    matrix[14] = destination.z;
}

void TransformTranslate::apply() {
    glTranslated(destination.x, destination.y, destination.z);
}

// TransformRotate
unsigned int TransformRotate::getAxisId() {
    if (axisArray.x == 1) {
        return 0;
    }
    if (axisArray.y == 1) {
        return 1;
    }
    if (axisArray.z == 1) {
        return 2;
    }
    return 0;
}

TransformRotate::TransformRotate(int iAxis, GLdouble iAngle, int tN) :
        Transform::Transform(tN) {
    axis = iAxis;
    angleRad = (iAngle * M_PI) / 180;

    axisArray.x = 0;
    axisArray.y = 0;
    axisArray.z = 0;

    angle = iAngle;
    updateMatrix();
}

TransformRotate::~TransformRotate() { }

void TransformRotate::updateMatrix()
{
    /* 0 for x, 1 for y and 2 for z */
    switch (axis) {
        case 0:
            axisArray.x = 1;

            /* X axis rotation (roll) */
            matrix[0] = 1;
            matrix[1] = 0;
            matrix[2] = 0;

            matrix[4] = 0;
            matrix[5] = cos(angleRad);
            matrix[6] = sin(angleRad);

            matrix[8] = 0;
            matrix[9] = -sin(angleRad);
            matrix[10] = cos(angleRad);
            break;

        case 1:
            axisArray.y = 1;

            /* Y axis rotation (pitch) */
            matrix[0] = cos(angleRad);
            matrix[1] = 0;
            matrix[2] = -sin(angleRad);

            matrix[4] = 0;
            matrix[5] = 1;
            matrix[6] = 0;

            matrix[8] = sin(angleRad);
            matrix[9] = 0;
            matrix[10] = cos(angleRad);
            break;

         case 2:
            axisArray.z = 1;

            /* Z axis rotation (yaw) */
            matrix[0] = cos(angleRad);
            matrix[1] = sin(angleRad);
            matrix[2] = 0;

            matrix[4] = -sin(angleRad);
            matrix[5] = cos(angleRad);
            matrix[6] = 0;

            matrix[8] = 0;
            matrix[9] = 0;
            matrix[10] = 1;
            break;

          default:
            throw Exception("Bug: Invalid value fed to axis at geTransformRotate!", true);
            break;
      }
}

void TransformRotate::apply() {
    glRotated(angle, axisArray.x, axisArray.y, axisArray.z);
}

// TransformScale
TransformScale::TransformScale(xyzPointDouble in, int tN) :
        Transform::Transform(tN) {
    scaleFactor = in;
    updateMatrix();
}

TransformScale::TransformScale(GLdouble x, GLdouble y, GLdouble z, int tN) :
        Transform::Transform(tN) {
    scaleFactor.x = x;
    scaleFactor.y = y;
    scaleFactor.z = z;
    updateMatrix();
}

TransformScale::~TransformScale() {}

void TransformScale::updateMatrix()
{
    matrix[0]  = scaleFactor.x;
    matrix[5]  = scaleFactor.y;
    matrix[10] = scaleFactor.z;
}

void TransformScale::apply() {
    glScaled(scaleFactor.x, scaleFactor.y, scaleFactor.z);
}

}

