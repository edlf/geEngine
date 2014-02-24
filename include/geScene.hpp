/*
 * Eduardo Fernandes
 *
 * Scene class.
 */

#ifndef GELSCENE_HPP_
#define GELSCENE_HPP_

#include "includes.hpp"
#include "geXmlDefs.hpp"

#include "tinyxml.h"

#include "geCamera.hpp"
#include "geLight.hpp"
#include "geAppearance.hpp"
#include "gePrimitive.hpp"
#include "geGraph.hpp"
#include "geAnimation.hpp"

using namespace std;

class geScene {
private:
    /* Glut window size */
    int windowSizeX, windowSizeY;
    double aspectRatio;

    string name;

    /* Globals */
    GLfloat backgroundColour[4];
    string drawMode;
    int drawModeInt;
    string shadingMode;
    string cullFace;
    string cullOrder;

    void setBackgroundColor(geColor in);
    void applyGlobals();
    /* End globals */

    /* Cameras */
    string initialCamera;
    geCamera* currentCameraPointer;
    vector<geCamera*> cameraVector;

    unsigned int numberOfCameras;

    GLfloat* customCameraMatrix;
    void setInitialCamera();
    void setViewRotate(GLfloat* input);
    GLfloat* getViewProjection();

    /* Lights */
    GLboolean lightingDoubleSided;
    GLboolean lightingLocal;
    GLboolean lightingEnable;
    GLfloat ambientLightColour[4];
    vector<geLight*> lightVector;
    unsigned int numberOfLights;

    void setAmbientLightColor(geColor in);
    void initLights();

    /* Textures */
    vector<geTexture*> textureVector;
    void initTextures();

    /* Appearances */
    vector<geAppearance*> appearanceVector;
    geAppearance* getAppearanceByString(string in);
    void initAppearanceTextures();

    /* Graph */
    geGraph* graph;
    vector<geNode*> unprocessedNodes;

    /* Animations */
    vector<geAnimation*> animationsVector;
    geAnimation* getAnimationByString(string in);

    /* Shader */
    vector<geWaterLine*> waterLineVector;

    /* Internal variables and methods for xml parsing */
    void parseAndLoadXml(string fileName);

    char* xmlFileName;
    TiXmlDocument* doc;

    void xmlLoadFile(char *filename);

    void xmlCheckDocumentType();

    void xmlLoadMainElements();
    void xmlCheckMainElements();

    bool xmlGlobalsLoaded;
    bool xmlCamerasLoaded;
    bool xmlLightsLoaded;
    bool xmlTexturesLoaded;
    bool xmlAppearancesLoaded;
    bool xmlGraphLoaded;
    bool xmlAnimationsLoaded;

    void xmlLoadGlobals();
    void xmlLoadCameras();
    void xmlLoadLighting();
    void xmlLoadTextures();
    void xmlLoadAppearances();
    void xmlLoadGraph();
    void xmlLoadAnimations();

    static TiXmlElement *findChildByAttribute(TiXmlElement *parent, const char * attr, const char *val);

    /* XML Elements */
    TiXmlElement* xmlRootElement;

    TiXmlElement* xmlGlobalsElement;
    TiXmlElement* xmlCamerasElement;
    TiXmlElement* xmlLightingElement;
    TiXmlElement* xmlTexturesElement;
    TiXmlElement* xmlAppearancesElement;
    TiXmlElement* xmlGraphElement;
    TiXmlElement* xmlAnimationsElement;

    /* XML Parse methods */
    string getStringFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);
    gePoint getPointFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);
    geColor getColorFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);
    float getFloatFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);
    unsigned int getUnsignedIntFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);
    bool getAttributeExistence(TiXmlElement* iElement, char const* iAttribute);
    ge2dPoint get2DdPointFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error);

    /* End XML related methods */

    /* Sanity checks */
    GLfloat validateOpenGLColour(GLfloat input);
    bool validateBoolean(string input);

    /* Validate and set strings */
    void setDrawMode(string input);
    void setShadingMode(string input);
    void setCullFace(string input);
    void setCullOrder(string input);

    /* Internal display functions */
    void displayLights();
    void applyCameraView();

    /* Update stuff */
    void checkUpdate(); ///< Called by the main application class, that handles timing, to check if an update is needed due to timeout. If so, it calls update(), and updates the timer.
    unsigned long updatePeriod;
    unsigned long lastUpdate;

    /* Vehicle pointer */
    vector<geVehicle*> sceneVehicles;

public:
    geCameraPerspective* externalGuiCamera;

    /* Constructor who receives an string with the file name to load the scene from. */
    geScene(string fileName);

    /* Gets */
    GLboolean getLightingEnableStatus();

    /*  */
    virtual void init();
    virtual void display();
    virtual void update(unsigned long millis);
    void activateCamera(unsigned int i);
    void changeCameraToExernal();
    void changeCameraToSceneDefault();

    unsigned int getNumberOfLights();
    int getPolygonMode();
    void reSetPolygonMode(int in);

    bool getLightStatus(unsigned int number);
    void disableLight(unsigned int number);
    void enableLight(unsigned int number);
    string getLightID(unsigned int number);

    unsigned int getNumberOfCameras();
    void resetUserCamera();
    string getCameraID(unsigned int number);
    void setCamera(unsigned int number);
    unsigned int getCurrentCamera();

    /* For reshape */
    void setCurrentWindowSize(int windowX, int windowY);
    /*Implement*/
    bool toggleLight(int lightID);
    bool getLightStatus(int lightID);

    /* Vehicle control */
    void moveVehicleUp();
    void moveVehicleDown();

    void moveVehicleFront();
    void moveVehicleBack();

    void moveVehicleLeft();
    void moveVehicleRight();

    void tiltVehicleLeft();
    void tiltVehicleRight();

    /*  */
    virtual ~geScene();
};

#endif
