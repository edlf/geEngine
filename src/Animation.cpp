/*
 * Eduardo Fernandes
 *
 * Animation class methods.
 */

#include <Animation.hpp>

namespace ge {
void Animation::updateDeltas() {
    if (deltaTimeSingleMove > 0) {
        dX = abs(currentDestionationPoint.x - currentSourcePoint.x)
                / deltaTimeSingleMove;
        dY = abs(currentDestionationPoint.y - currentSourcePoint.y)
                / deltaTimeSingleMove;
        dZ = abs(currentDestionationPoint.z - currentSourcePoint.z)
                / deltaTimeSingleMove;
    }
}

void Animation::updateAngles() {
    /* Must fix angles */
    angleX = acos(dX / dY) * deg2rad;
    angleZ = asin(dZ / dY) * deg2rad;
}

Animation::Animation(const std::string& iId, float iSpan,
        unsigned int iType) {
    if (iType != 1) {
        throw Exception("Bug: Invalid type fed into Animation!", true);
    }

    this->deg2rad = M_PI / 180.0;

    this->id = iId;
    this->span = iSpan;
    this->type = iType;

    this->numberOfControlPoints = 0;
    this->numberOfProcessedControlPoints = 0;
    this->deltaTimeMillis = 0;

    this->updateCounter = 0;

    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->angleX = 0;
    this->angleZ = 0;

    this->dX = 0;
    this->dY = 0;
    this->dZ = 0;

    this->spanMilliseconds = this->span * 1000;
    this->totalTimePassed = 0;

    /* Change this after receiving the second point */
    this->hasEnded = true;

    this->deltaTimeSingleMove = 0;
}

void Animation::insertPoint(xyzPointDouble in) {
    insertPoint(in.x, in.y, in.z);
}

void Animation::insertPoint(GLdouble ix, GLdouble iy, GLdouble iz) {
    /* Cheap way to set the first start point */
    if (numberOfControlPoints == 0) {
        currentSourcePoint.x = ix;
        currentSourcePoint.y = iy;
        currentSourcePoint.z = iz;
    } else if (numberOfControlPoints == 1) {
        currentDestionationPoint.x = ix;
        currentDestionationPoint.y = iy;
        currentDestionationPoint.z = iz;

        /* Do the first delta update */
        updateDeltas();
        updateAngles();

        /* We've got at least two points so the animation now can be processed */
        this->hasEnded = false;

        /* We just processed the 0 and 1 point */
        numberOfProcessedControlPoints = 2;
    }

    xyzPointDouble temp;
    temp.x = ix;
    temp.y = iy;
    temp.z = iz;

    controlPoints.push_back(temp);
    numberOfControlPoints++;

    if (numberOfControlPoints != 0) {
        /* Multiply by 1000 to convert to milliseconds */
        deltaTimeMillis =
                (long) ((span / (float) numberOfControlPoints) * 1000);

        deltaTimeSingleMove = (long) ((span / (float) numberOfControlPoints)
                * 1000 / GLUT_UPDATE_MS);

    } else {
        throw Exception("Bug: Division by zero on inserPoint at Animation.",
                true);
    }
}

unsigned long Animation::getDeltaTime() {
    return this->deltaTimeMillis;
}

std::string Animation::getID() {
    return this->id;
}

void Animation::updateAnimation(unsigned long timePassed) {
    /* Check if the animation has ended */
    if (hasEnded) {
        return;
    }

    /* Sum up all the time */
    totalTimePassed = totalTimePassed + timePassed;

    /* Check if we have a valid delta */
    if ((deltaTimeMillis > 0)) {

        /* We are at/or we have passed the time for the next control point */
        if (updateCounter >= deltaTimeMillis) {
            updateCounter = 0;

            /* Change animation control points here */
            if (numberOfProcessedControlPoints < numberOfControlPoints) {
                /* Swap points */
                currentSourcePoint = currentDestionationPoint;
                currentDestionationPoint =
                        controlPoints[numberOfProcessedControlPoints];

                /* Calculate new deltas */
                updateDeltas();
                updateAngles();

                /* Increment number of processed control points */
                numberOfProcessedControlPoints++;
            }
            /* Control points swap done */

        } else {
            /* Increment updateCounter and move the object towards the destination point */
            updateCounter = updateCounter + timePassed;

            /* Move object */
            x = x + dX;
            y = y + dY;
            z = z + dZ;
        }

    }

    /* Check if we reached the end of the animation */
    if (totalTimePassed >= spanMilliseconds) {
        this->hasEnded = true;

        angleX = 0;
        angleZ = 0;
    }
}

void Animation::applyAnimation() {
    /* Apply rotation */
    glRotated(angleX, 1, 0, 0);
    glRotated(angleZ, 0, 0, 1);

    /* Translate to new position */
    glTranslated(x, y, z);
}

Animation::~Animation() {

}

}
