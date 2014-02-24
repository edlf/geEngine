/*
 * Eduardo Fernandes
 *
 * Scene class methods.
 */

#include "geScene.hpp"

geScene::geScene(string fileName) {
    currentCameraPointer = NULL;

    parseAndLoadXml(fileName);

    /* Set current camera pointer to the initial camera defined in the XML */
    setInitialCamera();

    /* Create an camera that can be used to override the scene cameras */
    externalGuiCamera = new geCameraPerspective();
}

void geScene::parseAndLoadXml(string fileName) {
    xmlGlobalsLoaded = false;
    xmlCamerasLoaded = false;
    xmlLightsLoaded = false;
    xmlTexturesLoaded = false;
    xmlAppearancesLoaded = false;
    xmlGraphLoaded = false;
    xmlAnimationsLoaded = false;

    char *xmlFileName = new char[fileName.length() + 1];
    strcpy(xmlFileName, fileName.c_str());

    xmlLoadFile(xmlFileName);

    xmlCheckDocumentType();

    xmlLoadMainElements();
    xmlCheckMainElements();

    xmlLoadGlobals();
    xmlLoadCameras();
    xmlLoadLighting();
    xmlLoadTextures();
    xmlLoadAppearances();

    /* Before attempting to load animations check if the section exists */
    if (xmlAnimationsElement != NULL) {
        xmlLoadAnimations();
    }

    /* Graph is loaded last, since it will get references for the rest */
    xmlLoadGraph();

    if (doc != NULL) {
        delete (doc);
    }

    delete[] xmlFileName;

    if (!(xmlGlobalsLoaded && xmlCamerasLoaded && xmlLightsLoaded && xmlTexturesLoaded && xmlAppearancesLoaded && xmlGraphLoaded)) {
        throw geException("Something went wrong with the XML parsing.", true);
    }
}

void geScene::xmlLoadFile(char *filename) {
    doc = new TiXmlDocument(filename);
    bool loadOkay = doc->LoadFile();

    if (!loadOkay) {
        throw geException("Error while loading scene xml file.");
    }
}

void geScene::xmlCheckDocumentType() {
    xmlRootElement = doc->FirstChildElement(XML_DOCUMENT_TYPE);

    if (xmlRootElement == NULL) {
        throw geException("No correct root element found!", true);
    }
}

void geScene::xmlLoadMainElements() {
    xmlGlobalsElement = xmlRootElement->FirstChildElement(XML_SECTION_GLOBALS);
    xmlCamerasElement = xmlRootElement->FirstChildElement(XML_SECTION_CAMERAS);
    xmlLightingElement = xmlRootElement->FirstChildElement(XML_SECTION_LIGHTS);
    xmlTexturesElement = xmlRootElement->FirstChildElement(XML_SECTION_TEXTURES);
    xmlAppearancesElement = xmlRootElement->FirstChildElement(XML_SECTION_APPEARANCES);
    xmlGraphElement = xmlRootElement->FirstChildElement(XML_SECTION_GRAPH);
    xmlAnimationsElement = xmlRootElement->FirstChildElement(XML_SECTION_ANIMATIONS);
}

void geScene::xmlCheckMainElements() {
    /* Check if all required nodes exist */

    if (xmlGlobalsElement == NULL) {
        throw geException("Globals element not found.");
    }

    if (xmlCamerasElement == NULL) {
        throw geException("Cameras element not found.");
    }

    if (xmlLightingElement == NULL) {
        throw geException("Lighting element not found.");
    }

    if (xmlTexturesElement == NULL) {
        throw geException("Textures element not found.");
    }

    if (xmlAppearancesElement == NULL) {
        throw geException("Appearances element not found.");
    }

    if (xmlGraphElement == NULL) {
        throw geException("Graph element not found.");
    }
}

/* Load and set globals from XML */
void geScene::xmlLoadGlobals() {
    if (xmlGlobalsLoaded) {
        throw geException(XML_SECTION_GLOBALS_ERROR, true);
    }

    geColor bgColor = getColorFromElementAttribute(xmlGlobalsElement, XML_ATTRIBUTE_BACKGROUND, XML_ATTRIBUTE_BACKGROUND_ERROR);
    setBackgroundColor(bgColor);

    string drawMode = getStringFromElementAttribute(xmlGlobalsElement, XML_ATTRIBUTE_DRAWMODE, XML_ATTRIBUTE_DRAWMODE_ERROR);
    setDrawMode(drawMode);

    string shadingMode = getStringFromElementAttribute(xmlGlobalsElement, XML_ATTRIBUTE_SHADING, XML_ATTRIBUTE_SHADING_ERROR);
    setShadingMode(shadingMode);

    string cullFace = getStringFromElementAttribute(xmlGlobalsElement, XML_ATTRIBUTE_CULLFACE, XML_ATTRIBUTE_CULLFACE_ERROR);
    setCullFace(cullFace);

    string cullOrder = getStringFromElementAttribute(xmlGlobalsElement, XML_ATTRIBUTE_CULLORDER, XML_ATTRIBUTE_CULLORDER_ERROR);
    setCullOrder(cullOrder);

    xmlGlobalsLoaded = true;
}

