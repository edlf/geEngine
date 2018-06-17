/*
 * Eduardo Fernandes
 *
 * Type definitions
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

namespace ge {

struct xyzPointDouble {
    GLdouble x, y, z;
};

struct xyzPointFloat {
    GLfloat x, y, z;
};

struct xyPointDouble {
	GLdouble x, y;
};

struct xyPointFloat {
	GLfloat x, y;
};

struct color {
    float r, g, b, a;
};

}

#endif /* TYPES_HPP_ */
