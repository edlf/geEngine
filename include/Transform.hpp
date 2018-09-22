/*
 * Eduardo Fernandes
 *
 * Transform class.
 */

#ifndef GETRANSFORM_HPP_
#define GETRANSFORM_HPP_

#include "includes.hpp"
#include <cmath>

namespace ge {

class Transform {
protected:
    int transformNumber;
    GLdouble matrix[16];

    void setIndentityMatrix();

public:
    Transform() = delete;
    Transform(Transform const&) = delete;
    Transform& operator=(Transform const&) = delete;

    Transform(int number);
    virtual ~Transform();

    virtual void updateMatrix() = 0;
    virtual void apply() = 0;
    GLdouble* getTransformationMatrix();
};

class TransformTranslate: public Transform {
private:
    xyzPointDouble destination;

public:
    TransformTranslate() = delete;
    TransformTranslate(TransformTranslate const&) = delete;
    TransformTranslate& operator=(TransformTranslate const&) = delete;

    TransformTranslate(xyzPointDouble in, int tN);
    TransformTranslate(GLdouble x, GLdouble y, GLdouble z, int tN);
    virtual ~TransformTranslate();

    virtual void updateMatrix();
    virtual void apply();
};

class TransformRotate: public Transform {
private:
    xyzPointDouble axisArray;
    GLdouble angle;
    GLdouble angleRad;
    int axis;

    unsigned int getAxisId();

public:
    TransformRotate() = delete;
    TransformRotate(TransformRotate const&) = delete;
    TransformRotate& operator=(TransformRotate const&) = delete;

    TransformRotate(int iAxis, GLdouble iAngle, int tN);
    virtual ~TransformRotate();

    virtual void updateMatrix();
    virtual void apply();
};

class TransformScale: public Transform {
private:
    xyzPointDouble scaleFactor;
public:
    TransformScale() = delete;
    TransformScale(TransformScale const&) = delete;
    TransformScale& operator=(TransformScale const&) = delete;

    TransformScale(xyzPointDouble in, int tN);
    TransformScale(GLdouble x, GLdouble y, GLdouble z, int tN);
    virtual ~TransformScale();

    virtual void updateMatrix();
    virtual void apply();
};
}

#endif /* TRANSFORM_HPP_ */