/* Load and set cameras from XML */
void geScene::xmlLoadCameras() {
    if (xmlCamerasLoaded) {
        throw geException("XML: Cameras have already been loaded!", true);
    }

    int numberOfPerspectiveCameras = 0, numberOfOrthoCameras = 0;

    this->numberOfCameras = 0;
    this->initialCamera = getStringFromElementAttribute(xmlCamerasElement, XML_ATTRIBUTE_INITIAL_CAMERA, XML_ATTRIBUTE_INITIAL_CAMERA_ERROR);

    /* Get camera first child */
    TiXmlElement* xmlCameraElement = xmlCamerasElement->FirstChildElement();

    bool camerasLastRun = false;
    while (xmlCameraElement != NULL && !camerasLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlCameraElement == xmlCamerasElement->LastChild()) {
            camerasLastRun = true;
        }

        /* Camera is perspective */
        if (strcmp(xmlCameraElement->Value(), XML_SECTION_CAMERA_PERSPECTIVE) == 0) {
            string cameraId;
            float nearIn, farIn, angle;
            gePoint pos, target;

            cameraId = getStringFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_ID, XML_SECTION_CAMERA_ID_ERROR);
            nearIn = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_NEAR, XML_ATTRIBUTE_CAMERA_NEAR_ERROR);
            farIn = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_FAR, XML_ATTRIBUTE_CAMERA_FAR_ERROR);
            angle = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_ANGLE, XML_ATTRIBUTE_CAMERA_ANGLE_ERROR);
            pos = getPointFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_POSITION, XML_ATTRIBUTE_CAMERA_POSITION_ERROR);
            target = getPointFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_TARGET, XML_ATTRIBUTE_CAMERA_TARGET_ERROR);

            cameraVector.push_back(new geCameraPerspective(cameraId, nearIn, farIn, angle, pos, target));

            numberOfPerspectiveCameras++;
        }

        /* Camera is ortho */
        if (strcmp(xmlCameraElement->Value(), XML_SECTION_CAMERA_ORTHO) == 0) {
            string cameraId;
            float nearIn, farIn, left, right, top, bottom;

            cameraId = getStringFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_ID, XML_SECTION_CAMERA_ID_ERROR);
            nearIn = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_NEAR, XML_ATTRIBUTE_CAMERA_NEAR_ERROR);
            farIn = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_FAR, XML_ATTRIBUTE_CAMERA_FAR_ERROR);
            left = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_LEFT, XML_ATTRIBUTE_CAMERA_LEFT_ERROR);
            right = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_RIGHT, XML_ATTRIBUTE_CAMERA_RIGHT_ERROR);
            top = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_TOP, XML_ATTRIBUTE_CAMERA_TOP_ERROR);
            bottom = getFloatFromElementAttribute(xmlCameraElement, XML_ATTRIBUTE_CAMERA_BOTTOM, XML_ATTRIBUTE_CAMERA_BOTTOM_ERROR);

            cameraVector.push_back(new geCameraOrtho(cameraId, nearIn, farIn, left, right, top, bottom));
            numberOfOrthoCameras++;
        }

        xmlCameraElement = xmlCameraElement->NextSiblingElement();
    }

    /* Check if we got at least one camera */
    numberOfCameras = numberOfOrthoCameras + numberOfPerspectiveCameras;

    if (numberOfCameras < 1) {
        throw geException("XML: Cameras: No camera found.", true);
    }

    xmlCamerasLoaded = true;
}

/* Load and set lighting from XML */
void geScene::xmlLoadLighting() {
    if (xmlLightsLoaded) {
        throw geException("Lights have already been loaded!", true);
    }

    unsigned int numberOfOmniLights = 0, numberOfSpotLights = 0;
    this->numberOfLights = 0;

    string lightingDS = getStringFromElementAttribute(xmlLightingElement, XML_ATTRIBUTE_LIGHTING_DOUBLESIDED, XML_ATTRIBUTE_LIGHTING_DOUBLESIDED_ERROR);
    lightingDoubleSided = validateBoolean(lightingDS);

    string lightingL = getStringFromElementAttribute(xmlLightingElement, XML_ATTRIBUTE_LIGHTING_LOCAL, XML_ATTRIBUTE_LIGHTING_LOCAL_ERROR);
    lightingLocal = validateBoolean(lightingL);

    string lightingE = getStringFromElementAttribute(xmlLightingElement, XML_ATTRIBUTE_ENABLED, XML_ATTRIBUTE_LIGHTING_ENABLED_ERROR);
    lightingEnable = validateBoolean(lightingE);

    geColor ambientC = getColorFromElementAttribute(xmlLightingElement, XML_ATTRIBUTE_LIGHTING_AMBIENT, XML_ATTRIBUTE_LIGHTING_G_AMBIENT_ERROR);
    setAmbientLightColor(ambientC);

    /* Get lighting first child */
    TiXmlElement* xmlLightElement = xmlLightingElement->FirstChildElement();

    bool lightingLastRun = false;
    while (xmlLightElement != NULL && !lightingLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlLightElement == xmlLightingElement->LastChild()) {
            lightingLastRun = true;
        }

        /* Omni light */
        if (strcmp(xmlLightElement->Value(), XML_SECTION_LIGHTING_OMNI) == 0) {
            string lightId;
            bool lightEnable;
            gePoint location;
            geColor ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_ID, XML_SECTION_LIGHTING_ID_ERROR);

            string enabled = getStringFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_ENABLED, XML_SECTION_LIGHTING_ENABLED_ERROR);
            lightEnable = validateBoolean(enabled);

            location = getPointFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_LOCATION, XML_ATTRIBUTE_LIGHTING_LOCATION_ERROR);
            ambient = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_AMBIENT, XML_ATTRIBUTE_LIGHTING_AMBIENT_ERROR);
            diffuse = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_DIFFUSE, XML_ATTRIBUTE_LIGHTING_DIFFUSE_ERROR);
            specular = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_SPECULAR, XML_ATTRIBUTE_LIGHTING_SPECULAR_ERROR);

            lightVector.push_back(new geOmniLight(lightId, numberOfLights, lightEnable, location, ambient, diffuse, specular));
            numberOfOmniLights++;
        }

        /* Spot light */
        if (strcmp(xmlLightElement->Value(), XML_SECTION_LIGHTING_SPOT) == 0) {
            string lightId;
            bool lightEnable;
            float angle, exponent;
            gePoint location, direction;
            geColor ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_ID, XML_SECTION_LIGHTING_ID_ERROR);

            string enabled = getStringFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_ENABLED, XML_SECTION_LIGHTING_ENABLED_ERROR);
            lightEnable = validateBoolean(enabled);

            location = getPointFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_LOCATION, XML_ATTRIBUTE_LIGHTING_LOCATION_ERROR);
            ambient = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_AMBIENT, XML_ATTRIBUTE_LIGHTING_AMBIENT_ERROR);
            diffuse = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_DIFFUSE, XML_ATTRIBUTE_LIGHTING_DIFFUSE_ERROR);
            specular = getColorFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_SPECULAR, XML_ATTRIBUTE_LIGHTING_SPECULAR_ERROR);
            angle = getFloatFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_ANGLE, XML_SECTION_LIGHTING_ANGLE_ERROR);
            exponent = getFloatFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_EXPONENT, XML_ATTRIBUTE_LIGHTING_EXPONENT_ERROR);
            direction = getPointFromElementAttribute(xmlLightElement, XML_ATTRIBUTE_LIGHTING_DIRECTION, XML_ATTRIBUTE_LIGHTING_DIRECTION_ERROR);

            lightVector.push_back(new geSpotLight(lightId, numberOfLights, lightEnable, location, ambient, diffuse, specular, angle, exponent, direction));

            numberOfSpotLights++;
        }
        this->numberOfLights = numberOfOmniLights + numberOfSpotLights;
        xmlLightElement = xmlLightElement->NextSiblingElement();

        if (this->numberOfLights == MAX_LIGHTS) {
            cout << "XML: Lighting: Light limit reached, ignoring lights." << endl;
            break;
        }
    }

    if (numberOfLights < 1) {
        throw geException("XML: Lighting: No lights found!", true);
    }

    xmlLightsLoaded = true;
}

