/*
 * Eduardo Fernandes
 *
 * Primitive class methods.
 */

#include <Primitives.hpp>

namespace ge {
namespace Primitives {

/* Copy from GLUT
 * Compute lookup table of cos and sin values forming a circle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

/* Primitive super class */
void PrimitiveInterface::fghCircleTable(double **sint, double **cost,
        const int n) {
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2 * M_PI / (double) ((n == 0) ? 1 : n);

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size + 1);
    *cost = (double *) calloc(sizeof(double), size + 1);

    /* Bail out if memory allocation fails */

    if (!(*sint) || !(*cost)) {
        free(*sint);
        free(*cost);
        throw geException("Failed to allocate memory in fghCircleTable.", true);
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i = 1; i < size; i++) {
        (*sint)[i] = sin(angle * i);
        (*cost)[i] = cos(angle * i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}

void PrimitiveInterface::calculateNormal(GLdouble v[3][3], GLdouble out[3]) // Calculates Normal For A Quad Using 3 Points
        {
    GLdouble v1[3], v2[3];                // Vector 1 (x,y,z) & Vector 2 (x,y,z)
    static const int x = 0;                     // Define X Coord
    static const int y = 1;                     // Define Y Coord
    static const int z = 2;                     // Define Z Coord

    // Finds The Vector Between 2 Points By Subtracting
    // The x,y,z Coordinates From One Point To Another.

    // Calculate The Vector From Point 1 To Point 0
    v1[x] = v[0][x] - v[1][x];             // Vector 1.x=Vertex[0].x-Vertex[1].x
    v1[y] = v[0][y] - v[1][y];             // Vector 1.y=Vertex[0].y-Vertex[1].y
    v1[z] = v[0][z] - v[1][z];             // Vector 1.z=Vertex[0].y-Vertex[1].z
    // Calculate The Vector From Point 2 To Point 1
    v2[x] = v[1][x] - v[2][x];             // Vector 2.x=Vertex[0].x-Vertex[1].x
    v2[y] = v[1][y] - v[2][y];             // Vector 2.y=Vertex[0].y-Vertex[1].y
    v2[z] = v[1][z] - v[2][z];             // Vector 2.z=Vertex[0].z-Vertex[1].z
    // Compute The Cross Product To Give Us A Surface Normal
    out[x] = v1[y] * v2[z] - v1[z] * v2[y];           // Cross Product For Y - Z
    out[y] = v1[z] * v2[x] - v1[x] * v2[z];           // Cross Product For X - Z
    out[z] = v1[x] * v2[y] - v1[y] * v2[x];           // Cross Product For X - Y

    NormalizeVector(out);
}

void PrimitiveInterface::NormalizeVector(GLdouble vector[3]) {
    GLdouble length;

    /* Vector Length */
    length = (GLdouble) sqrt(
            (vector[0] * vector[0]) + (vector[1] * vector[1])
                    + (vector[2] * vector[2]));

    if (length == 0.0f) {
        length = 1.0f;
    }

    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
}

PrimitiveInterface::~PrimitiveInterface() {
}

/* Rectangle primitive */
Rectangle::Rectangle(xyPointDouble pt1, xyPointDouble pt2) {
    /* Check if rectangle is valid */
    if (pt1.x == pt2.x || pt1.y == pt2.y) {
        throw geException(
                "Invalid rectangle defined: the sides must not be equal!",
                true);
    }

    x1 = pt1.x;
    y1 = pt1.y;
    x2 = pt2.x;
    y2 = pt2.y;

    /* end */
}

Rectangle::Rectangle(GLdouble inX0, GLdouble inY0, GLdouble inX1,
        GLdouble inY1) {
    /* Check if rectangle is valid */
    if (inX0 == inX1 || inY0 == inY1) {
        throw geException(
                "Invalid rectangle defined: the sides must not be equal!",
                true);
    }

    x1 = inX0;
    y1 = inY0;
    x2 = inX1;
    y2 = inY1;

    /* end */
}

Rectangle::~Rectangle() {

}

void Rectangle::draw(GLdouble s, GLdouble t) {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBegin(GL_POLYGON);
    if ((this->y2 < this->y1) && (this->x2 < this->x1)) {
        glNormal3d(0.0, 0.0, -1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3d(this->x2, this->y2, 0.0);
        glTexCoord2d((this->x1 - this->x2) / s, 0.0);
        glVertex3d(this->x1, this->y2, 0.0);
        glTexCoord2d((this->x2 - this->x1) / s, (this->y2 - this->y1) / t);
        glVertex3d(this->x1, this->y1, 0.0);
        glTexCoord2d(0.0, (this->y2 - this->y1) / t);
        glVertex3d(this->x2, this->y1, 0.0);
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(this->x1, this->y1, 0.0);
        glTexCoord2d((this->x2 - this->x1) / s, 0.0);
        glVertex3d(this->x2, this->y1, 0.0);
        glTexCoord2d((this->x2 - this->x1) / s, (this->y2 - this->y1) / t);
        glVertex3d(this->x2, this->y2, 0.0);
        glTexCoord2d(0.0, (this->y2 - this->y1) / t);
        glVertex3d(this->x1, this->y2, 0.0);
    }

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/* Triangle primitive */
Triangle::Triangle(GLdouble inX1, GLdouble inY1, GLdouble inZ1, GLdouble inX2,
        GLdouble inY2, GLdouble inZ2, GLdouble inX3, GLdouble inY3,
        GLdouble inZ3) {
    this->point1[0] = inX1;
    this->point1[1] = inY1;
    this->point1[2] = inZ1;

    this->point2[0] = inX2;
    this->point2[1] = inY2;
    this->point2[2] = inZ2;

    this->point3[0] = inX3;
    this->point3[1] = inY3;
    this->point3[2] = inZ3;
}

Triangle::Triangle(xyzPointDouble pt1, xyzPointDouble pt2, xyzPointDouble pt3) {
    this->point1[0] = pt1.x;
    this->point1[1] = pt1.y;
    this->point1[2] = pt1.z;

    this->point2[0] = pt2.x;
    this->point2[1] = pt2.y;
    this->point2[2] = pt2.z;

    this->point3[0] = pt3.x;
    this->point3[1] = pt3.y;
    this->point3[2] = pt3.z;

    GLdouble pontos[3][3] = { { this->point1[0], this->point1[1],
            this->point1[2] }, { this->point2[0], this->point2[1],
            this->point2[2] }, { this->point3[0], this->point3[1],
            this->point3[2] } };
    calculateNormal(pontos, normal);
}

Triangle::~Triangle() {

}

void Triangle::draw(GLdouble s, GLdouble t) {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glNormal3d(normal[0], normal[1], normal[2]);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(this->point1[0], this->point1[1], this->point1[2]);

    glTexCoord2d(
            abs(
                    (this->point2[0] - this->point1[0]) / s * normal[2]
                            + (this->point2[0] - this->point1[0]) / s
                                    * normal[1]
                            + (this->point2[2] - this->point1[2]) / s
                                    * normal[0]), 0.0);
    glVertex3d(this->point2[0], this->point2[1], this->point2[2]);

    glTexCoord2d(
            abs(
                    (this->point3[0] - this->point1[0]) / s * normal[2]
                            + (this->point3[0] - this->point1[0]) / s
                                    * normal[1]
                            + (this->point3[2] - this->point1[2]) / s
                                    * normal[0]),
            abs(
                    (this->point3[1] - this->point1[1]) / t * normal[2]
                            + (this->point3[0] - this->point1[0]) / t
                                    * normal[1]
                            + (this->point3[1] - this->point1[1]) / t
                                    * normal[0]));
    glVertex3d(this->point3[0], this->point3[1], this->point3[2]);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/* Cylinder primitive */
Cylinder::Cylinder(GLdouble iBase, GLdouble iTop, GLdouble iHeight,
        unsigned int iSlices, unsigned int iStacks) {
    this->baseRadius = iBase;
    this->topRadius = iTop;
    this->height = iHeight;
    this->slices = iSlices;
    this->stacks = iStacks;

    this->angleStep = 2 * M_PI / slices;

    /* Precalc */

    /* Allocs */
    sinCache = (double *) calloc(sizeof(double), slices + 1);
    cosCache = (double *) calloc(sizeof(double), slices + 1);
    sinCache2 = (double *) calloc(sizeof(double), slices + 1);
    cosCache2 = (double *) calloc(sizeof(double), slices + 1);
    sinCache3 = (double *) calloc(sizeof(double), slices + 1);
    cosCache3 = (double *) calloc(sizeof(double), slices + 1);

    GLdouble angle;
    GLdouble length;

    GLdouble xyNormalRatio;

    if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0
            || height < 0.0) {
        throw geException("Invalid values fed to geCylinder.", true);
    }

    /* Compute length (needed for normal calculations) */
    deltaRadius = baseRadius - topRadius;
    length = sqrt(deltaRadius * deltaRadius + height * height);

    if (length == 0.0) {
        throw geException("Invalid values fed to geCylinder", true);
    }

    zNormal = deltaRadius / length;
    xyNormalRatio = height / length;

    for (int i = 0; i < slices; i++) {
        angle = 2 * M_PI * i / slices;

        sinCache[i] = sin(angle);
        cosCache[i] = cos(angle);

        sinCache2[i] = xyNormalRatio * sin(angle);
        cosCache2[i] = xyNormalRatio * cos(angle);

        sinCache3[i] = xyNormalRatio * sin(angle);
        cosCache3[i] = xyNormalRatio * cos(angle);
    }

    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    sinCache2[slices] = sinCache2[0];
    cosCache2[slices] = cosCache2[0];
    sinCache3[slices] = sinCache3[0];
    cosCache3[slices] = cosCache3[0];

    this->deltaAngle = 2.0 * M_PI / (this->slices);
}

void Cylinder::draw(GLdouble, GLdouble) {
    glPushMatrix();

    GLdouble radiusLow, radiusHigh;
    GLdouble zLow, zHigh;

    /* Cover the base and top */
    if (baseRadius > 0.0) {
        drawCircle(this->baseRadius);
    }

    /* Top needs a translation */
    if (topRadius > 0.0) {
        glPushMatrix();
        glTranslated(0, 0, this->height);
        drawCircle(this->topRadius);
        glPopMatrix();
    }

    for (int j = 0; j < stacks; j++) {
        zLow = j * height / stacks;
        zHigh = (j + 1) * height / stacks;
        radiusLow = baseRadius - deltaRadius * ((float) j / (float) stacks);
        radiusHigh = baseRadius
                - deltaRadius * ((float) (j + 1) / (float) stacks);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            glNormal3d(sinCache2[i], cosCache2[i], zNormal);

            glTexCoord2d(1 - (GLdouble) i / slices,
                    (GLdouble) j / (GLdouble) stacks);
            glVertex3d(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
            glTexCoord2d(1 - (GLdouble) i / slices,
                    (GLdouble) (j + 1) / (GLdouble) stacks);
            glVertex3d(radiusHigh * sinCache[i], radiusHigh * cosCache[i],
                    zHigh);
        }
    }
    glEnd();

    glPopMatrix();

}

/* Needs optimizations */
void Cylinder::drawCircle(GLdouble radius) {
    GLdouble vertex[4];
    GLdouble texcoord[2];

    glBegin(GL_TRIANGLE_FAN);

    /* draw the vertex at the center of the circle */
    texcoord[0] = 0.5;
    texcoord[1] = 0.5;
    glTexCoord2dv(texcoord);

    vertex[0] = vertex[1] = vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4dv(vertex);

    for (int i = 0; i < this->slices; i++) {
        texcoord[0] = (cosCache[i] + 1.0) * 0.5;
        texcoord[1] = (sinCache[i] + 1.0) * 0.5;
        glTexCoord2dv(texcoord);

        vertex[0] = cosCache[i] * radius;
        vertex[1] = sinCache[i] * radius;
        vertex[2] = 0.0;
        vertex[3] = 1.0;
        glVertex4dv(vertex);
    }

    texcoord[0] = (1.0 + 1.0) * 0.5;
    texcoord[1] = (0.0 + 1.0) * 0.5;
    glTexCoord2dv(texcoord);

    vertex[0] = 1.0 * radius;
    vertex[1] = 0.0 * radius;
    vertex[2] = 0.0;
    vertex[3] = 1.0;
    glVertex4dv(vertex);
    glEnd();
}

Cylinder::~Cylinder() {
    free(sinCache);
    free(cosCache);
    free(sinCache2);
    free(cosCache2);
    free(sinCache3);
    free(cosCache3);
}

/* Sphere primitive */
Sphere::Sphere(GLdouble iRadius, int iSlices, int iStacks) {
    this->radius = iRadius;
    this->slices = iSlices;
    this->stacks = iStacks;

    fghCircleTable(&sint1, &cost1, -this->slices);
    fghCircleTable(&sint2, &cost2, this->stacks * 2);
}

void Sphere::draw(GLdouble, GLdouble) {
    glPushMatrix();

    int i, j;

    /* Adjust z and radius as stacks are drawn. */

    double z0, z1;
    double r0, r1;

    z0 = 1.0;
    z1 = cost2[(stacks > 0) ? 1 : 0];
    r0 = 0.0;
    r1 = sint2[(stacks > 0) ? 1 : 0];

    glBegin(GL_TRIANGLE_FAN);

    glNormal3d(0, 0, 1);
    glVertex3d(0, 0, radius);

    for (j = slices; j >= 0; j--) {
        glNormal3d(cost1[j] * r1, sint1[j] * r1, z1);
        glTexCoord3d(cost1[j] * r1 * radius, sint1[j] * r1 * radius,
                z1 * radius);
        glVertex3d(cost1[j] * r1 * radius, sint1[j] * r1 * radius, z1 * radius);
    }

    glEnd();

    /* Cover each stack with a quad strip, except the top and bottom stacks */

    for (i = 1; i < stacks - 1; i++) {
        z0 = z1;
        z1 = cost2[i + 1];
        r0 = r1;
        r1 = sint2[i + 1];

        glBegin(GL_QUAD_STRIP);

        for (j = 0; j <= slices; j++) {

            glNormal3d(cost1[j] * r1, sint1[j] * r1, z1);
            glTexCoord3d(cost1[j] * r1 * radius, sint1[j] * r1 * radius,
                    z1 * radius);

            glVertex3d(cost1[j] * r1 * radius, sint1[j] * r1 * radius,
                    z1 * radius);
            glNormal3d(cost1[j] * r0, sint1[j] * r0, z0);

            glTexCoord3d(cost1[j] * r0 * radius, sint1[j] * r0 * radius,
                    z0 * radius);
            glVertex3d(cost1[j] * r0 * radius, sint1[j] * r0 * radius,
                    z0 * radius);
        }

        glEnd();
    }

    /* The bottom stack is covered with a triangle fan */

    z0 = z1;
    r0 = r1;

    glBegin(GL_TRIANGLE_FAN);

    glNormal3d(0, 0, -1);
    glVertex3d(0, 0, -radius);

    for (j = 0; j <= slices; j++) {
        glNormal3d(cost1[j] * r0, sint1[j] * r0, z0);
        glTexCoord3d(cost1[j] * r0 * radius, sint1[j] * r0 * radius,
                z0 * radius);
        glVertex3d(cost1[j] * r0 * radius, sint1[j] * r0 * radius, z0 * radius);
    }

    glEnd();

    glPopMatrix();
}

Sphere::~Sphere() {
    /* Release sin and cos tables */
    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}

/* Torus primitive */
Torus::Torus(GLdouble iInner, GLdouble iOuter, int iSlices, int iLoops) {
    this->inner = iInner;
    this->outer = iOuter;
    this->slices = iSlices;
    this->loops = iLoops;

    /* Checks */
    if (this->slices < 1) {
        this->slices = 1;
    }

    if (this->loops < 1) {
        this->loops = 1;
    }

    /* Increment the number of sides and rings to allow for one more point than surface */
    slices++;
    loops++;

    /* Allocate the vertices array */
    vertex = (double *) calloc(sizeof(double), 3 * slices * loops);
    normal = (double *) calloc(sizeof(double), 3 * slices * loops);

    /* Precalc */
    double dpsi, dphi;
    double spsi, cpsi, sphi, cphi;
    double phi, psi;

    dpsi = 2.0 * M_PI / (double) (loops - 1);
    dphi = -2.0 * M_PI / (double) (slices - 1);

    psi = 0.0;

    for (int j = 0; j < loops; j++) {
        cpsi = cos(psi);
        spsi = sin(psi);
        phi = 0.0;

        for (int i = 0; i < slices; i++) {
            int offset = 3 * (j * slices + i);
            cphi = cos(phi);
            sphi = sin(phi);
            *(vertex + offset + 0) = cpsi * (outer + cphi * inner);
            *(vertex + offset + 1) = spsi * (outer + cphi * inner);
            *(vertex + offset + 2) = sphi * inner;
            *(normal + offset + 0) = cpsi * cphi;
            *(normal + offset + 1) = spsi * cphi;
            *(normal + offset + 2) = sphi;
            phi += dphi;
        }

        psi += dpsi;
    }
    /* End precalcs */
}

Torus::~Torus() {
    free(vertex);
    free(normal);
}

void Torus::draw(GLdouble, GLdouble) {
    glPushMatrix();

    glBegin(GL_QUADS);
    for (int i = 0; i < slices - 1; i++) {
        for (int j = 0; j < loops - 1; j++) {
            int offset = 3 * (j * slices + i);
            glNormal3dv(normal + offset);
            glVertex3dv(vertex + offset);
            glNormal3dv(normal + offset + 3);
            glVertex3dv(vertex + offset + 3);
            glNormal3dv(normal + offset + 3 * slices + 3);
            glVertex3dv(vertex + offset + 3 * slices + 3);
            glNormal3dv(normal + offset + 3 * slices);
            glVertex3dv(vertex + offset + 3 * slices);
        }
    }
    glEnd();

    glPopMatrix();
}

/* Plane primitive */
GLdouble Plane::planeGrid[4][4][3] = { { { -0.5, 0.0, -0.5 }, { -0.125, 0.0,
        -0.5 }, { 0.125, 0.0, -0.5 }, { 0.5, 0.0, -0.5 } }, { { -0.5, 0.0,
        -0.125 }, { -0.125, 0.0, -0.125 }, { 0.125, 0.0, -0.125 }, { 0.5, 0.0,
        -0.125 } }, { { -0.5, 0.0, 0.125 }, { -0.125, 0.0, 0.125 }, { 0.125,
        0.0, 0.125 }, { 0.5, 0.0, 0.125 } }, { { -0.5, 0.0, 0.5 }, { -0.125,
        0.0, 0.5 }, { 0.125, 0.0, 0.5 }, { 0.5, 0.0, 0.5 } } };

Plane::Plane(unsigned int parts) {
    this->partsPerAxis = parts;
    this->evaluatorOrder = 4;
    this->grid = &planeGrid[0][0][0];
}

Plane::~Plane() {

}

void Plane::draw(GLdouble, GLdouble) {
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(this->partsPerAxis, 0.0, 1.0, this->partsPerAxis, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glMap2d(GL_MAP2_VERTEX_3, 0, 1, 3, evaluatorOrder, 0, 1, evaluatorOrder * 3,
            evaluatorOrder, grid);
    glEvalMesh2(GL_FILL, 0, this->partsPerAxis, 0, this->partsPerAxis);
}

/* Patch primitive */
/* Texture points for the evaluators */
GLdouble Patch::texturePoints[4][2] = { { 0.0, 0.0 }, { 0.0, 1.0 },
        { 1.0, 0.0 }, { 1.0, 1.0 } };

Patch::Patch(unsigned int iorder, unsigned int ipartsU, unsigned int ipartsV,
        unsigned int icompute) {
    this->order = iorder + 1;
    this->partsU = ipartsU;
    this->partsV = ipartsV;
    this->compute = icompute;
    this->readPoints = 0;

    /* Check this */
    this->numberOfPoints = (int) pow((float) (order), (float) 2);

    /* Set grid pointer to null */
    grid = nullptr;

}

unsigned int Patch::getNumberOfPoints() {
    return this->numberOfPoints;
}

void Patch::insertPoint(GLdouble ix, GLdouble iy, GLdouble iz) {
    if (readPoints < numberOfPoints) {

        xyzPointDouble temporaryPoint;

        temporaryPoint.x = ix;
        temporaryPoint.y = iy;
        temporaryPoint.z = iz;

        points.push_back(temporaryPoint);

        readPoints++;

        /* If we just filled the vector */
        if (readPoints == numberOfPoints) {
            grid = &points[0].x;
        }

    } else {
        throw geException("gePatch: Too many points!", true);
    }
}

Patch::~Patch() {
    points.clear();
}

void Patch::insertPoint(xyzPointDouble i) {
    if (readPoints < numberOfPoints) {
        points.push_back(i);

        readPoints++;

        /* If we just filled the vector */
        if (readPoints == numberOfPoints) {
            grid = &points[0].x;
        }

    } else {
        throw geException("gePatch: Too many points!", true);
    }
}

void Patch::draw(GLdouble, GLdouble) {
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_MAP2_TEXTURE_COORD_2);

    glMapGrid2f(this->partsU, 0.0, 1.0, this->partsV, 0.0, 1.0);

    //glMap2f (GL_MAP2_VERTEX_3, 0, 1, 3, this->partsU, 0, 1, this->partsU * 3, this->partsV, grid);
    glMap2d(GL_MAP2_VERTEX_3, 0, 1, 3, this->order, 0, 1, this->order * 3,
            this->order, grid);

    glMap2d(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
            &texturePoints[0][0]);

    switch (this->compute) {
        case '0':
            glEvalMesh2(GL_POINT, 0, this->partsU, 0, this->partsV);
            break;

        case '1':
            glEvalMesh2(GL_LINE, 0, this->partsU, 0, this->partsV);
            break;

        case '2':
            glEvalMesh2(GL_FILL, 0, this->partsU, 0, this->partsV);
            break;

        default:
            glEvalMesh2(GL_FILL, 0, this->partsU, 0, this->partsV);
            break;
    }

    glEnable(GL_AUTO_NORMAL);
}

/* Vehicle primitive */
Vehicle::Vehicle() {
    numberOfSlices = 20;

    topHub = new Cylinder(0.5, 0.5, 0.3, numberOfSlices, 4);

    topBody = new Cylinder(2, 0.5, 0.7, numberOfSlices, 8);
    bottomBody = new Cylinder(1, 2, 0.3, numberOfSlices, 4);

    direction = 0.0;
    position = 0.0;
    tilt = 0.0;

    angleStep = 1.5;
    walkStep = 0.2;

    posX = 0.0;
    posY = 0.0;
    posZ = 0.0;

    deg2rad = M_PI / 180.0;

    evaluatorDemo = new Patch(2, 20, 20, 2);
    /* Feed points */
    evaluatorDemo->insertPoint(0, 0, 0);
    evaluatorDemo->insertPoint(2, 0, 0);
    evaluatorDemo->insertPoint(4, 0, 0);

    evaluatorDemo->insertPoint(0, 0.3, 0.5);
    evaluatorDemo->insertPoint(1.5, 0.5, 0.5);
    evaluatorDemo->insertPoint(3, 0.7, 0.5);

    evaluatorDemo->insertPoint(0, 0, 1);
    evaluatorDemo->insertPoint(1.5, 0, 1);
    evaluatorDemo->insertPoint(2, 0, 1);
}

void Vehicle::draw(GLdouble s, GLdouble t) {
    glPushMatrix();

    /* Position the vehicle */
    glTranslated(posX, posY, posZ);
    glRotated(direction, 0, 1, 0);

    /* Rotate the ufo */
    glRotated(-90, 1, 0, 0);

    glRotated(tilt, 0, 1, 0);

    glRotated(90, 0, 0, 1);
    evaluatorDemo->draw(s, t);

    bottomBody->draw(0, 0);

    glTranslated(0, 0, 0.3);

    topBody->draw(0, 0);

    glTranslated(0, 0, 0.7);

    topHub->draw(0, 0);

    glPopMatrix();
}

void Vehicle::moveUp() {
    posY = posY + walkStep;
}

void Vehicle::moveDown() {
    posY = posY - walkStep;
}

void Vehicle::moveFront() {
    position = position + walkStep;
    posX = posX + sin(direction * deg2rad);
    posZ = posZ + cos(direction * deg2rad);
}

void Vehicle::moveBack() {
    position = position - walkStep;
    posX = posX - sin(direction * deg2rad);
    posZ = posZ - cos(direction * deg2rad);
}

void Vehicle::moveLeft() {
    direction = direction + angleStep;
}

void Vehicle::moveRight() {
    direction = direction - angleStep;
}

void Vehicle::tiltLeft() {
    tilt = tilt - tiltFactor;
}

void Vehicle::tiltRight() {
    tilt = tilt + tiltFactor;
}

Vehicle::~Vehicle() {
    delete (topHub);
    delete (topBody);
    delete (bottomBody);
}

/* Water Line primitive */
WaterLine::WaterLine(std::string& hmap, std::string& tmap, std::string& fshader,
        std::string& vshader) {

    /* Plane */
    this->plane = new Plane(100);

    /* Append the shader folder */
    this->heightmap = hmap;
    this->texturemap = tmap;
    this->fragmentshader = std::string(SHADER_FOLDER) + fshader;
    this->vertexshader = std::string(SHADER_FOLDER) + vshader;

    /* Do not create shader object before openGL is initialized */
    this->waterShader = nullptr;
    std::string heightMapTextureName = "INTERNAL_heightMap";
    this->heightmapTexture = new geTexture(heightMapTextureName,
            this->heightmap);
    std::string textureName = "INTERNAL_textureMap";
    this->texture = new geTexture(textureName, this->texturemap);

    this->dT = 0.0f;
    this->deltaTimeLoc = 0;
    this->baseImageLoc = 0;
    this->heightImageLoc = 0;
}

WaterLine::~WaterLine() {

}

void WaterLine::draw(GLdouble s, GLdouble t) {
    if (waterShader == nullptr) {
        /* Create the actual shader object */
        this->waterShader = new geShader(vertexshader.c_str(),
                fragmentshader.c_str());
        this->heightmapTexture->loadTexture();
        this->texture->loadTexture();

    } else {

        /* Bind it */
        this->waterShader->bind();

        /*Sending variables to shader */
        deltaTimeLoc = glGetUniformLocation(waterShader->getId(), "dT");
        glUniform1f(deltaTimeLoc, dT);

        baseImageLoc = glGetUniformLocation(waterShader->getId(),
                "baseTexture");
        glUniform1i(baseImageLoc, 0);

        heightImageLoc = glGetUniformLocation(waterShader->getId(),
                "heightTexture");
        glUniform1i(heightImageLoc, 1);

        /* Activating textures */
        glActiveTexture(GL_TEXTURE1);
        heightmapTexture->apply();

        glActiveTexture(GL_TEXTURE0);
        texture->apply();

        /* Draw the plane */
        this->plane->draw(s, t);

        /* Unbind the shader */
        waterShader->unbind();
    }
}

void WaterLine::update(unsigned long/* timePassed */) {
    if (waterShader != nullptr) {

        //waterShader->update(timePassed);

        dT = dT + 0.003f;
        if (dT >= 1.0f) {
            dT = 0.0f;
        }
    }
}

} // namespace Primitives
}
