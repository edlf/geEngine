/*
 * Eduardo Fernandes
 *
 * Global UI variables.
 */

#ifndef GEAPP_HPP_
#define GEAPP_HPP_

#include "includes.hpp"
#include "geScene.hpp"

using namespace std;

/* glui window variables */
int mainWindow;
GLUI *gluiSubWindow;
static int keyModifiers;
bool panelVisibility;

unsigned int width, height;
unsigned int mousePosX, mousePosY;

static unsigned int numberOfLightCheckboxes;
static unsigned int numberOfCameraComboBoxEntries;

int lightEnableStatus[8];
GLUI_Checkbox* lightcb[8];
vector<string> lightIDs;

GLUI_Listbox *cameraListBox;
vector<string> cameraIDs;
int cameraListBoxStatus;

int polygonModeStatus;

/* Scene Pointer */
geScene* scene;

/* State and control variables to handle mouse interaction */
float displacementX;
float displacementY;
bool pressingMouseLeft;
bool pressingMouseMiddle;
bool pressingMouseRight;
float prev_X;
float prev_Y;

/* Picking buffer */
GLuint selectBuf[PICKING_BUFFER_SIZE];

/* Prototypes */
void processHits(GLint hits, GLuint buffer[]);

#endif /* GEAPP_HPP_ */