/* Load textures information from XML */
void geScene::xmlLoadTextures() {
    if (xmlTexturesLoaded) {
        throw geException("Textures have already been loaded!", true);
    }

    /* Get textures first child */
    TiXmlElement* xmlTextureElement = xmlTexturesElement->FirstChildElement();

    bool texturesLastRun = false;
    while (xmlTextureElement != NULL && !texturesLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlTextureElement == xmlTextureElement->LastChild()) {
            texturesLastRun = true;
        }

        string textureId, textureFileName;
        textureId = getStringFromElementAttribute(xmlTextureElement, XML_ATTRIBUTE_ID, XML_SECTION_TEXTURE_ID_ERROR);
        textureFileName = getStringFromElementAttribute(xmlTextureElement, XML_ATTRIBUTE_FILE, XML_SECTION_TEXTURE_FILE_ERROR);

        /* Create texture object here. */
        textureVector.push_back(new geTexture(textureId, textureFileName));

        xmlTextureElement = xmlTextureElement->NextSiblingElement();
    }

    xmlTexturesLoaded = true;
}

/* Load appearances from XML */
void geScene::xmlLoadAppearances() {
    if (xmlAppearancesLoaded) {
        throw geException("Appearances have already been loaded!", true);
    }

    int numberOfAppearances = 0;

    /* Get apperances first child */
    TiXmlElement* xmlAppearanceElement = xmlAppearancesElement->FirstChildElement();
    bool appearanceLastRun = false;
    while (xmlAppearanceElement != NULL && !appearanceLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlAppearanceElement == xmlAppearancesElement->LastChild()) {
            appearanceLastRun = true;
        }

        string appearanceId, textureId;
        float shininess, texlength_s, texlength_t;
        geColor emissive, ambient, diffuse, specular;
        bool hasTexture;

        appearanceId = getStringFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_ID, XML_SECTION_APPEARANCE_ID_ERROR);
        emissive = getColorFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_EMISSIVE, XML_ATTRIBUTE_APPEARANCE_EMISSIVE_ERROR);
        ambient = getColorFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_AMBIENT, XML_ATTRIBUTE_APPEARANCE_AMBIENT_ERROR);
        diffuse = getColorFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_DIFFUSE, XML_ATTRIBUTE_APPEARANCE_DIFFUSE_ERROR);
        specular = getColorFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_SPECULAR, XML_ATTRIBUTE_APPEARANCE_SPECULAR_ERROR);
        shininess = getFloatFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_SHININESS, XML_ATTRIBUTE_APPEARANCE_SHININESS_ERROR);
        hasTexture = getAttributeExistence(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_TEXTURE_ID);

        if (hasTexture) {
            textureId = getStringFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_TEXTURE_ID, XML_ATTRIBUTE_APPEARANCE_TEXTURE_ID_ERROR);
            texlength_s = getFloatFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S, XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S_ERROR);
            texlength_t = getFloatFromElementAttribute(xmlAppearanceElement, XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T, XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T_ERROR);

            geAppearance* temporaryAppearance = new geAppearance(appearanceId, emissive, ambient, diffuse, specular, shininess, textureId, texlength_s, texlength_t);
            appearanceVector.push_back(temporaryAppearance);
            temporaryAppearance = NULL;
        } else {
            geAppearance* temporaryAppearance = new geAppearance(appearanceId, emissive, ambient, diffuse, specular, shininess);
            appearanceVector.push_back(temporaryAppearance);
            temporaryAppearance = NULL;
        }

        numberOfAppearances++;
        xmlAppearanceElement = xmlAppearanceElement->NextSiblingElement();
    }

    if (numberOfAppearances < 1) {
        throw geException("No appearances found!", true);
    }

    xmlAppearancesLoaded = true;
}

