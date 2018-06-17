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

class PrimitiveInterface {
    protected:
        GLdouble normal [3];
        void fghCircleTable (double **sint, double **cost, const int n);
        void calculateNormal (GLdouble v [3] [3], GLdouble out [3]);
        void NormalizeVector (GLdouble vector [3]);

    public:
        virtual void draw (GLdouble s, GLdouble t) = 0;
        virtual ~PrimitiveInterface ();
};

class Rectangle: public PrimitiveInterface {
    private:
        GLdouble x1, y1, x2, y2;
        GLdouble normal [3];
        //unsigned int _numDivisions, dx, dy;

    public:
        Rectangle (xyPointDouble pt1, xyPointDouble pt2);
        Rectangle (GLdouble inX0, GLdouble inY0, GLdouble inX1, GLdouble inY1);
        virtual ~Rectangle();

        void draw (GLdouble s, GLdouble t);
};

/* Triangle class. Assumes that you don't repeat points. Incomplete */
class Triangle: public PrimitiveInterface {
    private:
        GLdouble point1 [3], point2 [3], point3 [3];

    public:
        Triangle (GLdouble inX1, GLdouble inY1, GLdouble inZ1, GLdouble inX2, GLdouble inY2, GLdouble inZ2, GLdouble inX3, GLdouble inY3, GLdouble inZ3);
        Triangle (xyzPointDouble pt1, xyzPointDouble pt2, xyzPointDouble pt3);
        virtual ~Triangle();

        void draw (GLdouble s, GLdouble t);
};

/* Cylinder */
class Cylinder: public PrimitiveInterface {
    private:
        GLint slices, stacks;
        GLdouble baseRadius, topRadius, height;

        /* Internal precalc tables */
        GLdouble *sinCache;
        GLdouble *cosCache;
        GLdouble *sinCache2;
        GLdouble *cosCache2;
        GLdouble *sinCache3;
        GLdouble *cosCache3;

        /* Internal precalc vars */
        GLdouble angleStep;
        GLdouble zNormal;

        GLdouble deltaRadius, deltaAngle;

    public:
        Cylinder (GLdouble iBase, GLdouble iTop, GLdouble iHeight, unsigned int iSlices, unsigned int iStacks);
        virtual ~Cylinder ();

        virtual void draw (GLdouble s, GLdouble t);
        virtual void drawCircle (GLdouble radius);
};

class Sphere: public PrimitiveInterface {
    private:
        int slices, stacks;
        GLdouble radius;

        double *sint1, *cost1;
        double *sint2, *cost2;

    public:
        Sphere (GLdouble iRadius, int iSlices, int iStacks);
        virtual ~Sphere ();

        void draw (GLdouble s, GLdouble t);
};

class Torus: public PrimitiveInterface {
    private:
        int slices, loops;
        GLdouble inner, outer;

        /* Precalcs */
        double *vertex, *normal;

    public:
        Torus (GLdouble iInner, GLdouble iOuter, int iSlices, int iLoops);
        virtual ~Torus ();

        void draw (GLdouble s, GLdouble t);
};

/* Plane 1x1 */
/* More complex 4nd order initial grid.  16 points (4 by 4). */
class Plane: public PrimitiveInterface {
    protected:
        unsigned int partsPerAxis;
        static GLdouble planeGrid [4] [4] [3];
        GLdouble *grid;

        int evaluatorOrder;

    public:
        Plane (unsigned int parts);
        virtual ~Plane ();

        void draw (GLdouble s, GLdouble t);
};

/* Patch */
class Patch: public PrimitiveInterface {
    private:
        static GLdouble texturePoints [4] [2];

        unsigned int order, partsU, partsV, compute;
        unsigned int numberOfPoints;
        unsigned int readPoints;
        // compute= 0 point, 1 line, 2 fill
        // number of points = (order+1)^2

        /* Point grid */
        GLdouble *grid;

        std::vector<xyzPointDouble> points;

    public:
        Patch (unsigned int iorder, unsigned int ipartsU, unsigned int ipartsV, unsigned int icompute);

        unsigned int getNumberOfPoints ();

        void insertPoint (GLdouble ix, GLdouble iy, GLdouble iz);
        void insertPoint (xyzPointDouble i);

        void draw (GLdouble s, GLdouble t);

        virtual ~Patch ();
};

#define tiltFactor 1.5

class Vehicle: public PrimitiveInterface {
    private:
        Cylinder* topHub;
        Cylinder* topBody;
        Cylinder* bottomBody;

        Patch* evaluatorDemo;

        double position;
        double direction;
        double posX, posY, posZ;

        double tilt;

        unsigned int numberOfSlices;
        float angleStep;
        double walkStep;

        double deg2rad;

    public:
        Vehicle ();
        virtual ~Vehicle ();
        void draw (GLdouble s, GLdouble t);

        /* Vehicle control */
        void moveUp ();
        void moveDown ();

        void moveFront ();
        void moveBack ();

        void moveLeft ();
        void moveRight ();

        void tiltLeft ();
        void tiltRight ();
};

/* Water line */
class WaterLine: public PrimitiveInterface {
    protected:
        std::string heightmap, texturemap, fragmentshader, vertexshader;

        geTexture* heightmapTexture;
        geTexture* texture;
        geShader* waterShader;

        Plane* plane;

        GLint deltaTimeLoc, baseImageLoc, heightImageLoc;
        GLdouble dT;

    public:
        WaterLine (std::string& hmap, std::string& tmap, std::string& fshader, std::string& vshader);
        virtual ~WaterLine();

        void draw (GLdouble s, GLdouble t);
        void update (unsigned long timePassed);
};

} // namespace Primitives
}

#endif /* GEPRIMITIVE_HPP_ */
