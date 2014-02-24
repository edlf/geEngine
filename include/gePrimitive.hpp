/*
 * Eduardo Fernandes
 *
 * Primitive class.
 */

#ifndef GEPRIMITIVE_HPP_
#define GEPRIMITIVE_HPP_

#include "includes.hpp"
#include "geTexture.hpp"
#include "geShader.hpp"

class gePrimitive {
    protected:
        float normal [3];
        void fghCircleTable (double **sint, double **cost, const int n);
        void calculateNormal (float v [3] [3], float out [3]);
        void NormalizeVector (float vector [3]);

    public:
        virtual void draw (GLfloat s, GLfloat t);
        virtual ~gePrimitive ();
};

/* Rectangle class */
class geRectangle: public gePrimitive {
    private:
        GLfloat x1, y1, x2, y2;
        GLfloat normal [3];
        //unsigned int _numDivisions, dx, dy;

    public:
        geRectangle (ge2dPoint pt1, ge2dPoint pt2);
        geRectangle (GLfloat inX0, GLfloat inY0, GLfloat inX1, GLfloat inY1);

        void draw (GLfloat s, GLfloat t);
};

/* Triangle class. Assumes that you don't repeat points. Incomplete */
class geTriangle: public gePrimitive {
    private:
        GLfloat point1 [3], point2 [3], point3 [3];

    public:
        geTriangle (GLfloat inX1, GLfloat inY1, GLfloat inZ1, GLfloat inX2, GLfloat inY2, GLfloat inZ2, GLfloat inX3, GLfloat inY3, GLfloat inZ3);
        geTriangle (gePoint pt1, gePoint pt2, gePoint pt3);

        void draw (GLfloat s, GLfloat t);
};

/* Cylinder */
class geCylinder: public gePrimitive {
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
        geCylinder (GLfloat iBase, GLfloat iTop, GLfloat iHeight, unsigned int iSlices, unsigned int iStacks);

        void draw (GLfloat s, GLfloat t);

        void drawCircle (GLfloat radius);

        ~geCylinder ();
};

/* Sphere */
class geSphere: public gePrimitive {
    private:
        int slices, stacks;
        GLdouble radius;

        double *sint1, *cost1;
        double *sint2, *cost2;

    public:
        geSphere (GLfloat iRadius, int iSlices, int iStacks);

        void draw (GLfloat s, GLfloat t);

        ~geSphere ();
};

/* Torus */
class geTorus: public gePrimitive {
    private:
        int slices, loops;
        GLdouble inner, outer;

        /* Precalcs */
        double *vertex, *normal;

    public:
        geTorus (GLdouble iInner, GLdouble iOuter, int iSlices, int iLoops);

        void draw (GLfloat s, GLfloat t);

        ~geTorus ();
};

/* New primitives */
static GLfloat gePlaneGrid [4] [4] [3] = { { {-0.5, 0.0, -0.5}, {-0.125, 0.0, -0.5}, {0.125, 0.0, -0.5}, {0.5, 0.0, -0.5}}, { {-0.5, 0.0, -0.125}, {-0.125, 0.0,
        -0.125}, {0.125, 0.0, -0.125}, {0.5, 0.0, -0.125}}, { {-0.5, 0.0, 0.125}, {-0.125, 0.0, 0.125}, {0.125, 0.0, 0.125}, {0.5, 0.0, 0.125}}, { {-0.5, 0.0,
        0.5}, {-0.125, 0.0, 0.5}, {0.125, 0.0, 0.5}, {0.5, 0.0, 0.5}}};

/* Texture points for the evaluators */
static GLfloat texturePoints [4] [2] = { {0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};

/* Plane 1x1 */
/* More complex 4nd order initial grid.  16 points (4 by 4). */
class gePlane: public gePrimitive {
    protected:
        unsigned int partsPerAxis;

        GLfloat *grid;

        int evaluatorOrder;

    public:
        gePlane (unsigned int parts);

        void draw (GLfloat s, GLfloat t);

        virtual ~gePlane ();
};

/* Patch */
class gePatch: public gePrimitive {
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
        gePatch (unsigned int iorder, unsigned int ipartsU, unsigned int ipartsV, unsigned int icompute);

        unsigned int getNumberOfPoints ();

        void insertPoint (GLfloat ix, GLfloat iy, GLfloat iz);
        void insertPoint (gePoint i);

        void draw (GLfloat s, GLfloat t);

        virtual ~gePatch ();
};

#define tiltFactor 1.5

/* Flying vehicle */
class geVehicle: public gePrimitive {
    private:
        geCylinder* topHub;
        geCylinder* topBody;
        geCylinder* bottomBody;

        gePatch* evaluatorDemo;

        float position;
        float direction;
        float posX, posY, posZ;

        float tilt;

        unsigned int numberOfSlices;
        float angleStep;
        float walkStep;

        float deg2rad;

    public:
        geVehicle ();
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

        ~geVehicle ();
};

/* Water line */
class geWaterLine: public gePrimitive {
    protected:
        string heightmap, texturemap, fragmentshader, vertexshader;

        geTexture* heightmapTexture;
        geTexture* texture;
        geShader* waterShader;

        gePlane* plane;

        GLint deltaTimeLoc, baseImageLoc, heightImageLoc;
        GLfloat dT;

    public:
        geWaterLine (string hmap, string tmap, string fshader, string vshader);
        void draw (GLfloat s, GLfloat t);
        void update (unsigned long timePassed);
};

#endif /* GEPRIMITIVE_HPP_ */