/* Load the scene graph from XML */
void geScene::xmlLoadGraph() {
    if (xmlGraphLoaded) {
        throw geException("Graph has already been loaded!", true);
    }

    /* Root ID */
    string rootId;
    rootId = getStringFromElementAttribute(xmlGraphElement, XML_ATTRIBUTE_GRAPH_ROOTID, XML_ATTRIBUTE_GRAPH_ROOTID_ERROR);

    /* Create geGraph object */
    graph = new geGraph(rootId);

    /* Get graph first child (node) */
    int numberOfGraphNodes = 0;
    TiXmlElement* xmlNodeElement = xmlGraphElement->FirstChildElement();

    if (xmlGraphElement == NULL) {
        throw geException("Graph: Error while reading graph node.", true);
    }

    bool nodeLastRun = false;
    while (xmlNodeElement != NULL && !nodeLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlNodeElement == xmlGraphElement->LastChild()) {
            nodeLastRun = true;
        }

        string nodeId;

        /* Id */
        nodeId = getStringFromElementAttribute(xmlNodeElement, XML_ATTRIBUTE_ID, XML_SECTION_GRAPH_ID_ERROR);

        /* Display list */
        bool displayList;
        bool displayListAttribute = getAttributeExistence(xmlNodeElement, XML_ATTRIBUTE_NODE_DISPLAYLIST);

        if (displayListAttribute) {
            string displayListTemp;
            displayListTemp = getStringFromElementAttribute(xmlNodeElement, XML_ATTRIBUTE_NODE_DISPLAYLIST, XML_ATTRIBUTE_NODE_DISPLAYLIST_ERROR);
            displayList = validateBoolean(displayListTemp);
        } else {
            displayList = false;
        }

        geNode* temporaryNode;
        temporaryNode = new geNode(nodeId, displayList);

        /*************************************** Begin transforms ***************************************/
        /* Get node first child (transforms) */
        int numberOfTransforms = 0;
        TiXmlElement* xmlTransformsElement = xmlNodeElement->FirstChildElement(XML_BLOCK_TRANSFORMS);
        if (xmlTransformsElement == NULL) {
            throw geException("Node: No transform block found.", true, xmlNodeElement->Row());
        }

        TiXmlElement* xmlTransformElement = xmlTransformsElement->FirstChildElement();

        bool transformsLastRun = false;
        while (xmlTransformElement != NULL && !transformsLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlTransformElement == xmlNodeElement->LastChild()) {
                transformsLastRun = true;
            }

            if (strcmp(xmlTransformElement->Value(), XML_SECTION_TRANSFORM_SCALE) == 0) {
                /* Factor */
                gePoint scaleFactor;
                scaleFactor = getPointFromElementAttribute(xmlTransformElement, XML_ATTRIBUTE_TRANSFORM_FACTOR, XML_ATTRIBUTE_TRANSFORM_FACTOR_ERROR);

                temporaryNode->addTransform(new geTransformScale(scaleFactor, numberOfTransforms));
                numberOfTransforms++;
            }

            if (strcmp(xmlTransformElement->Value(), XML_SECTION_TRANSFORM_ROTATE) == 0) {
                string axis;
                int axisInt;
                float angle;

                /* Axis */
                axis = getStringFromElementAttribute(xmlTransformElement, XML_ATTRIBUTE_TRANSFORM_AXIS, XML_ATTRIBUTE_TRANSFORM_AXIS_ERROR);

                if (axis == "x") {
                    axisInt = 0;
                } else if (axis == "y") {
                    axisInt = 1;
                } else if (axis == "z") {
                    axisInt = 2;
                } else {
                    throw geException("Transform: Invalid string in transform axis.", true, xmlTransformElement->Row());
                }

                /* Angle */
                angle = getFloatFromElementAttribute(xmlTransformElement, XML_ATTRIBUTE_ANGLE, XML_ATTRIBUTE_ANGLE_ERROR);

                temporaryNode->addTransform(new geTransformRotate(axisInt, angle, numberOfTransforms));
                numberOfTransforms++;
            }

            /* Translate */
            if (strcmp(xmlTransformElement->Value(), XML_SECTION_TRANSFORM_TRANSLATE) == 0) {
                gePoint translate;
                translate = getPointFromElementAttribute(xmlTransformElement, XML_ATTRIBUTE_TRANSFORM_TO, XML_ATTRIBUTE_TRANSFORM_TO_ERROR);

                temporaryNode->addTransform(new geTransformTranslate(translate, numberOfTransforms));
                numberOfTransforms++;
            }

            xmlTransformElement = xmlTransformElement->NextSiblingElement();
        }

        /* End transforms */

        /*************************************** Begin appearanceref ***************************************/
        TiXmlElement* xmlAppearancerefElement = xmlNodeElement->FirstChildElement(XML_REFERENCE_APPEARANCE);
        if (xmlAppearancerefElement == NULL) {

        } else {
            string appearanceId;
            /* Id */
            appearanceId = getStringFromElementAttribute(xmlAppearancerefElement, XML_ATTRIBUTE_ID, XML_SECTION_GRAPH_APPEARANCE_ID_ERROR);

            temporaryNode->setAppearance(getAppearanceByString(appearanceId));
        }
        /* End appearanceref */

        /*************************************** Begin animationref ***************************************/
        TiXmlElement* xmlAnimationrefElement = xmlNodeElement->FirstChildElement(XML_REFERENCE_ANIMATION);
        if (xmlAnimationrefElement == NULL) {

        } else {
            /* Id */
            string animationId;
            animationId = getStringFromElementAttribute(xmlAnimationrefElement, XML_ATTRIBUTE_ID, XML_SECTION_GRAPH_ANIMATION_ID_ERROR);

            temporaryNode->setAnimation(getAnimationByString(animationId));
        }
        /* End animationref */

        /*************************************** Begin children ***************************************/
        /* Get node first child (children) */
        int numberOfChildren = 0;
        TiXmlElement* xmlChildrensElement = xmlNodeElement->FirstChildElement(XML_BLOCK_CHILDREN);
        if (xmlChildrensElement == NULL) {
            throw geException("Node: No children block found.", true, xmlNodeElement->Row());
        }

        TiXmlElement* xmlChildrenElement = xmlChildrensElement->FirstChildElement();

        bool childrensLastRun = false;
        while (xmlChildrenElement != NULL && !childrensLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlChildrenElement == xmlChildrensElement->LastChild()) {
                childrensLastRun = true;
            }

            /*Rectangle*/
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_RECTANGLE) == 0) {
                ge2dPoint pt1, pt2;

                /* XY1 */
                pt1 = get2DdPointFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_XY1, XML_ATTRIBUTE_PRIMITIVE_XY1_ERROR);

                /* XY2 */
                pt2 = get2DdPointFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_XY2, XML_ATTRIBUTE_PRIMITIVE_XY2_ERROR);

                /* Create rectangle and store pointer in graph */
                gePrimitive* primitiveTemp = new geRectangle(pt1, pt2);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /*Triangle*/
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_TRIANGLE) == 0) {
                gePoint point1, point2, point3;
                point1 = getPointFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_XYZ1, XML_ATTRIBUTE_PRIMITIVE_XYZ1_ERROR);
                point2 = getPointFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_XYZ2, XML_ATTRIBUTE_PRIMITIVE_XYZ2_ERROR);
                point3 = getPointFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_XYZ3, XML_ATTRIBUTE_PRIMITIVE_XYZ3_ERROR);

                /* Create triangle and store pointer in graph */
                gePrimitive* primitiveTemp = new geTriangle(point1, point2, point3);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /*Cylinder*/
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_CYLINDER) == 0) {
                float base, top, height;
                unsigned int slices, stacks;

                base = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_BASE, XML_ATTRIBUTE_PRIMITIVE_CYL_BASE_ERROR);
                top = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_TOP, XML_ATTRIBUTE_PRIMITIVE_CYL_TOP_ERROR);
                height = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_HEIGHT, XML_ATTRIBUTE_PRIMITIVE_CYL_HEIGHT_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_SLICES, XML_ATTRIBUTE_PRIMITIVE_CYL_SLICES_ERROR);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_STACKS, XML_ATTRIBUTE_PRIMITIVE_CYL_STACKS_ERROR);

                /* Create cylinder and store pointer in graph */
                gePrimitive* primitiveTemp = new geCylinder(base, top, height, slices, stacks);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /*Sphere*/
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_SPHERE) == 0) {
                float radius;
                unsigned int slices, stacks;

                radius = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_RADIUS, XML_ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_SLICES, XML_ATTRIBUTE_PRIMITIVE_SPHERE_SLICES_ERROR);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_STACKS, XML_ATTRIBUTE_PRIMITIVE_SPHERE_STACKS_ERROR);

                /* Create sphere and store pointer in graph */
                gePrimitive* primitiveTemp = new geSphere(radius, slices, stacks);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /*Torus*/
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_TORUS) == 0) {
                float inner, outer;
                unsigned int slices, loops;

                inner = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_INNER, XML_ATTRIBUTE_PRIMITIVE_TORUS_INNER_ERROR);
                outer = getFloatFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_OUTER, XML_ATTRIBUTE_PRIMITIVE_TORUS_OUTER_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_SLICES, XML_ATTRIBUTE_PRIMITIVE_TORUS_SLICES_ERROR);
                loops = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_LOOPS, XML_ATTRIBUTE_PRIMITIVE_TORUS_LOOPS_ERROR);

                /* Create torus and store pointer in graph */
                gePrimitive* primitiveTemp = new geTorus(inner, outer, slices, loops);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /* Plane */
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_PLANE) == 0) {
                unsigned int parts;

                parts = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_PARTS, XML_ATTRIBUTE_PRIMITIVE_PLANE_PARTS_ERROR);

                /* Create plane and store pointer in graph */
                gePrimitive* primitiveTemp = new gePlane(parts);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = NULL;

                numberOfChildren++;
            }

            /* Patch */
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_PATCH) == 0) {
                unsigned int order, partsU, partsV, compute;

                order = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_ORDER, XML_ATTRIBUTE_PRIMITIVE_PATCH_ORDER_ERROR);
                partsU = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_PARTSU, XML_ATTRIBUTE_PRIMITIVE_PATCH_PARTSU_ERROR);
                partsV = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_PARTSV, XML_ATTRIBUTE_PRIMITIVE_PATCH_PARTSV_ERROR);
                compute = getUnsignedIntFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_COMPUTE, XML_ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE_ERROR);

                gePatch* primitiveTempP = new gePatch(order, partsU, partsV, compute);

                /* Get number of points to be read */
                unsigned int numberOfPoints = primitiveTempP->getNumberOfPoints();

                TiXmlElement* xmlPatchPointElement = xmlChildrenElement->FirstChildElement();

                bool pointsLastRun = false;
                unsigned int readPoints = 0;

                while (xmlPatchPointElement != NULL && !pointsLastRun) {
                    /* Checks if we are loading the last child, if so this will be the last run. */
                    if (xmlPatchPointElement == xmlPatchPointElement->LastChild()) {
                        pointsLastRun = true;
                    }

                    /* Point */
                    if (strcmp(xmlPatchPointElement->Value(), XML_SECTION_PRIMITIVE_CONTROLPOINT) == 0) {
                        gePoint point;
                        point.x = getFloatFromElementAttribute(xmlPatchPointElement, XML_ATTRIBUTE_PRIMITIVE_X, XML_ATTRIBUTE_PRIMITIVE_PATCH_X_ERROR);
                        point.y = getFloatFromElementAttribute(xmlPatchPointElement, XML_ATTRIBUTE_PRIMITIVE_Y, XML_ATTRIBUTE_PRIMITIVE_PATCH_Y_ERROR);
                        point.z = getFloatFromElementAttribute(xmlPatchPointElement, XML_ATTRIBUTE_PRIMITIVE_Z, XML_ATTRIBUTE_PRIMITIVE_PATCH_Z_ERROR);

                        primitiveTempP->insertPoint(point);
                        readPoints++;

                        xmlPatchPointElement = xmlPatchPointElement->NextSiblingElement();
                    } else {
                        throw geException("Patch: Invalid child found.", true);
                    }
                }

                if (readPoints == numberOfPoints) {

                } else {
                    throw geException("Patch: Number of read points is invalid.", true);
                }

                /* Store gePatch pointer in graph */
                temporaryNode->addPrimitive(primitiveTempP);
                primitiveTempP = NULL;

                numberOfChildren++;
            }

            /* Vehicle */
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_VEHICLE) == 0) {

                /* Create vehicle */
                geVehicle* primitiveTempV = new geVehicle();

                /* Store the geVehicle pointer in the scene too, so that we can move it */
                sceneVehicles.push_back(primitiveTempV);

                /* Store geVehicle pointer in graph */
                temporaryNode->addPrimitive(primitiveTempV);
                primitiveTempV = NULL;

                numberOfChildren++;
            }

            /* Water line */
            if (strcmp(xmlChildrenElement->Value(), XML_SECTION_PRIMITIVE_WATERLINE) == 0) {
                string hmap, tmap, fshader, vshader;

                hmap = getStringFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_HEIGHTMAP, XML_ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP_ERROR);
                tmap = getStringFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_TEXTUREMAP, XML_ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP_ERROR);
                fshader = getStringFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_FRAGMENTSHADER, XML_ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER_ERROR);
                vshader = getStringFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_PRIMITIVE_VERTEXSHADER, XML_ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR);

                /* Create waterline */
                geWaterLine* primitiveTempWL = new geWaterLine(hmap, tmap, fshader, vshader);

                /* Store geVehicle pointer in graph */
                temporaryNode->addPrimitive(primitiveTempWL);
                waterLineVector.push_back(primitiveTempWL);

                primitiveTempWL = NULL;

                numberOfChildren++;
            }

            /* Noderef */
            if (strcmp(xmlChildrenElement->Value(), XML_REFERENCE_NODE) == 0) {
                string nodeReferenceId;
                nodeReferenceId = getStringFromElementAttribute(xmlChildrenElement, XML_ATTRIBUTE_ID, XML_SECTION_GRAPH_CHILDREN_ID_ERROR);

                /* Set reference id */
                temporaryNode->addChildrenID(nodeReferenceId);

                numberOfChildren++;
            }

            xmlChildrenElement = xmlChildrenElement->NextSiblingElement();
        }

        /* End children */

        unprocessedNodes.push_back(temporaryNode);

        numberOfGraphNodes++;
        xmlNodeElement = xmlNodeElement->NextSiblingElement();
    }

    if (numberOfGraphNodes < 1) {
        throw geException("Node: No nodes found.", true, xmlNodeElement->Row());
    }

    graph->importNodesPointerVector(unprocessedNodes);

    xmlGraphLoaded = true;
}

