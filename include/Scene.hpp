/*
 * Eduardo Fernandes
 *
 * Scene class.
 */

#ifndef GELSCENE_HPP_
#define GELSCENE_HPP_

#include <Animation.hpp>
#include <Appearance.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <Primitives.hpp>
#include <SceneGraph.hpp>
#include <xmlDefinitions.hpp>
#include "includes.hpp"

#define TIXML_USE_STL
#include <tinyxml.h>


namespace ge {

class Scene {
private:
    /* Glut window size */
    int windowSizeX, windowSizeY;
    double aspectRatio;

    std::string name;

    /* Globals */
    GLclampf backgroundColour[4];
    std::string drawMode;
    int drawModeInt;
    std::string shadingMode;
    std::string cullFace;
    std::string cullOrder;

    void setBackgroundColor(color in);
    void applyGlobals();
    /* End globals */

    /* Cameras */
    std::string initialCamera;
    CameraInterface* currentCameraPointer;
    std::vector<CameraInterface*> cameraVector;

    unsigned int numberOfCameras;

    GLdouble* customCameraMatrix;
    void setInitialCamera();
    void setViewRotate(GLdouble* input);
    GLdouble* getViewProjection();

    /* Lights */
    GLboolean lightingDoubleSided;
    GLboolean lightingLocal;
    GLboolean lightingEnable;
    GLfloat ambientLightColour[4];
    std::vector<Light*> lightVector;
    unsigned int numberOfLights;

    void setAmbientLightColor(color in);
    void initLights();

    /* Textures */
    std::vector<Texture*> textureVector;
    void initTextures();

    /* Appearances */
    std::vector<Appearance*> appearanceVector;
    Appearance* getAppearanceByString(std::string& in);
    void initAppearanceTextures();

    /* Graph */
    SceneGraph* graph;
    std::vector<Node*> unprocessedNodes;

    /* Animations */
    std::vector<Animation*> animationsVector;
    Animation* getAnimationByString(std::string& in);

    /* Shader */
    std::vector<Primitives::WaterLine*> waterLineVector;

    /* Internal variables and methods for xml parsing */
    void parseAndLoadXml(std::string& fileName);

    char* xmlFileName;
    TiXmlDocument* doc;

    void xmlLoadFile(const std::string& filename);

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

    static TiXmlElement *findChildByAttribute(TiXmlElement *parent,
            const char * attr, const char *val);

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
    std::string getStringFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    xyzPointDouble getDoublePointFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    xyzPointFloat getFloatPointFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    color getColorFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    double getDoubleFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    float getFloatFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    unsigned int getUnsignedIntFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);
    bool getAttributeExistence(TiXmlElement* iElement, const std::string& iAttribute);
    xyPointDouble get2DdPointFromElementAttribute(TiXmlElement* iElement,
            const std::string& iAttribute, const std::string& Error);

    /* End XML related methods */

    /* Sanity checks */
    GLdouble validateOpenGLColour(GLdouble input);
    bool validateBoolean(std::string&);

    /* Validate and set strings */
    void setDrawMode(std::string&);
    void setShadingMode(std::string&);
    void setCullFace(std::string&);
    void setCullOrder(std::string&);

    /* Internal display functions */
    void displayLights();
    void applyCameraView();

    /* Update stuff */
    void checkUpdate(); ///< Called by the main application class, that handles timing, to check if an update is needed due to timeout. If so, it calls update(), and updates the timer.
    unsigned long updatePeriod;
    unsigned long lastUpdate;

    /* Vehicle pointer */
    std::vector<Primitives::Vehicle*> sceneVehicles;

public:
    PerspectiveCamera* externalGuiCamera;

    /* Constructor who receives an string with the file name to load the scene from. */
    Scene(std::string& fileName);
    virtual ~Scene();

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
    std::string getLightID(unsigned int number);

    unsigned int getNumberOfCameras();
    void resetUserCamera();
    std::string getCameraID(unsigned int number);
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
};

}
#endif
