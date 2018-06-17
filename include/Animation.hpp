/*
 * Eduardo Fernandes
 *
 * Animation class.
 */

#ifndef GEANIMATION_HPP_
#define GEANIMATION_HPP_

#include "includes.hpp"

namespace ge {

class geAnimation {
protected:
    std::string id;
    float span;
    unsigned int type;

    std::vector<xyzPointDouble> controlPoints;
    unsigned int numberOfControlPoints;

    unsigned long deltaTimeMillis;
    unsigned long updateCounter;

    unsigned long deltaTimeSingleMove;

    unsigned long spanMilliseconds;
    unsigned long totalTimePassed;

    GLdouble x, y, z;

    GLdouble angleX;
    GLdouble angleZ;

    void updateAngles();

    bool hasEnded;

    xyzPointDouble currentSourcePoint;
    xyzPointDouble currentDestionationPoint;
    unsigned int numberOfProcessedControlPoints;

    GLdouble dX, dY, dZ;

    void updateDeltas();

    GLdouble deg2rad;

public:
    geAnimation(const std::string& iId, float iSpan, unsigned int iType);
    virtual ~geAnimation();

    void insertPoint(xyzPointDouble in);
    void insertPoint(GLdouble ix, GLdouble iy, GLdouble iz);

    unsigned long getDeltaTime();
    std::string getID();

    /* Receives the (approximated) time passed since last update */
    void updateAnimation(unsigned long timePassed);
    void applyAnimation();
};

} //namespace ge

#endif /* GEANIMATION_HPP_ */