/* Load animations from XML */
void geScene::xmlLoadAnimations() {
    if (xmlAnimationsLoaded) {
        throw geException("Animations have already been loaded!", true);
    }

    int numberOfAnimations = 0;

    /* Get animations first child */
    TiXmlElement* xmlAnimationElement = xmlAnimationsElement->FirstChildElement();

    bool animationsLastRun = false;
    while (xmlAnimationElement != NULL && !animationsLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlAnimationElement == xmlAnimationElement->LastChild()) {
            animationsLastRun = true;
        }

        string animationId, animationType;
        float animationSpan;
        unsigned int animationTypeNumber;

        animationId = getStringFromElementAttribute(xmlAnimationElement, XML_ATTRIBUTE_ID, XML_ATTRIBUTE_ANIMATION_ID_ERROR);
        animationSpan = getFloatFromElementAttribute(xmlAnimationElement, XML_ATTRIBUTE_ANIMATION_SPAN, XML_ATTRIBUTE_ANIMATION_SPAN_ERROR);
        animationType = getStringFromElementAttribute(xmlAnimationElement, XML_ATTRIBUTE_ANIMATION_TYPE, XML_ATTRIBUTE_ANIMATION_TYPE_ERROR);

        /* Check the animation type string */
        if (animationType == XML_VALUE_ANIMATION_LINEAR) {
            animationTypeNumber = 1;
        } else {
            throw geException("Animations: Invalid animation type string.", true, xmlAnimationElement->Row());
        }

        /* Create animation object here. (So that we can add control points later) */
        geAnimation* temporaryAnimation;
        temporaryAnimation = new geAnimation(animationId, animationSpan, animationTypeNumber);

        animationsVector.push_back(temporaryAnimation);

        /* Control Points */
        TiXmlElement* xmlAnimationPointElement = xmlAnimationElement->FirstChildElement();

        bool pointsLastRun = false;
        unsigned int readPoints = 0;

        while (xmlAnimationPointElement != NULL && !pointsLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlAnimationPointElement == xmlAnimationPointElement->LastChild()) {
                pointsLastRun = true;
            }

            /* Point */
            if (strcmp(xmlAnimationPointElement->Value(), XML_SECTION_PRIMITIVE_CONTROLPOINT) == 0) {
                gePoint point;

                point.x = getFloatFromElementAttribute(xmlAnimationPointElement, XML_ATTRIBUTE_ANIMATION_X, XML_ATTRIBUTE_ANIMATION_X_ERROR);
                point.y = getFloatFromElementAttribute(xmlAnimationPointElement, XML_ATTRIBUTE_ANIMATION_Y, XML_ATTRIBUTE_ANIMATION_Y_ERROR);
                point.z = getFloatFromElementAttribute(xmlAnimationPointElement, XML_ATTRIBUTE_ANIMATION_Z, XML_ATTRIBUTE_ANIMATION_Z_ERROR);

                temporaryAnimation->insertPoint(point);

                readPoints++;

                xmlAnimationPointElement = xmlAnimationPointElement->NextSiblingElement();
            } else {
                throw geException("Animation: Invalid child found.", true);
            }
        }

        numberOfAnimations++;
        xmlAnimationElement = xmlAnimationElement->NextSiblingElement();
    }

    xmlAnimationsLoaded = true;
}

