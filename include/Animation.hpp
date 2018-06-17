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

    std::vector<gePoint> controlPoints;
    unsigned int numberOfControlPoints;

    unsigned long deltaTimeMillis;
    unsigned long updateCounter;

    unsigned long deltaTimeSingleMove;

    unsigned long spanMilliseconds;
    unsigned long totalTimePassed;

    float x, y, z;

    float angleX;
    float angleZ;

    void updateAngles();

    bool hasEnded;

    gePoint currentSourcePoint;
    gePoint currentDestionationPoint;
    unsigned int numberOfProcessedControlPoints;

    float dX, dY, dZ;

    void updateDeltas();

    float deg2rad;

public:
    geAnimation(const std::string& iId, float iSpan, unsigned int iType);

    void insertPoint(gePoint in);
    void insertPoint(GLfloat ix, GLfloat iy, GLfloat iz);

    unsigned long getDeltaTime();
    std::string getID();

    /* Receives the (approximated) time passed since last update */
    void updateAnimation(unsigned long timePassed);
    void applyAnimation();

    ~geAnimation();
};

} //namespace ge

#endif /* GEANIMATION_HPP_ */
