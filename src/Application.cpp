/*
 * Eduardo Fernandes
 *
 * Entry point and user interface methods.
 */
#include <Application.hpp>

namespace ge {
/* Input args */
int     Application::argCount;
char**  Application::arguments;

std::string Application::sceneFile;

/* glui window variables */
int Application::mainWindow;
GLUI* Application::gluiSubWindow;
int Application::keyModifiers;
bool Application::panelVisibility;

unsigned int Application::width, Application::height;
unsigned int Application::mousePosX, Application::mousePosY;

unsigned int Application::numberOfLightCheckboxes;
unsigned int Application::numberOfCameraComboBoxEntries;

int Application::lightEnableStatus[8];
GLUI_Checkbox* Application::lightcb[8];
std::vector<std::string> Application::lightIDs;

GLUI_Listbox* Application::cameraListBox;
std::vector<std::string> Application::cameraIDs;
int Application::cameraListBoxStatus;

int Application::polygonModeStatus;

/* Scene Pointer */
ge::Scene* Application::scene;

/* State and control variables to handle mouse interaction */
int  Application::displacementX;
int  Application::displacementY;
bool Application::pressingMouseLeft;
bool Application::pressingMouseMiddle;
bool Application::pressingMouseRight;
int  Application::prev_X;
int  Application::prev_Y;

/* Picking buffer */
GLuint Application::selectBuf[PICKING_BUFFER_SIZE];

Application::Application(int argCount, char** args, std::string& sceneFileName){
	this->argCount = argCount;
	this->arguments = args;

	this->sceneFile = sceneFileName;

	try {
		scene = new ge::Scene(sceneFile);
	} catch (geException& e) {
	    e.printerErrorMessage();
	}

	glutConfig(argCount, arguments);

	geInitialize();

	/* RTFM */
	std::cout << "Check readme file for instructions." << std::endl;

	glutMainLoop();
}

Application::~Application(){

}

/* Call snapshot function and tell user about it */
void Application::snapshot() {
    takeSnapshot(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    std::cout << "Snapshot taken." << std::endl;
}

/* Toggle a light */
void Application::toggleLight(unsigned int i) {
    if (lightEnableStatus[i] == 1) {
        scene->enableLight(i);
    } else {
        scene->disableLight(i);
    }
}

/* Call function for GLUI */
void Application::callBack(int control) {
    /* Call back IDs
     * 1xx Draw Mode
     * 2xx Lights
     * 3xx Cameras
     * 4xx Buttons
     */

    switch (control) {
    /* Draw mode has changed */
    case 100:
        scene->reSetPolygonMode(polygonModeStatus);
        break;

        /* Some light has been toggled */
    case 200:
        toggleLight(0);
        break;

    case 201:
        toggleLight(1);
        break;

    case 202:
        toggleLight(2);
        break;

    case 203:
        toggleLight(3);
        break;

    case 204:
        toggleLight(4);
        break;

    case 205:
        toggleLight(5);
        break;

    case 206:
        toggleLight(6);
        break;

    case 207:
        toggleLight(7);
        break;

        /* Someone wants a different view */
    case 300:
        if (cameraListBox->get_int_val() == 399) {
            scene->changeCameraToExernal();
        } else {
            scene->setCamera(cameraListBox->get_int_val());
        }

        glutPostRedisplay();
        break;

    case 400:
        scene->resetUserCamera();
        glutPostRedisplay();
        break;

    case 401:
        gluiSubWindow->hide();
        panelVisibility = false;
        glutPostRedisplay();
        break;

    case 402:
        snapshot();
        break;

        /* Rats */
    default:
        break;
    }

}

/* Calls scene display function. */
void Application::display(void) {
    scene->display();
}

void Application::processMouse(int button, int state, int x, int y) {
    prev_X = x;
    prev_Y = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        pressingMouseLeft = true;
    } else {
        pressingMouseLeft = false;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        pressingMouseRight = true;
    } else {
        pressingMouseRight = false;
    }

    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        pressingMouseMiddle = true;
    } else {
        pressingMouseMiddle = false;
    }

    glutPostRedisplay();
}

void Application::processMouseMoved(int x, int y) {
    displacementX = x - static_cast<int>(prev_X);
    displacementY = y - static_cast<int>(prev_Y);

    if (pressingMouseLeft && keyModifiers == 0) {
        scene->externalGuiCamera->rotate(0, displacementY * MOUSE_ROTATE_FACTOR);
        scene->externalGuiCamera->rotate(1, displacementX * MOUSE_ROTATE_FACTOR);

    } else if (pressingMouseRight && keyModifiers == 0) {
        scene->externalGuiCamera->translate(0, displacementX * MOUSE_PAN_FACTOR);
        scene->externalGuiCamera->translate(1, -displacementY * MOUSE_PAN_FACTOR);

    } else if (pressingMouseMiddle || (pressingMouseLeft && (keyModifiers & GLUT_ACTIVE_CTRL))) {
        scene->externalGuiCamera->translate(2, displacementY * MOUSE_ZOOM_FACTOR);
    }

    prev_X = x;
    prev_Y = y;

    /* Ask scene to change camera to the special GUI cam (Id: 399 for call back) */
    scene->changeCameraToExernal();
    cameraListBox->set_int_val(399);

    /* Redisplay */
    glutPostRedisplay();
}

