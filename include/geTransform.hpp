/*
 * Eduardo Fernandes
 *
 * Transform class.
 */

#ifndef GETRANSFORM_HPP_
#define GETRANSFORM_HPP_

#include "includes.hpp"

namespace ge {
class geTransform {
protected:
    int transformNumber;
    GLfloat matrix[16];

public:
    geTransform(int number) {
        transformNumber = number;
    }
    ~geTransform() {}

    virtual GLfloat* getTransformationMatrix() = 0;
    virtual void apply() = 0;
};

class geTransformTranslate: public geTransform {
private:
    GLfloat destination[3];

public:
    geTransformTranslate(gePoint in, int tN) :
            geTransform(tN) {
        destination[0] = in.x;
        destination[1] = in.y;
        destination[2] = in.z;

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

        matrix[12] = destination[0];
        matrix[13] = destination[1];
        matrix[14] = destination[2];
        matrix[15] = 1;
    }

    geTransformTranslate(GLfloat x, GLfloat y, GLfloat z, int tN) :
            geTransform(tN) {
        destination[0] = x;
        destination[1] = y;
        destination[2] = z;

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

        matrix[12] = destination[0];
        matrix[13] = destination[1];
        matrix[14] = destination[2];
        matrix[15] = 1;
    }

    GLfloat* getTransformationMatrix() {
        return matrix;
    }

    void apply() {
        glTranslatef(destination[0], destination[1], destination[2]);
    }
};

class geTransformRotate: public geTransform {
private:
    GLfloat axisArray[3];
    GLfloat angle;
    GLfloat angleRad;

    unsigned int getAxisId() {
        if (axisArray[0] == 1) {
            return 0;
        }

        if (axisArray[1] == 1) {
            return 1;
        }

        if (axisArray[2] == 1) {
            return 2;
        }
        return 0;
    }

public:
    geTransformRotate(int iAxis, GLfloat iAngle, int tN) :
            geTransform(tN) {
        angleRad = (iAngle * M_PI) / 180;

        axisArray[0] = 0;
        axisArray[1] = 0;
        axisArray[2] = 0;

        /* 0 for x, 1 for y and 2 for z */
        switch (iAxis) {
        case 0:
            axisArray[0] = 1;

            /* X axis rotation (roll) */
            matrix[0] = 1;
            matrix[1] = 0;
            matrix[2] = 0;
            matrix[3] = 0;

            matrix[4] = 0;
            matrix[5] = cos(angleRad);
            matrix[6] = sin(angleRad);
            matrix[7] = 0;

            matrix[8] = 0;
            matrix[9] = -sin(angleRad);
            matrix[10] = cos(angleRad);
            matrix[11] = 0;

            matrix[12] = 0;
            matrix[13] = 0;
            matrix[14] = 0;
            matrix[15] = 1;
            break;

        case 1:
            axisArray[1] = 1;

            /* Y axis rotation (pitch) */
            matrix[0] = cos(angleRad);
            matrix[1] = 0;
            matrix[2] = -sin(angleRad);
            matrix[3] = 0;

            matrix[4] = 0;
            matrix[5] = 1;
            matrix[6] = 0;
            matrix[7] = 0;

            matrix[8] = sin(angleRad);
            matrix[9] = 0;
            matrix[10] = cos(angleRad);
            matrix[11] = 0;

            matrix[12] = 0;
            matrix[13] = 0;
            matrix[14] = 0;
            matrix[15] = 1;
            break;

        case 2:
            axisArray[2] = 1;

            /* Z axis rotation (yaw) */
            matrix[0] = cos(angleRad);
            matrix[1] = sin(angleRad);
            matrix[2] = 0;
            matrix[3] = 0;

            matrix[4] = -sin(angleRad);
            matrix[5] = cos(angleRad);
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
            break;

        default:
            throw geException("Bug: Invalid value fed to axis at geTransformRotate!", true);
            break;
        }

        angle = iAngle;
    }

    GLfloat* getTransformationMatrix() {
        return matrix;
    }

    void apply() {
        glRotatef(angle, axisArray[0], axisArray[1], axisArray[2]);
    }

};

class geTransformScale: public geTransform {
private:
    GLfloat scaleFactor[3];
public:
    geTransformScale(gePoint in, int tN) :
            geTransform(tN) {
        scaleFactor[0] = in.x;
        scaleFactor[1] = in.y;
        scaleFactor[2] = in.z;

        matrix[0] = scaleFactor[0];
        matrix[1] = 0;
        matrix[2] = 0;
        matrix[3] = 0;

        matrix[4] = 0;
        matrix[5] = scaleFactor[1];
        matrix[6] = 0;
        matrix[7] = 0;

        matrix[8] = 0;
        matrix[9] = 0;
        matrix[10] = scaleFactor[2];
        matrix[11] = 0;

        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        matrix[15] = 1;
    }

    geTransformScale(GLfloat x, GLfloat y, GLfloat z, int tN) :
            geTransform(tN) {
        scaleFactor[0] = x;
        scaleFactor[1] = y;
        scaleFactor[2] = z;

        matrix[0] = scaleFactor[0];
        matrix[1] = 0;
        matrix[2] = 0;
        matrix[3] = 0;

        matrix[4] = 0;
        matrix[5] = scaleFactor[1];
        matrix[6] = 0;
        matrix[7] = 0;

        matrix[8] = 0;
        matrix[9] = 0;
        matrix[10] = scaleFactor[2];
        matrix[11] = 0;

        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        matrix[15] = 1;
    }

    GLfloat* getTransformationMatrix() {
        return matrix;
    }

    void apply() {
        glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
    }

};
}

#endif /* GETRANSFORM_HPP_ */