TiXmlElement *geScene::findChildByAttribute(TiXmlElement *parent, const char * attr, const char *val) {
    TiXmlElement *child = parent->FirstChildElement();
    bool found = false;

    while (child && !found) {
        if (child->Attribute(attr) && strcmp(child->Attribute(attr), val) == 0) {
            found = true;
        } else {
            child = child->NextSiblingElement();
        }
    }

    return child;
}

string geScene::getStringFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    string output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString) {
        output = string(valString);
    } else {
        throw geException(Error, true, iElement->Row());
    }
    return output;
}

gePoint geScene::getPointFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    gePoint output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%f %f %f", &output.x, &output.y, &output.z) == 3) {
        return output;
    } else {
        throw geException(Error, true, iElement->Row());
    }

    return output;
}

geColor geScene::getColorFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    geColor output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%f %f %f %f", &output.r, &output.g, &output.b, &output.a) == 4) {
        return output;
    } else {
        throw geException(Error, true, iElement->Row());
    }

    return output;
}

float geScene::getFloatFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    float output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%f", &output) == 1) {
        return output;
    } else {
        throw geException(Error, true, iElement->Row());
    }

    return output;
}

unsigned int geScene::getUnsignedIntFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    unsigned int output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%u", &output) == 1) {
        return output;
    } else {
        throw geException(Error, true, iElement->Row());
    }

    return output;
}

bool geScene::getAttributeExistence(TiXmlElement* iElement, char const* iAttribute) {
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString) {
        return true;
    } else {
        return false;
    }
}

ge2dPoint geScene::get2DdPointFromElementAttribute(TiXmlElement* iElement, char const* iAttribute, string Error) {
    ge2dPoint output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%f %f", &output.x, &output.y) == 2) {
        return output;
    } else {
        throw geException(Error, true, iElement->Row());
    }

    return output;
}

/* End XML functions */

/* Set stuff */
void geScene::setBackgroundColor(geColor in) {
    backgroundColour[0] = in.r;
    backgroundColour[1] = in.g;
    backgroundColour[2] = in.b;
    backgroundColour[3] = in.a;
}

void geScene::setAmbientLightColor(geColor in) {
    ambientLightColour[0] = in.r;
    ambientLightColour[1] = in.g;
    ambientLightColour[2] = in.b;
    ambientLightColour[3] = in.a;
}

/* Validate values & strings */
GLfloat geScene::validateOpenGLColour(GLfloat input) {
    if (input < -1.0) {
        throw geException("Out of range color value!", true);
        return -1.0;
    }

    if (input > 1.0) {
        throw geException("Out of range color value!", true);
        return 1.0;
    }

    return input;
}

