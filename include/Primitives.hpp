/*
 * Eduardo Fernandes
 *
 * Primitives
 */

#ifndef GEPRIMITIVE_HPP_
#define GEPRIMITIVE_HPP_

#include "includes.hpp"
#include "geTexture.hpp"
#include "geShader.hpp"

namespace ge {
namespace Primitives {

class Base {
    protected:
        float normal [3];
        void fghCircleTable (double **sint, double **cost, const int n);
        void calculateNormal (float v [3] [3], float out [3]);
        void NormalizeVector (float vector [3]);

    public:
        virtual void draw (GLfloat s, GLfloat t) = 0;
        virtual ~Base ();
};

class Rectangle: public Base {
    private:
        GLfloat x1, y1, x2, y2;
        GLfloat normal [3];
        //unsigned int _numDivisions, dx, dy;

    public:
        Rectangle (ge2dPoint pt1, ge2dPoint pt2);
        Rectangle (GLfloat inX0, GLfloat inY0, GLfloat inX1, GLfloat inY1);

        void draw (GLfloat s, GLfloat t);
};

/* Triangle class. Assumes that you don't repeat points. Incomplete */
class Triangle: public Base {
    private:
        GLfloat point1 [3], point2 [3], point3 [3];

    public:
        Triangle (GLfloat inX1, GLfloat inY1, GLfloat inZ1, GLfloat inX2, GLfloat inY2, GLfloat inZ2, GLfloat inX3, GLfloat inY3, GLfloat inZ3);
        Triangle (gePoint pt1, gePoint pt2, gePoint pt3);

        void draw (GLfloat s, GLfloat t);
};

/* Cylinder */
class Cylinder: public Base {
    private:
        GLint slices, stacks;
        GLfloat baseRadius, topRadius, height;

        /* Internal precalc tables */
        GLfloat *sinCache;
        GLfloat *cosCache;
        GLfloat *sinCache2;
        GLfloat *cosCache2;
        GLfloat *sinCache3;
        GLfloat *cosCache3;

        /* Internal precalc vars */
        GLfloat angleStep;
        GLfloat zNormal;

        GLfloat deltaRadius, deltaAngle;

    public:
        Cylinder (GLfloat iBase, GLfloat iTop, GLfloat iHeight, unsigned int iSlices, unsigned int iStacks);
        ~Cylinder ();

        virtual void draw (GLfloat s, GLfloat t);
        virtual void drawCircle (GLfloat radius);
};

class Sphere: public Base {
    private:
        int slices, stacks;
        GLdouble radius;

        double *sint1, *cost1;
        double *sint2, *cost2;

    public:
        Sphere (GLfloat iRadius, int iSlices, int iStacks);
        ~Sphere ();

        void draw (GLfloat s, GLfloat t);
};

class Torus: public Base {
    private:
        int slices, loops;
        GLdouble inner, outer;

        /* Precalcs */
        double *vertex, *normal;

    public:
        Torus (GLdouble iInner, GLdouble iOuter, int iSlices, int iLoops);
        ~Torus ();

        void draw (GLfloat s, GLfloat t);
};

/* New primitives */
static GLfloat gePlaneGrid [4] [4] [3] = { { {-0.5, 0.0, -0.5}, {-0.125, 0.0, -0.5}, {0.125, 0.0, -0.5}, {0.5, 0.0, -0.5}}, { {-0.5, 0.0, -0.125}, {-0.125, 0.0,
        -0.125}, {0.125, 0.0, -0.125}, {0.5, 0.0, -0.125}}, { {-0.5, 0.0, 0.125}, {-0.125, 0.0, 0.125}, {0.125, 0.0, 0.125}, {0.5, 0.0, 0.125}}, { {-0.5, 0.0,
        0.5}, {-0.125, 0.0, 0.5}, {0.125, 0.0, 0.5}, {0.5, 0.0, 0.5}}};

/* Texture points for the evaluators */
static GLfloat texturePoints [4] [2] = { {0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};

/* Plane 1x1 */
/* More complex 4nd order initial grid.  16 points (4 by 4). */
class Plane: public Base {
    protected:
        unsigned int partsPerAxis;

        GLfloat *grid;

        int evaluatorOrder;

    public:
        Plane (unsigned int parts);

        void draw (GLfloat s, GLfloat t);

        virtual ~Plane ();
};

/* Patch */
class Patch: public Base {
    private:
        unsigned int order, partsU, partsV, compute;
        unsigned int numberOfPoints;
        unsigned int readPoints;
        // compute= 0 point, 1 line, 2 fill
        // number of points = (order+1)^2

        /* Point grid */
        GLfloat *grid;

        vector<gePoint> points;

    public:
        Patch (unsigned int iorder, unsigned int ipartsU, unsigned int ipartsV, unsigned int icompute);

        unsigned int getNumberOfPoints ();

        void insertPoint (GLfloat ix, GLfloat iy, GLfloat iz);
        void insertPoint (gePoint i);

        void draw (GLfloat s, GLfloat t);

        virtual ~Patch ();
};

#define tiltFactor 1.5

class Vehicle: public Base {
    private:
        Cylinder* topHub;
        Cylinder* topBody;
        Cylinder* bottomBody;

        Patch* evaluatorDemo;

        float position;
        float direction;
        float posX, posY, posZ;

        float tilt;

        unsigned int numberOfSlices;
        float angleStep;
        float walkStep;

        float deg2rad;

    public:
        Vehicle ();
        void draw (GLfloat s, GLfloat t);

        /* Vehicle control */
        void moveUp ();
        void moveDown ();

        void moveFront ();
        void moveBack ();

        void moveLeft ();
        void moveRight ();

        void tiltLeft ();
        void tiltRight ();

        ~Vehicle ();
};

/* Water line */
class WaterLine: public Base {
    protected:
        string heightmap, texturemap, fragmentshader, vertexshader;

        geTexture* heightmapTexture;
        geTexture* texture;
        geShader* waterShader;

        Plane* plane;

        GLint deltaTimeLoc, baseImageLoc, heightImageLoc;
        GLfloat dT;

    public:
        WaterLine (string hmap, string tmap, string fshader, string vshader);
        void draw (GLfloat s, GLfloat t);
        void update (unsigned long timePassed);
};

} // namespace Primitives
}

#endif /* GEPRIMITIVE_HPP_ */