void Application::processPassiveMouseMoved(int x, int y) {
    /* Store mouse pos */
    mousePosX = x;
    mousePosY = y;

    /* Redisplay */
    glutPostRedisplay();
}

void Application::reshape(int w, int h) {
    width = w;
    height = h;
    scene->setCurrentWindowSize(width, height);
    glutPostRedisplay();
}

void Application::keyboard(unsigned char key, int x, int y) {
    /* Store mouse pos */
    mousePosX = x;
    mousePosY = y;

    keyModifiers = glutGetModifiers();

    switch (key) {
    case 27:		// ESC
        exit(0);
        break;

    case 'p':
        snapshot();
        break;

        /* Vehicle control */
    case 'w':
        scene->moveVehicleFront();
        break;

    case 's':
        scene->moveVehicleBack();
        break;

    case 'q':
        scene->moveVehicleUp();
        break;

    case 'e':
        scene->moveVehicleDown();
        break;

    case 'a':
        scene->moveVehicleLeft();
        break;

    case 'd':
        scene->moveVehicleRight();
        break;

    case 'z':
        scene->tiltVehicleLeft();
        break;

    case 'x':
        scene->tiltVehicleRight();
        break;

    }
}

void Application::specialKeyboard(int key, int x, int y) {
    /* Store mouse pos */
    mousePosX = x;
    mousePosY = y;

    keyModifiers = glutGetModifiers();

    switch (key) {
    case GLUT_KEY_HOME:
        scene->changeCameraToSceneDefault();
        glutPostRedisplay();
        break;

    case GLUT_KEY_END:
        if (panelVisibility) {
            gluiSubWindow->hide();
        } else {
            gluiSubWindow->show();
        }

        panelVisibility = !panelVisibility;

        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void Application::glutIdle(void) {
    /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
    if (glutGetWindow() != mainWindow) {
        glutSetWindow(mainWindow);
    }

    glutPostRedisplay();
}

void Application::geUpdate(int value) {
    /* Call the scene update method */
    scene->update(value);

    /* Redraw and activate timer again */
    glutPostRedisplay();

    glutTimerFunc(GLUT_UPDATE_MS, geUpdate, GLUT_UPDATE_MS);
}

void Application::geInitialize() {
    /* Initialize glew */
    glewInit();

    /* Initialize global variables */
    displacementX = 0;
    displacementY = 0;

    pressingMouseLeft = false;
    pressingMouseRight = false;
    pressingMouseMiddle = false;

    prev_X = 0;
    prev_Y = 0;

    /* Check if scene has a valid pointer */
    if (scene == NULL) {
        std::cerr << "Null scene pointer. You got bugs to fix!" << std::endl;
        exit(-1);
    }

    /* Ask scene to initialize and parse the yaf */
    scene->init();

    /* UI stuff goes here */

    /* The panel is visible when the app is opened */
    panelVisibility = true;

    /* Get number of lights/cameras  and the polygon mode for the UI */
    numberOfLightCheckboxes = scene->getNumberOfLights();
    numberOfCameraComboBoxEntries = scene->getNumberOfCameras();
    polygonModeStatus = scene->getPolygonMode();

    /* Polygon Mode Column */
    GLUI_Panel* polygonPanel;
    polygonPanel = gluiSubWindow->add_panel("Polygon Mode", 1);
    polygonPanel->set_alignment(1);

    GLUI_RadioGroup *radioGroup;

    radioGroup = gluiSubWindow->add_radiogroup_to_panel(polygonPanel, &polygonModeStatus, 100, callBack);
    gluiSubWindow->add_radiobutton_to_group(radioGroup, "Fill");
    gluiSubWindow->add_radiobutton_to_group(radioGroup, "Wireframe");
    gluiSubWindow->add_radiobutton_to_group(radioGroup, "Point");
    /* End polygon mode */

    /* Get light IDs */
    for (unsigned int i = 0; i < numberOfLightCheckboxes; i++) {
        lightIDs.push_back(scene->getLightID(i));
    }
    gluiSubWindow->add_statictext(" ");
    GLUI_Panel* lightPanel;
    lightPanel = gluiSubWindow->add_panel("Lights", 1);
    lightPanel->set_alignment(1);

    for (unsigned int i = 0; i < numberOfLightCheckboxes; i++) {
        lightcb[i] = gluiSubWindow->add_checkbox_to_panel(lightPanel, lightIDs[i].c_str(), &lightEnableStatus[i], 200 + i, callBack);
    }

    for (unsigned int i = 0; i < numberOfLightCheckboxes; i++) {
        if (scene->getLightStatus(i)) {
            lightEnableStatus[i] = 1;
        } else {
            lightEnableStatus[i] = 0;
        }
    }
    /* End light column */

    /* Camera Column */
    /* Get camera IDs */
    for (unsigned int i = 0; i < numberOfCameraComboBoxEntries; i++) {
        cameraIDs.push_back(scene->getCameraID(i));
    }

    gluiSubWindow->add_statictext(" ");
    GLUI_Panel* cameraPanel;
    cameraPanel = gluiSubWindow->add_panel("Camera", 1);
    cameraPanel->set_alignment(1);
    cameraListBox = gluiSubWindow->add_listbox_to_panel(cameraPanel, "ID:", &cameraListBoxStatus, 300, callBack);

    for (unsigned int i = 0; i < numberOfCameraComboBoxEntries; i++) {
        cameraListBox->add_item(i, cameraIDs[i].c_str());
    }

    cameraListBox->add_item(399, "User Camera");
    cameraListBox->set_int_val(scene->getCurrentCamera());

    gluiSubWindow->add_button_to_panel(cameraPanel, "Reset User Cam", 400, callBack);

    gluiSubWindow->add_statictext(" ");
    gluiSubWindow->add_button("Hide Panel", 401, callBack);
    gluiSubWindow->add_statictext(" ");
    gluiSubWindow->add_button("Snapshot", 402, callBack);

    /* Sync Live variables */
    gluiSubWindow->sync_live();

    /* Activate timer for updates */
    glutTimerFunc(GLUT_UPDATE_MS, geUpdate, GLUT_UPDATE_MS);
}

/* Picking */
void Application::performPicking(int x, int y) {
    // Sets the buffer to be used for selection and activate selection mode
    glSelectBuffer (PICKING_BUFFER_SIZE, selectBuf);
    glRenderMode(GL_SELECT);

    // Initialize the picking name stack
    glInitNames();

    // The process of picking manipulates the projection matrix
    // so we will be activating, saving and manipulating it
    glMatrixMode(GL_PROJECTION);

    //store current projection matrix to restore easily in the end with a pop
    glPushMatrix ();

    //get the actual projection matrix values on an array of our own to multiply with pick matrix later
    GLfloat projmat[16];
    glGetFloatv(GL_PROJECTION_MATRIX,projmat);

    // reset projection matrix
    glLoadIdentity();

    // get current viewport and use it as reference for
    // setting a small picking window of 5x5 pixels around mouse coordinates for picking
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // this is multiplied in the projection matrix
    gluPickMatrix ((GLdouble) x, (GLdouble) (height - y), 5.0, 5.0, viewport);

    // multiply the projection matrix stored in our array to ensure same conditions as in normal render
    glMultMatrixf(projmat);

    // force scene drawing under this mode
    // only the names of objects that fall in the 5x5 window will actually be stored in the buffer
    scene->display();

    // restore original projection matrix
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();

    glFlush();

    // revert to render mode, get the picking results and process them
    GLint hits;
    hits = glRenderMode(GL_RENDER);
    processHits(hits, selectBuf);
}

void Application::processHits(GLint hits, GLuint buffer[]) {
    GLuint *ptr = buffer;
    GLuint mindepth = 0xFFFFFFFF;
    GLuint *selected=NULL;
    GLuint nselected;

    // iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
    for (int i=0;i<hits;i++) {
        int num = *ptr; ptr++;
        GLuint z1 = *ptr; ptr++;
        ptr++;
        if (z1 < mindepth && num>0) {
            mindepth = z1;
            selected = ptr;
            nselected=num;
        }
        for (int j=0; j < num; j++)
            ptr++;
    }

    // if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
    if (selected!=NULL)
    {
        // this should be replaced by code handling the picked object's ID's (stored in "selected"),
        // possibly invoking a method on the scene class and passing "selected" and "nselected"
        printf("Picked ID's: ");
        for (unsigned int i = 0; i < nselected; ++i) {
        	printf("%d ",selected[i]);
        }
        printf("\n");
    }
    else {
    	printf("Nothing selected while picking \n");
    }
}

void Application::glutConfig(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    width  = WINDOW_SIZE_X;
    height = WINDOW_SIZE_Y;
    glutInitWindowSize(width, height);
    glutInitWindowPosition(WINDOW_INITIALPOS_X, WINDOW_INITIALPOS_Y);
    mainWindow = glutCreateWindow(APP_NAME_VERSION);

    glutDisplayFunc(display);

    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutKeyboardFunc(keyboard);
    GLUI_Master.set_glutMouseFunc(processMouse);
    glutMotionFunc(processMouseMoved);
    glutPassiveMotionFunc(processPassiveMouseMoved);

    GLUI_Master.set_glutSpecialFunc(specialKeyboard);
    GLUI_Master.set_glutIdleFunc(glutIdle);

    /* Left division controls */
    gluiSubWindow = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_LEFT);
    gluiSubWindow->set_main_gfx_window(mainWindow);
}
}