bool geScene::validateBoolean(string input) {
    if (input == XML_ATTRIBUTE_VALUE_TRUE) {
        return true;
    }

    if (input == XML_ATTRIBUTE_VALUE_FALSE) {
        return false;
    }

    throw geException("Invalid boolean value!", true);
    return false;
}

void geScene::setDrawMode(string input) {
    if ((input == XML_ATTRIBUTE_VALUE_DRAWMODE_FILL) || (input == XML_ATTRIBUTE_VALUE_DRAWMODE_LINE) || (input == XML_ATTRIBUTE_VALUE_DRAWMODE_POINT)) {
        this->drawMode = input;

    } else {
        throw geException("Draw mode string not recognized!", true);
    }
}

void geScene::setShadingMode(string input) {
    if ((input == XML_ATTRIBUTE_VALUE_SHADING_FLAT) || (input == XML_ATTRIBUTE_VALUE_SHADING_GOURAUD)) {
        this->shadingMode = input;
    } else {
        throw geException("Shading mode string not recognized!", true);
    }
}

void geScene::setCullFace(string input) {
    if ((input == XML_ATTRIBUTE_VALUE_CULLFACE_NONE) || (input == XML_ATTRIBUTE_VALUE_CULLFACE_BACK) || (input == XML_ATTRIBUTE_VALUE_CULLFACE_FRONT) || (input == XML_ATTRIBUTE_VALUE_CULLFACE_BOTH)) {
        this->cullFace = input;
    } else {
        throw geException("Cull face string not recognized!", true);
    }
}

void geScene::setCullOrder(string input) {
    if ((input == XML_ATTRIBUTE_VALUE_CULLORDER_CCW) || (input == XML_ATTRIBUTE_VALUE_CULLORDER_CW)) {
        this->cullOrder = input;
    } else {
        throw geException("Cull order string not recognized!", true);
    }
}
/* End validate values & strings */

/* Apply globals */
void geScene::applyGlobals() {
    /* Background color [OK] */
    glClearColor(backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3]);

    /* Cull order */
    if (this->cullOrder == XML_ATTRIBUTE_VALUE_CULLORDER_CCW) {
        glFrontFace(GL_CCW);
    } else {
        glFrontFace(GL_CW);

    }

    /* Shading mode [OK] */
    if (this->shadingMode == XML_ATTRIBUTE_VALUE_SHADING_FLAT) {
        glShadeModel(GL_FLAT);

    } else {
        glShadeModel(GL_SMOOTH);

    }

    /* Cull face [OK] */
    if (this->cullFace == XML_ATTRIBUTE_VALUE_CULLFACE_NONE) {
        glCullFace(GL_NONE);
    }

    if (this->cullFace == XML_ATTRIBUTE_VALUE_CULLFACE_BACK) {
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    }

    if (this->cullFace == XML_ATTRIBUTE_VALUE_CULLFACE_FRONT) {
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Cull order [OK] */
    if (this->cullFace == XML_ATTRIBUTE_VALUE_CULLFACE_BOTH) {
        glCullFace(GL_BACK);
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Draw mode */
    if (this->drawMode == XML_ATTRIBUTE_VALUE_DRAWMODE_FILL) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        drawModeInt = 0;
    }

    if (this->drawMode == XML_ATTRIBUTE_VALUE_DRAWMODE_LINE) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        drawModeInt = 1;
    }

    if (this->drawMode == XML_ATTRIBUTE_VALUE_DRAWMODE_POINT) {
        glPolygonMode(GL_FRONT, GL_POINT);
        glPolygonMode(GL_BACK, GL_POINT);
        drawModeInt = 2;
    }

    /* Enable depth comparisons */
    glEnable(GL_DEPTH_TEST);
}

GLboolean geScene::getLightingEnableStatus() {
    return this->lightingEnable;
}

void geScene::init() {
#ifdef ENGINE_VERBOSE
    cout << "Setting up globals" << endl;
#endif
    applyGlobals();
#ifdef ENGINE_VERBOSE
    cout << "Setting up lights" << endl;
#endif
    initLights();

#ifdef ENGINE_VERBOSE
    cout << "Setting up textures" << endl;
#endif
    initTextures();

#ifdef ENGINE_VERBOSE
    cout << "Assigning textures to respective appearances" << endl;
#endif
    initAppearanceTextures();

}

void geScene::initLights() {
    /* General lighting parameters setup */
    /* Local is missing */
    if (lightingEnable) {
        glEnable(GL_LIGHTING);
    }

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->ambientLightColour);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, this->lightingDoubleSided);
    /* End general lighting */

#ifdef ENGINE_VERBOSE
    if (!this->lightVector.empty ()) {
        for (std::vector<geLight*>::iterator it = lightVector.begin (); it != this->lightVector.end (); it++) {
            if ((*it)->getLightEnableStatus ()) {
                cout << "Light enabled: [" << (*it)->getID () << "]" << endl;
            }
        }
    }
#endif
}

/* Initialize textures (throws a fatal geException if texture file doesn't exist) */
void geScene::initTextures() {
    for (std::vector<geTexture*>::iterator jt = textureVector.begin(); jt != this->textureVector.end(); jt++) {
        (*jt)->loadTexture();
    }
}

/* Assign textures to appearances */
void geScene::initAppearanceTextures() {
    if (!this->textureVector.empty() && this->xmlAppearancesLoaded && this->xmlTexturesLoaded) {
        for (std::vector<geAppearance*>::iterator it = appearanceVector.begin(); it != this->appearanceVector.end(); it++) {

            string apperanceRef = (*it)->getTextureReference();

            for (std::vector<geTexture*>::iterator jt = textureVector.begin(); jt != this->textureVector.end(); jt++) {
                if ((*jt)->getXmlId() == apperanceRef) {
                    (*it)->setTexture(*jt);

                    break;
                }
            }

        }
    }
}

/* Search initial camera and set current camera pointer to it */
void geScene::setInitialCamera() {
    if (cameraVector.empty()) {
        throw geException("Camera vector is empty!", true);
    }

    if (currentCameraPointer != NULL) {
        throw geException("Current camera pointer is not NULL!", true);
    }

    vector<geCamera*>::iterator cameraIt;

    cameraIt = cameraVector.begin();
    while (cameraIt != cameraVector.end()) {
        if ((*cameraIt)->getID() == this->initialCamera) {
            currentCameraPointer = *cameraIt;
            break;
        }
        cameraIt++;
    }

    if (currentCameraPointer == NULL) {
        throw geException("Initial camera not found!", true);
    }
}

