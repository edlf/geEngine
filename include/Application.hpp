/*
 * Eduardo Fernandes
 *
 * Global UI variables.
 */

#ifndef GEAPP_HPP_
#define GEAPP_HPP_

#include <Scene.hpp>
#include "includes.hpp"

namespace ge {

namespace Defaults {
const unsigned int InitialWindowSizeX = 800;
const unsigned int InitialWindowSizeY = 600;
const unsigned int InitialWindowPosX = 50;
const unsigned int InitialWindowPosY = 50;

const double MouseRotateFactor= 0.25;
const double MousePanFactor = 0.025;
const double MouseZoomFactor = 0.25;
}

class Application {

public:
    Application(int argCount, char** args, std::string& sceneFileName);
    virtual ~Application();

private:
    /* Input args */
    static int argCount;
    static char** arguments;

    static std::string sceneFile;

    /* glui window variables */
    static int mainWindow;
    static GLUI *gluiSubWindow;
    static int keyModifiers;
    static bool panelVisibility;

    static unsigned int width, height;
    static unsigned int mousePosX, mousePosY;

    static unsigned int numberOfLightCheckboxes;
    static unsigned int numberOfCameraComboBoxEntries;

    static int lightEnableStatus[8];
    static GLUI_Checkbox* lightcb[8];
    static std::vector<std::string> lightIDs;

    static GLUI_Listbox *cameraListBox;
    static std::vector<std::string> cameraIDs;
    static int cameraListBoxStatus;

    static int polygonModeStatus;

    /* Scene Pointer */
    static Scene* scene;

    /* State and control variables to handle mouse interaction */
    static int displacementX;
    static int displacementY;
    static bool pressingMouseLeft;
    static bool pressingMouseMiddle;
    static bool pressingMouseRight;
    static int prev_X;
    static int prev_Y;

    /* Picking buffer */
    static GLuint selectBuf[PICKING_BUFFER_SIZE];

    static void geInitialize();
    static void display();
    static void reshape(int w, int h);
    static void glutIdle(void);

    static void geUpdate(int value);

    static void snapshot();

    static void callBack(int control);

    static void processMouse(int button, int state, int x, int y);
    static void processMouseMoved(int x, int y);
    static void processPassiveMouseMoved(int x, int y);

    static void keyboard(unsigned char key, int x, int y);
    static void specialKeyboard(int key, int x, int y);

    static void performPicking(int x, int y);
    static void processHits(GLint hits, GLuint buffer[]);

    static void glutConfig(int argc, char** argv);

    static void toggleLight(unsigned int);
};

}

#endif /* GEAPP_HPP_ */