/* Draw lights (each light pushes and pops a matrix) */
void geScene::displayLights() {
    if (!this->lightVector.empty()) {
        for (std::vector<geLight*>::iterator it = lightVector.begin(); it != this->lightVector.end(); it++) {
            if ((*it)->getLightEnableStatus()) {
                (*it)->draw();
            }
        }
    }
}

/* Apply current camera view */
void geScene::applyCameraView() {
    if (this->currentCameraPointer != NULL) {
        this->currentCameraPointer->applyView(this->aspectRatio);
    } else {
        throw geException("The current camera pointer is NULL.", true);
    }
}

void geScene::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowSizeX, windowSizeY);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    applyCameraView();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    graph->draw();

    displayLights();

    glutSwapBuffers();
}

void geScene::setViewRotate(GLfloat* input) {
    for (unsigned int i = 0; i < 16; i++) {
        customCameraMatrix[i] = input[i];
    }
}

GLfloat* geScene::getViewProjection() {
    return this->customCameraMatrix;
}

void geScene::update(unsigned long millis) {
    /* Call the animations update */
    if (!this->animationsVector.empty()) {
        for (vector<geAnimation*>::iterator it = this->animationsVector.begin(); it != this->animationsVector.end(); ++it) {
            (*it)->updateAnimation(millis);
        }
    }

    /* Call the shader update */
    if (!this->waterLineVector.empty()) {
        for (vector<geWaterLine*>::iterator it = this->waterLineVector.begin(); it != this->waterLineVector.end(); ++it) {
            (*it)->update(millis);
        }
    }
}

void geScene::activateCamera(unsigned int i) {
    if (i < this->cameraVector.size()) {
        currentCameraPointer = this->cameraVector.at(i);
        cout << ".";
        // CGFapplication::activeApp->forceRefresh();
    } else {
        throw geException("Invalid camera chosen.", true);
    }
}

void geScene::setCurrentWindowSize(int windowX, int windowY) {
    this->windowSizeX = windowX;
    this->windowSizeY = windowY;
    this->aspectRatio = (double) windowX / (double) windowY;
}

geAppearance* geScene::getAppearanceByString(string in) {
    if (this->xmlAppearancesLoaded) {

        if (!this->appearanceVector.empty()) {
            for (vector<geAppearance*>::iterator it = this->appearanceVector.begin(); it != this->appearanceVector.end(); ++it) {
                if ((*it)->getAppearanceID() == in) {
                    return *it;
                }
            }

            throw geException("XML: Reference to a non existing appearance [" + in + "].", true);

        } else {
            throw geException("Bug: appearances vector is empty!", true);
        }

    } else {
        throw geException("Bug: Attempting to find an apperance without appearances being loaded.", true);
        return NULL;
    }
}

geAnimation* geScene::getAnimationByString(string in) {
    if (this->xmlAnimationsLoaded) {

        if (!this->animationsVector.empty()) {
            for (vector<geAnimation*>::iterator it = this->animationsVector.begin(); it != this->animationsVector.end(); ++it) {
                if ((*it)->getID() == in) {
                    return *it;
                }
            }

            throw geException("XML: Reference to a non existing animation [" + in + "].", true);

        } else {
            throw geException("XML: Reference to an animation, but no animation exists!", true);
        }

    } else {
        throw geException("Bug: Attempting to find an animation without animations being loaded.", true);
        return NULL;
    }
}

void geScene::changeCameraToExernal() {
    currentCameraPointer = externalGuiCamera;
}

void geScene::changeCameraToSceneDefault() {
    vector<geCamera*>::iterator cameraIt;
    cameraIt = cameraVector.begin();
    while (cameraIt != cameraVector.end()) {
        if ((*cameraIt)->getID() == this->initialCamera) {
            currentCameraPointer = *cameraIt;
            break;
        }
        cameraIt++;
    }
}

unsigned int geScene::getNumberOfLights() {
    return this->numberOfLights;
}

int geScene::getPolygonMode() {
    if (drawModeInt >= 0 && drawModeInt <= 2) {
        return drawModeInt;
    }
    return 0;
}

void geScene::reSetPolygonMode(int in) {
    switch (in) {
    case 0:
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;

    case 1:
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        break;

    case 2:
        glPolygonMode(GL_FRONT, GL_POINT);
        glPolygonMode(GL_BACK, GL_POINT);
        break;

    default:
        break;
    }
}

bool geScene::getLightStatus(unsigned int number) {
    return lightVector[number]->getLightEnableStatus();
}

void geScene::disableLight(unsigned int number) {
    lightVector[number]->disable();
}

void geScene::enableLight(unsigned int number) {
    lightVector[number]->enable();
}

string geScene::getLightID(unsigned int number) {
    return this->lightVector[number]->getID();
}

unsigned int geScene::getNumberOfCameras() {
    return this->numberOfCameras;
}

void geScene::resetUserCamera() {
    externalGuiCamera->resetCamera();
}

string geScene::getCameraID(unsigned int number) {
    return this->cameraVector[number]->getID();
}

void geScene::setCamera(unsigned int number) {
    currentCameraPointer = cameraVector[number];
}

unsigned int geScene::getCurrentCamera() {
    for (unsigned int i = 0; i < cameraVector.size(); i++) {
        if (cameraVector[i] == currentCameraPointer) {
            return i;
        }
    }

    return 0;
}

void geScene::moveVehicleUp() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveUp();
        }
    }
}

void geScene::moveVehicleDown() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveDown();
        }
    }
}

void geScene::moveVehicleFront() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveFront();
        }
    }
}

void geScene::moveVehicleBack() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveBack();
        }
    }
}

void geScene::moveVehicleLeft() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveLeft();
        }
    }
}

void geScene::moveVehicleRight() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveRight();
        }
    }
}

void geScene::tiltVehicleLeft() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->tiltLeft();
        }
    }
}

void geScene::tiltVehicleRight() {
    if (!this->sceneVehicles.empty()) {
        for (vector<geVehicle*>::iterator i = sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->tiltRight();
        }
    }
}

geScene::~geScene() {
    if (graph != NULL) {
        delete (graph);
    }

    if (doc != NULL) {
        delete (doc);
    }
}
