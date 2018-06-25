/*
 * Eduardo Fernandes
 *
 * Scene class methods.
 */

#include <Scene.hpp>

namespace ge {
Scene::Scene(std::string& fileName) {
    currentCameraPointer = nullptr;

    parseAndLoadXml(fileName);

    /* Set current camera pointer to the initial camera defined in the XML */
    setInitialCamera();

    /* Create an camera that can be used to override the scene cameras */
    externalGuiCamera = new PerspectiveCamera();
}

void Scene::parseAndLoadXml(std::string& fileName) {
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
    if (xmlAnimationsElement != nullptr) {
        xmlLoadAnimations();
    }

    /* Graph is loaded last, since it will get references for the rest */
    xmlLoadGraph();

    if (doc != nullptr) {
        delete (doc);
    }

    delete[] xmlFileName;

    if (!(xmlGlobalsLoaded && xmlCamerasLoaded && xmlLightsLoaded
            && xmlTexturesLoaded && xmlAppearancesLoaded && xmlGraphLoaded)) {
        throw Exception("Something went wrong with the XML parsing.", true);
    }
}

void Scene::xmlLoadFile(char *filename) {
    doc = new TiXmlDocument(filename);
    bool loadOkay = doc->LoadFile();

    if (!loadOkay) {
        throw Exception("Error while loading scene xml file.");
    }
}

void Scene::xmlCheckDocumentType() {
    xmlRootElement = doc->FirstChildElement(Xml::DOCUMENT_TYPE);

    if (xmlRootElement == nullptr) {
        throw Exception("No correct root element found!", true);
    }
}

void Scene::xmlLoadMainElements() {
    xmlGlobalsElement = xmlRootElement->FirstChildElement(Xml::SECTION_GLOBALS);
    xmlCamerasElement = xmlRootElement->FirstChildElement(Xml::SECTION_CAMERAS);
    xmlLightingElement = xmlRootElement->FirstChildElement(Xml::SECTION_LIGHTS);
    xmlTexturesElement = xmlRootElement->FirstChildElement(
            Xml::SECTION_TEXTURES);
    xmlAppearancesElement = xmlRootElement->FirstChildElement(
            Xml::SECTION_APPEARANCES);
    xmlGraphElement = xmlRootElement->FirstChildElement(Xml::SECTION_GRAPH);
    xmlAnimationsElement = xmlRootElement->FirstChildElement(
            Xml::SECTION_ANIMATIONS);
}

void Scene::xmlCheckMainElements() {
    /* Check if all required nodes exist */

    if (xmlGlobalsElement == nullptr) {
        throw Exception("Globals element not found.");
    }

    if (xmlCamerasElement == nullptr) {
        throw Exception("Cameras element not found.");
    }

    if (xmlLightingElement == nullptr) {
        throw Exception("Lighting element not found.");
    }

    if (xmlTexturesElement == nullptr) {
        throw Exception("Textures element not found.");
    }

    if (xmlAppearancesElement == nullptr) {
        throw Exception("Appearances element not found.");
    }

    if (xmlGraphElement == nullptr) {
        throw Exception("Graph element not found.");
    }
}

/* Load and set globals from XML */
void Scene::xmlLoadGlobals() {
    if (xmlGlobalsLoaded) {
        throw Exception(Xml::SECTION_GLOBALS_ERROR, true);
    }

    color bgColor = getColorFromElementAttribute(xmlGlobalsElement,
            Xml::ATTRIBUTE_BACKGROUND, Xml::ATTRIBUTE_BACKGROUND_ERROR);
    setBackgroundColor(bgColor);

    std::string drawMode = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::ATTRIBUTE_DRAWMODE, Xml::ATTRIBUTE_DRAWMODE_ERROR);
    setDrawMode(drawMode);

    std::string shadingMode = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::ATTRIBUTE_SHADING, Xml::ATTRIBUTE_SHADING_ERROR);
    setShadingMode(shadingMode);

    std::string cullFace = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::ATTRIBUTE_CULLFACE, Xml::ATTRIBUTE_CULLFACE_ERROR);
    setCullFace(cullFace);

    std::string cullOrder = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::ATTRIBUTE_CULLORDER, Xml::ATTRIBUTE_CULLORDER_ERROR);
    setCullOrder(cullOrder);

    xmlGlobalsLoaded = true;
}

/* Load and set cameras from XML */
void Scene::xmlLoadCameras() {
    if (xmlCamerasLoaded) {
        throw Exception("XML: Cameras have already been loaded!", true);
    }

    int numberOfPerspectiveCameras = 0, numberOfOrthoCameras = 0;

    this->numberOfCameras = 0;
    this->initialCamera = getStringFromElementAttribute(xmlCamerasElement,
            Xml::ATTRIBUTE_INITIAL_CAMERA, Xml::ATTRIBUTE_INITIAL_CAMERA_ERROR);

    /* Get camera first child */
    TiXmlElement* xmlCameraElement = xmlCamerasElement->FirstChildElement();

    bool camerasLastRun = false;
    while (xmlCameraElement != nullptr && !camerasLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlCameraElement == xmlCamerasElement->LastChild()) {
            camerasLastRun = true;
        }

        /* Camera is perspective */
        if (strcmp(xmlCameraElement->Value(), Xml::SECTION_CAMERA_PERSPECTIVE)
                == 0) {
            std::string cameraId;
            float nearIn, farIn, angle;
            xyzPointDouble pos, target;

            cameraId = getStringFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_ID, Xml::SECTION_CAMERA_ID_ERROR);
            nearIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_NEAR,
                    Xml::ATTRIBUTE_CAMERA_NEAR_ERROR);
            farIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_FAR, Xml::ATTRIBUTE_CAMERA_FAR_ERROR);
            angle = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_ANGLE,
                    Xml::ATTRIBUTE_CAMERA_ANGLE_ERROR);
            pos = getDoublePointFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_POSITION,
                    Xml::ATTRIBUTE_CAMERA_POSITION_ERROR);
            target = getDoublePointFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_TARGET,
                    Xml::ATTRIBUTE_CAMERA_TARGET_ERROR);

            cameraVector.push_back(
                    new PerspectiveCamera(cameraId, nearIn, farIn, angle, pos,
                            target));

            numberOfPerspectiveCameras++;
        }

        /* Camera is ortho */
        if (strcmp(xmlCameraElement->Value(), Xml::SECTION_CAMERA_ORTHO) == 0) {
            std::string cameraId;
            float nearIn, farIn, left, right, top, bottom;

            cameraId = getStringFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_ID, Xml::SECTION_CAMERA_ID_ERROR);
            nearIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_NEAR,
                    Xml::ATTRIBUTE_CAMERA_NEAR_ERROR);
            farIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_FAR, Xml::ATTRIBUTE_CAMERA_FAR_ERROR);
            left = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_LEFT,
                    Xml::ATTRIBUTE_CAMERA_LEFT_ERROR);
            right = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_RIGHT,
                    Xml::ATTRIBUTE_CAMERA_RIGHT_ERROR);
            top = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_TOP, Xml::ATTRIBUTE_CAMERA_TOP_ERROR);
            bottom = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::ATTRIBUTE_CAMERA_BOTTOM,
                    Xml::ATTRIBUTE_CAMERA_BOTTOM_ERROR);

            cameraVector.push_back(
                    new OrthoCamera(cameraId, nearIn, farIn, left, right, top,
                            bottom));
            numberOfOrthoCameras++;
        }

        xmlCameraElement = xmlCameraElement->NextSiblingElement();
    }

    /* Check if we got at least one camera */
    numberOfCameras = numberOfOrthoCameras + numberOfPerspectiveCameras;

    if (numberOfCameras < 1) {
        throw Exception("XML: Cameras: No camera found.", true);
    }

    xmlCamerasLoaded = true;
}

/* Load and set lighting from XML */
void Scene::xmlLoadLighting() {
    if (xmlLightsLoaded) {
        throw Exception("Lights have already been loaded!", true);
    }

    unsigned int numberOfOmniLights = 0, numberOfSpotLights = 0;
    this->numberOfLights = 0;

    std::string lightingDS = getStringFromElementAttribute(xmlLightingElement,
            Xml::ATTRIBUTE_LIGHTING_DOUBLESIDED,
            Xml::ATTRIBUTE_LIGHTING_DOUBLESIDED_ERROR);
    lightingDoubleSided = validateBoolean(lightingDS);

    std::string lightingL = getStringFromElementAttribute(xmlLightingElement,
            Xml::ATTRIBUTE_LIGHTING_LOCAL, Xml::ATTRIBUTE_LIGHTING_LOCAL_ERROR);
    lightingLocal = validateBoolean(lightingL);

    std::string lightingE = getStringFromElementAttribute(xmlLightingElement,
            Xml::ATTRIBUTE_ENABLED, Xml::ATTRIBUTE_LIGHTING_ENABLED_ERROR);
    lightingEnable = validateBoolean(lightingE);

    color ambientC = getColorFromElementAttribute(xmlLightingElement,
            Xml::ATTRIBUTE_LIGHTING_AMBIENT,
            Xml::ATTRIBUTE_LIGHTING_G_AMBIENT_ERROR);
    setAmbientLightColor(ambientC);

    /* Get lighting first child */
    TiXmlElement* xmlLightElement = xmlLightingElement->FirstChildElement();

    bool lightingLastRun = false;
    while (xmlLightElement != nullptr && !lightingLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlLightElement == xmlLightingElement->LastChild()) {
            lightingLastRun = true;
        }

        /* Omni light */
        if (strcmp(xmlLightElement->Value(), Xml::SECTION_LIGHTING_OMNI) == 0) {
            std::string lightId;
            bool lightEnable;
            xyzPointFloat location;
            color ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_ID, Xml::SECTION_LIGHTING_ID_ERROR);

            std::string enabled = getStringFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_ENABLED,
                    Xml::SECTION_LIGHTING_ENABLED_ERROR);
            lightEnable = validateBoolean(enabled);

            location = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_LOCATION,
                    Xml::ATTRIBUTE_LIGHTING_LOCATION_ERROR);
            ambient = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_AMBIENT,
                    Xml::ATTRIBUTE_LIGHTING_AMBIENT_ERROR);
            diffuse = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_DIFFUSE,
                    Xml::ATTRIBUTE_LIGHTING_DIFFUSE_ERROR);
            specular = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_SPECULAR,
                    Xml::ATTRIBUTE_LIGHTING_SPECULAR_ERROR);

            lightVector.push_back(
                    new OmniLight(lightId, numberOfLights, lightEnable,
                            location, ambient, diffuse, specular));
            numberOfOmniLights++;
        }

        /* Spot light */
        if (strcmp(xmlLightElement->Value(), Xml::SECTION_LIGHTING_SPOT) == 0) {
            std::string lightId;
            bool lightEnable;
            GLfloat angle, exponent;
            xyzPointFloat location;
            xyzPointFloat direction;
            color ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_ID, Xml::SECTION_LIGHTING_ID_ERROR);

            std::string enabled = getStringFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_ENABLED,
                    Xml::SECTION_LIGHTING_ENABLED_ERROR);
            lightEnable = validateBoolean(enabled);

            location = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_LOCATION,
                    Xml::ATTRIBUTE_LIGHTING_LOCATION_ERROR);
            ambient = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_AMBIENT,
                    Xml::ATTRIBUTE_LIGHTING_AMBIENT_ERROR);
            diffuse = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_DIFFUSE,
                    Xml::ATTRIBUTE_LIGHTING_DIFFUSE_ERROR);
            specular = getColorFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_SPECULAR,
                    Xml::ATTRIBUTE_LIGHTING_SPECULAR_ERROR);
            angle = getFloatFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_ANGLE, Xml::SECTION_LIGHTING_ANGLE_ERROR);
            exponent = getFloatFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_EXPONENT,
                    Xml::ATTRIBUTE_LIGHTING_EXPONENT_ERROR);
            direction = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::ATTRIBUTE_LIGHTING_DIRECTION,
                    Xml::ATTRIBUTE_LIGHTING_DIRECTION_ERROR);

            lightVector.push_back(
                    new SpotLight(lightId, numberOfLights, lightEnable,
                            location, ambient, diffuse, specular, angle,
                            exponent, direction));

            numberOfSpotLights++;
        }
        this->numberOfLights = numberOfOmniLights + numberOfSpotLights;
        xmlLightElement = xmlLightElement->NextSiblingElement();

        if (this->numberOfLights == MAX_LIGHTS) {
            std::cout << "XML: Lighting: Light limit reached, ignoring lights."
                    << std::endl;
            break;
        }
    }

    if (numberOfLights < 1) {
        throw Exception("XML: Lighting: No lights found!", true);
    }

    xmlLightsLoaded = true;
}

/* Load textures information from XML */
void Scene::xmlLoadTextures() {
    if (xmlTexturesLoaded) {
        throw Exception("Textures have already been loaded!", true);
    }

    /* Get textures first child */
    TiXmlElement* xmlTextureElement = xmlTexturesElement->FirstChildElement();

    bool texturesLastRun = false;
    while (xmlTextureElement != nullptr && !texturesLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlTextureElement == xmlTextureElement->LastChild()) {
            texturesLastRun = true;
        }

        std::string textureId, textureFileName;
        textureId = getStringFromElementAttribute(xmlTextureElement,
                Xml::ATTRIBUTE_ID, Xml::SECTION_TEXTURE_ID_ERROR);
        textureFileName = getStringFromElementAttribute(xmlTextureElement,
                Xml::ATTRIBUTE_FILE, Xml::SECTION_TEXTURE_FILE_ERROR);

        /* Create texture object here. */
        textureVector.push_back(new Texture(textureId, textureFileName));

        xmlTextureElement = xmlTextureElement->NextSiblingElement();
    }

    xmlTexturesLoaded = true;
}

/* Load appearances from XML */
void Scene::xmlLoadAppearances() {
    if (xmlAppearancesLoaded) {
        throw Exception("Appearances have already been loaded!", true);
    }

    int numberOfAppearances = 0;

    /* Get apperances first child */
    TiXmlElement* xmlAppearanceElement =
            xmlAppearancesElement->FirstChildElement();
    bool appearanceLastRun = false;
    while (xmlAppearanceElement != nullptr && !appearanceLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlAppearanceElement == xmlAppearancesElement->LastChild()) {
            appearanceLastRun = true;
        }

        std::string appearanceId, textureId;
        float shininess, texlength_s, texlength_t;
        color emissive, ambient, diffuse, specular;
        bool hasTexture;

        appearanceId = getStringFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_ID, Xml::SECTION_APPEARANCE_ID_ERROR);
        emissive = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_EMISSIVE,
                Xml::ATTRIBUTE_APPEARANCE_EMISSIVE_ERROR);
        ambient = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_AMBIENT,
                Xml::ATTRIBUTE_APPEARANCE_AMBIENT_ERROR);
        diffuse = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_DIFFUSE,
                Xml::ATTRIBUTE_APPEARANCE_DIFFUSE_ERROR);
        specular = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_SPECULAR,
                Xml::ATTRIBUTE_APPEARANCE_SPECULAR_ERROR);
        shininess = getFloatFromElementAttribute(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_SHININESS,
                Xml::ATTRIBUTE_APPEARANCE_SHININESS_ERROR);
        hasTexture = getAttributeExistence(xmlAppearanceElement,
                Xml::ATTRIBUTE_APPEARANCE_TEXTURE_ID);

        if (hasTexture) {
            textureId = getStringFromElementAttribute(xmlAppearanceElement,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_ID,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_ID_ERROR);
            texlength_s = getFloatFromElementAttribute(xmlAppearanceElement,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S_ERROR);
            texlength_t = getFloatFromElementAttribute(xmlAppearanceElement,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T,
                    Xml::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T_ERROR);

            Appearance* temporaryAppearance = new Appearance(appearanceId,
                    emissive, ambient, diffuse, specular, shininess, textureId,
                    texlength_s, texlength_t);
            appearanceVector.push_back(temporaryAppearance);
            temporaryAppearance = nullptr;
        } else {
            Appearance* temporaryAppearance = new Appearance(appearanceId,
                    emissive, ambient, diffuse, specular, shininess);
            appearanceVector.push_back(temporaryAppearance);
            temporaryAppearance = nullptr;
        }

        numberOfAppearances++;
        xmlAppearanceElement = xmlAppearanceElement->NextSiblingElement();
    }

    if (numberOfAppearances < 1) {
        throw Exception("No appearances found!", true);
    }

    xmlAppearancesLoaded = true;
}

/* Load the scene graph from XML */
void Scene::xmlLoadGraph() {
    if (xmlGraphLoaded) {
        throw Exception("Graph has already been loaded!", true);
    }

    /* Root ID */
    std::string rootId;
    rootId = getStringFromElementAttribute(xmlGraphElement,
            Xml::ATTRIBUTE_GRAPH_ROOTID, Xml::ATTRIBUTE_GRAPH_ROOTID_ERROR);

    /* Create geGraph object */
    graph = new SceneGraph(rootId);

    /* Get graph first child (node) */
    int numberOfGraphNodes = 0;
    TiXmlElement* xmlNodeElement = xmlGraphElement->FirstChildElement();

    if (xmlGraphElement == nullptr) {
        throw Exception("Graph: Error while reading graph node.", true);
    }

    bool nodeLastRun = false;
    while (xmlNodeElement != nullptr && !nodeLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlNodeElement == xmlGraphElement->LastChild()) {
            nodeLastRun = true;
        }

        std::string nodeId;

        /* Id */
        nodeId = getStringFromElementAttribute(xmlNodeElement,
                Xml::ATTRIBUTE_ID, Xml::SECTION_GRAPH_ID_ERROR);

        /* Display list */
        bool displayList;
        bool displayListAttribute = getAttributeExistence(xmlNodeElement,
                Xml::ATTRIBUTE_NODE_DISPLAYLIST);

        if (displayListAttribute) {
            std::string displayListTemp;
            displayListTemp = getStringFromElementAttribute(xmlNodeElement,
                    Xml::ATTRIBUTE_NODE_DISPLAYLIST,
                    Xml::ATTRIBUTE_NODE_DISPLAYLIST_ERROR);
            displayList = validateBoolean(displayListTemp);
        } else {
            displayList = false;
        }

        Node* temporaryNode;
        temporaryNode = new Node(nodeId, displayList);

        /*************************************** Begin transforms ***************************************/
        /* Get node first child (transforms) */
        int numberOfTransforms = 0;
        TiXmlElement* xmlTransformsElement = xmlNodeElement->FirstChildElement(
                Xml::BLOCK_TRANSFORMS);
        if (xmlTransformsElement == nullptr) {
            throw Exception("Node: No transform block found.", true,
                    xmlNodeElement->Row());
        }

        TiXmlElement* xmlTransformElement =
                xmlTransformsElement->FirstChildElement();

        bool transformsLastRun = false;
        while (xmlTransformElement != nullptr && !transformsLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlTransformElement == xmlNodeElement->LastChild()) {
                transformsLastRun = true;
            }

            if (strcmp(xmlTransformElement->Value(),
                    Xml::SECTION_TRANSFORM_SCALE) == 0) {
                /* Factor */
                xyzPointDouble scaleFactor;
                scaleFactor = getDoublePointFromElementAttribute(
                        xmlTransformElement, Xml::ATTRIBUTE_TRANSFORM_FACTOR,
                        Xml::ATTRIBUTE_TRANSFORM_FACTOR_ERROR);

                temporaryNode->addTransform(
                        new TransformScale(scaleFactor, numberOfTransforms));
                numberOfTransforms++;
            }

            if (strcmp(xmlTransformElement->Value(),
                    Xml::SECTION_TRANSFORM_ROTATE) == 0) {
                std::string axis;
                int axisInt;
                float angle;

                /* Axis */
                axis = getStringFromElementAttribute(xmlTransformElement,
                        Xml::ATTRIBUTE_TRANSFORM_AXIS,
                        Xml::ATTRIBUTE_TRANSFORM_AXIS_ERROR);

                if (axis == "x") {
                    axisInt = 0;
                } else if (axis == "y") {
                    axisInt = 1;
                } else if (axis == "z") {
                    axisInt = 2;
                } else {
                    throw Exception(
                            "Transform: Invalid string in transform axis.",
                            true, xmlTransformElement->Row());
                }

                /* Angle */
                angle = getFloatFromElementAttribute(xmlTransformElement,
                        Xml::ATTRIBUTE_ANGLE, Xml::ATTRIBUTE_ANGLE_ERROR);

                temporaryNode->addTransform(
                        new TransformRotate(axisInt, angle,
                                numberOfTransforms));
                numberOfTransforms++;
            }

            /* Translate */
            if (strcmp(xmlTransformElement->Value(),
                    Xml::SECTION_TRANSFORM_TRANSLATE) == 0) {
                xyzPointDouble translate;
                translate = getDoublePointFromElementAttribute(
                        xmlTransformElement, Xml::ATTRIBUTE_TRANSFORM_TO,
                        Xml::ATTRIBUTE_TRANSFORM_TO_ERROR);

                temporaryNode->addTransform(
                        new TransformTranslate(translate,
                                numberOfTransforms));
                numberOfTransforms++;
            }

            xmlTransformElement = xmlTransformElement->NextSiblingElement();
        }

        /* End transforms */

        /*************************************** Begin appearanceref ***************************************/
        TiXmlElement* xmlAppearancerefElement =
                xmlNodeElement->FirstChildElement(Xml::REFERENCE_APPEARANCE);
        if (xmlAppearancerefElement == nullptr) {

        } else {
            std::string appearanceId;
            /* Id */
            appearanceId = getStringFromElementAttribute(
                    xmlAppearancerefElement, Xml::ATTRIBUTE_ID,
                    Xml::SECTION_GRAPH_APPEARANCE_ID_ERROR);

            temporaryNode->setAppearance(getAppearanceByString(appearanceId));
        }
        /* End appearanceref */

        /*************************************** Begin animationref ***************************************/
        TiXmlElement* xmlAnimationrefElement =
                xmlNodeElement->FirstChildElement(Xml::REFERENCE_ANIMATION);
        if (xmlAnimationrefElement == nullptr) {

        } else {
            /* Id */
            std::string animationId;
            animationId = getStringFromElementAttribute(xmlAnimationrefElement,
                    Xml::ATTRIBUTE_ID, Xml::SECTION_GRAPH_ANIMATION_ID_ERROR);

            temporaryNode->setAnimation(getAnimationByString(animationId));
        }
        /* End animationref */

        /*************************************** Begin children ***************************************/
        /* Get node first child (children) */
        int numberOfChildren = 0;
        TiXmlElement* xmlChildrensElement = xmlNodeElement->FirstChildElement(
                Xml::BLOCK_CHILDREN);
        if (xmlChildrensElement == nullptr) {
            throw Exception("Node: No children block found.", true,
                    xmlNodeElement->Row());
        }

        TiXmlElement* xmlChildrenElement =
                xmlChildrensElement->FirstChildElement();

        bool childrensLastRun = false;
        while (xmlChildrenElement != nullptr && !childrensLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlChildrenElement == xmlChildrensElement->LastChild()) {
                childrensLastRun = true;
            }

            /*Rectangle*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_RECTANGLE) == 0) {
                xyPointDouble pt1, pt2;

                /* XY1 */
                pt1 = get2DdPointFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_XY1,
                        Xml::ATTRIBUTE_PRIMITIVE_XY1_ERROR);

                /* XY2 */
                pt2 = get2DdPointFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_XY2,
                        Xml::ATTRIBUTE_PRIMITIVE_XY2_ERROR);

                /* Create rectangle and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Rectangle(pt1, pt2);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Triangle*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_TRIANGLE) == 0) {
                xyzPointDouble point1, point2, point3;
                point1 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ1,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ1_ERROR);
                point2 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ2,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ2_ERROR);
                point3 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ3,
                        Xml::ATTRIBUTE_PRIMITIVE_XYZ3_ERROR);

                /* Create triangle and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Triangle(point1, point2, point3);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Cylinder*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_CYLINDER) == 0) {
                float base, top, height;
                unsigned int slices, stacks;

                base = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_BASE,
                        Xml::ATTRIBUTE_PRIMITIVE_CYL_BASE_ERROR);
                top = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_TOP,
                        Xml::ATTRIBUTE_PRIMITIVE_CYL_TOP_ERROR);
                height = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_HEIGHT,
                        Xml::ATTRIBUTE_PRIMITIVE_CYL_HEIGHT_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_SLICES,
                        Xml::ATTRIBUTE_PRIMITIVE_CYL_SLICES_ERROR);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_STACKS,
                        Xml::ATTRIBUTE_PRIMITIVE_CYL_STACKS_ERROR);

                /* Create cylinder and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Cylinder(base, top, height, slices,
                                stacks);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Sphere*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_SPHERE) == 0) {
                float radius;
                unsigned int slices, stacks;

                radius = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_RADIUS,
                        Xml::ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_SLICES,
                        Xml::ATTRIBUTE_PRIMITIVE_SPHERE_SLICES_ERROR);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_STACKS,
                        Xml::ATTRIBUTE_PRIMITIVE_SPHERE_STACKS_ERROR);

                /* Create sphere and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Sphere(radius, slices, stacks);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Torus*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_TORUS) == 0) {
                float inner, outer;
                unsigned int slices, loops;

                inner = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_INNER,
                        Xml::ATTRIBUTE_PRIMITIVE_TORUS_INNER_ERROR);
                outer = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_OUTER,
                        Xml::ATTRIBUTE_PRIMITIVE_TORUS_OUTER_ERROR);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_SLICES,
                        Xml::ATTRIBUTE_PRIMITIVE_TORUS_SLICES_ERROR);
                loops = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_LOOPS,
                        Xml::ATTRIBUTE_PRIMITIVE_TORUS_LOOPS_ERROR);

                /* Create torus and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Torus(inner, outer, slices, loops);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /* Plane */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_PLANE) == 0) {
                unsigned int parts;

                parts = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_PARTS,
                        Xml::ATTRIBUTE_PRIMITIVE_PLANE_PARTS_ERROR);

                /* Create plane and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Plane(parts);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /* Patch */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_PATCH) == 0) {
                unsigned int order, partsU, partsV, compute;

                order = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_ORDER,
                        Xml::ATTRIBUTE_PRIMITIVE_PATCH_ORDER_ERROR);
                partsU = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_PARTSU,
                        Xml::ATTRIBUTE_PRIMITIVE_PATCH_PARTSU_ERROR);
                partsV = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_PARTSV,
                        Xml::ATTRIBUTE_PRIMITIVE_PATCH_PARTSV_ERROR);
                compute = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_COMPUTE,
                        Xml::ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE_ERROR);

                Primitives::Patch* primitiveTempP = new Primitives::Patch(order,
                        partsU, partsV, compute);

                /* Get number of points to be read */
                unsigned int numberOfPoints =
                        primitiveTempP->getNumberOfPoints();

                TiXmlElement* xmlPatchPointElement =
                        xmlChildrenElement->FirstChildElement();

                bool pointsLastRun = false;
                unsigned int readPoints = 0;

                while (xmlPatchPointElement != nullptr && !pointsLastRun) {
                    /* Checks if we are loading the last child, if so this will be the last run. */
                    if (xmlPatchPointElement
                            == xmlPatchPointElement->LastChild()) {
                        pointsLastRun = true;
                    }

                    /* Point */
                    if (strcmp(xmlPatchPointElement->Value(),
                            Xml::SECTION_PRIMITIVE_CONTROLPOINT) == 0) {
                        xyzPointDouble point;
                        point.x = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::ATTRIBUTE_PRIMITIVE_X,
                                Xml::ATTRIBUTE_PRIMITIVE_PATCH_X_ERROR);
                        point.y = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::ATTRIBUTE_PRIMITIVE_Y,
                                Xml::ATTRIBUTE_PRIMITIVE_PATCH_Y_ERROR);
                        point.z = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::ATTRIBUTE_PRIMITIVE_Z,
                                Xml::ATTRIBUTE_PRIMITIVE_PATCH_Z_ERROR);

                        primitiveTempP->insertPoint(point);
                        readPoints++;

                        xmlPatchPointElement =
                                xmlPatchPointElement->NextSiblingElement();
                    } else {
                        throw Exception("Patch: Invalid child found.", true);
                    }
                }

                if (readPoints == numberOfPoints) {

                } else {
                    throw Exception(
                            "Patch: Number of read points is invalid.", true);
                }

                /* Store gePatch pointer in graph */
                temporaryNode->addPrimitive(primitiveTempP);
                primitiveTempP = nullptr;

                numberOfChildren++;
            }

            /* Vehicle */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_VEHICLE) == 0) {

                /* Create vehicle */
                Primitives::Vehicle* primitiveTempV = new Primitives::Vehicle();

                /* Store the geVehicle pointer in the scene too, so that we can move it */
                sceneVehicles.push_back(primitiveTempV);

                /* Store geVehicle pointer in graph */
                temporaryNode->addPrimitive(primitiveTempV);
                primitiveTempV = nullptr;

                numberOfChildren++;
            }

            /* Water line */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::SECTION_PRIMITIVE_WATERLINE) == 0) {
                std::string hmap, tmap, fshader, vshader;

                hmap = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_HEIGHTMAP,
                        Xml::ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP_ERROR);
                tmap = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_TEXTUREMAP,
                        Xml::ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP_ERROR);
                fshader = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_FRAGMENTSHADER,
                        Xml::ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER_ERROR);
                vshader = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::ATTRIBUTE_PRIMITIVE_VERTEXSHADER,
                        Xml::ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR);

                /* Create waterline */
                Primitives::WaterLine* primitiveTempWL =
                        new Primitives::WaterLine(hmap, tmap, fshader, vshader);

                /* Store geVehicle pointer in graph */
                temporaryNode->addPrimitive(primitiveTempWL);
                waterLineVector.push_back(primitiveTempWL);

                primitiveTempWL = nullptr;

                numberOfChildren++;
            }

            /* Noderef */
            if (strcmp(xmlChildrenElement->Value(), Xml::REFERENCE_NODE) == 0) {
                std::string nodeReferenceId;
                nodeReferenceId = getStringFromElementAttribute(
                        xmlChildrenElement, Xml::ATTRIBUTE_ID,
                        Xml::SECTION_GRAPH_CHILDREN_ID_ERROR);

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
        throw Exception("Node: No nodes found.", true, xmlNodeElement->Row());
    }

    graph->importNodesPointerVector(unprocessedNodes);

    xmlGraphLoaded = true;
}

/* Load animations from XML */
void Scene::xmlLoadAnimations() {
    if (xmlAnimationsLoaded) {
        throw Exception("Animations have already been loaded!", true);
    }

    int numberOfAnimations = 0;

    /* Get animations first child */
    TiXmlElement* xmlAnimationElement =
            xmlAnimationsElement->FirstChildElement();

    bool animationsLastRun = false;
    while (xmlAnimationElement != nullptr && !animationsLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlAnimationElement == xmlAnimationElement->LastChild()) {
            animationsLastRun = true;
        }

        std::string animationId, animationType;
        float animationSpan;
        unsigned int animationTypeNumber;

        animationId = getStringFromElementAttribute(xmlAnimationElement,
                Xml::ATTRIBUTE_ID, Xml::ATTRIBUTE_ANIMATION_ID_ERROR);
        animationSpan = getFloatFromElementAttribute(xmlAnimationElement,
                Xml::ATTRIBUTE_ANIMATION_SPAN,
                Xml::ATTRIBUTE_ANIMATION_SPAN_ERROR);
        animationType = getStringFromElementAttribute(xmlAnimationElement,
                Xml::ATTRIBUTE_ANIMATION_TYPE,
                Xml::ATTRIBUTE_ANIMATION_TYPE_ERROR);

        /* Check the animation type string */
        if (animationType == Xml::VALUE_ANIMATION_LINEAR) {
            animationTypeNumber = 1;
        } else {
            throw Exception("Animations: Invalid animation type string.",
                    true, xmlAnimationElement->Row());
        }

        /* Create animation object here. (So that we can add control points later) */
        Animation* temporaryAnimation;
        temporaryAnimation = new Animation(animationId, animationSpan,
                animationTypeNumber);

        animationsVector.push_back(temporaryAnimation);

        /* Control Points */
        TiXmlElement* xmlAnimationPointElement =
                xmlAnimationElement->FirstChildElement();

        bool pointsLastRun = false;
        unsigned int readPoints = 0;

        while (xmlAnimationPointElement != nullptr && !pointsLastRun) {
            /* Checks if we are loading the last child, if so this will be the last run. */
            if (xmlAnimationPointElement
                    == xmlAnimationPointElement->LastChild()) {
                pointsLastRun = true;
            }

            /* Point */
            if (strcmp(xmlAnimationPointElement->Value(),
                    Xml::SECTION_PRIMITIVE_CONTROLPOINT) == 0) {
                xyzPointDouble point;

                point.x = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::ATTRIBUTE_ANIMATION_X,
                        Xml::ATTRIBUTE_ANIMATION_X_ERROR);
                point.y = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::ATTRIBUTE_ANIMATION_Y,
                        Xml::ATTRIBUTE_ANIMATION_Y_ERROR);
                point.z = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::ATTRIBUTE_ANIMATION_Z,
                        Xml::ATTRIBUTE_ANIMATION_Z_ERROR);

                temporaryAnimation->insertPoint(point);

                readPoints++;

                xmlAnimationPointElement =
                        xmlAnimationPointElement->NextSiblingElement();
            } else {
                throw Exception("Animation: Invalid child found.", true);
            }
        }

        numberOfAnimations++;
        xmlAnimationElement = xmlAnimationElement->NextSiblingElement();
    }

    xmlAnimationsLoaded = true;
}

TiXmlElement *Scene::findChildByAttribute(TiXmlElement *parent,
        const char * attr, const char *val) {
    TiXmlElement *child = parent->FirstChildElement();
    bool found = false;

    while (child && !found) {
        if (child->Attribute(attr)
                && strcmp(child->Attribute(attr), val) == 0) {
            found = true;
        } else {
            child = child->NextSiblingElement();
        }
    }

    return child;
}

std::string Scene::getStringFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    std::string output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString) {
        return std::string(valString);
    }

    throw Exception(Error, true, iElement->Row());
}

xyzPointDouble Scene::getDoublePointFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    xyzPointDouble output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString
            && sscanf(valString, "%lf %lf %lf", &output.x, &output.y, &output.z)
                    == 3) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

xyzPointFloat Scene::getFloatPointFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    xyzPointFloat output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString
            && sscanf(valString, "%f %f %f", &output.x, &output.y, &output.z)
                    == 3) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

color Scene::getColorFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    color output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString
            && sscanf(valString, "%f %f %f %f", &output.r, &output.g, &output.b,
                    &output.a) == 4) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

double Scene::getDoubleFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    double output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%lf", &output) == 1) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

float Scene::getFloatFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    float output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%f", &output) == 1) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

unsigned int Scene::getUnsignedIntFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    unsigned int output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%u", &output) == 1) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

bool Scene::getAttributeExistence(TiXmlElement* iElement,
        char const* iAttribute) {
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString) {
        return true;
    } else {
        return false;
    }
}

xyPointDouble Scene::get2DdPointFromElementAttribute(TiXmlElement* iElement,
        char const* iAttribute, const std::string& Error) {
    xyPointDouble output;
    char * valString = (char *) iElement->Attribute(iAttribute);

    if (valString && sscanf(valString, "%lf %lf", &output.x, &output.y) == 2) {
        return output;
    }

    throw Exception(Error, true, iElement->Row());
}

/* End XML functions */

/* Set stuff */
void Scene::setBackgroundColor(color in) {
    backgroundColour[0] = in.r;
    backgroundColour[1] = in.g;
    backgroundColour[2] = in.b;
    backgroundColour[3] = in.a;
}

void Scene::setAmbientLightColor(color in) {
    ambientLightColour[0] = in.r;
    ambientLightColour[1] = in.g;
    ambientLightColour[2] = in.b;
    ambientLightColour[3] = in.a;
}

/* Validate values & strings */
GLdouble Scene::validateOpenGLColour(GLdouble input) {
    if (input < -1.0) {
        throw Exception("Out of range color value!", true);
        return -1.0;
    }

    if (input > 1.0) {
        throw Exception("Out of range color value!", true);
        return 1.0;
    }

    return input;
}

bool Scene::validateBoolean(std::string& input) {
    if (input == Xml::ATTRIBUTE_VALUE_TRUE) {
        return true;
    }

    if (input == Xml::ATTRIBUTE_VALUE_FALSE) {
        return false;
    }

    throw Exception("Invalid boolean value!", true);
    return false;
}

void Scene::setDrawMode(std::string& input) {
    if ((input == Xml::ATTRIBUTE_VALUE_DRAWMODE_FILL)
            || (input == Xml::ATTRIBUTE_VALUE_DRAWMODE_LINE)
            || (input == Xml::ATTRIBUTE_VALUE_DRAWMODE_POINT)) {
        this->drawMode = input;

    } else {
        throw Exception("Draw mode string not recognized!", true);
    }
}

void Scene::setShadingMode(std::string& input) {
    if ((input == Xml::ATTRIBUTE_VALUE_SHADING_FLAT)
            || (input == Xml::ATTRIBUTE_VALUE_SHADING_GOURAUD)) {
        this->shadingMode = input;
    } else {
        throw Exception("Shading mode string not recognized!", true);
    }
}

void Scene::setCullFace(std::string& input) {
    if ((input == Xml::ATTRIBUTE_VALUE_CULLFACE_NONE)
            || (input == Xml::ATTRIBUTE_VALUE_CULLFACE_BACK)
            || (input == Xml::ATTRIBUTE_VALUE_CULLFACE_FRONT)
            || (input == Xml::ATTRIBUTE_VALUE_CULLFACE_BOTH)) {
        this->cullFace = input;
    } else {
        throw Exception("Cull face string not recognized!", true);
    }
}

void Scene::setCullOrder(std::string& input) {
    if ((input == Xml::ATTRIBUTE_VALUE_CULLORDER_CCW)
            || (input == Xml::ATTRIBUTE_VALUE_CULLORDER_CW)) {
        this->cullOrder = input;
    } else {
        throw Exception("Cull order string not recognized!", true);
    }
}
/* End validate values & strings */

/* Apply globals */
void Scene::applyGlobals() {
    /* Background color [OK] */
    glClearColor(backgroundColour[0], backgroundColour[1], backgroundColour[2],
            backgroundColour[3]);

    /* Cull order */
    if (this->cullOrder == Xml::ATTRIBUTE_VALUE_CULLORDER_CCW) {
        glFrontFace(GL_CCW);
    } else {
        glFrontFace(GL_CW);

    }

    /* Shading mode [OK] */
    if (this->shadingMode == Xml::ATTRIBUTE_VALUE_SHADING_FLAT) {
        glShadeModel(GL_FLAT);

    } else {
        glShadeModel(GL_SMOOTH);

    }

    /* Cull face [OK] */
    if (this->cullFace == Xml::ATTRIBUTE_VALUE_CULLFACE_NONE) {
        glCullFace(GL_NONE);
    }

    if (this->cullFace == Xml::ATTRIBUTE_VALUE_CULLFACE_BACK) {
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    }

    if (this->cullFace == Xml::ATTRIBUTE_VALUE_CULLFACE_FRONT) {
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Cull order [OK] */
    if (this->cullFace == Xml::ATTRIBUTE_VALUE_CULLFACE_BOTH) {
        glCullFace(GL_BACK);
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Draw mode */
    if (this->drawMode == Xml::ATTRIBUTE_VALUE_DRAWMODE_FILL) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        drawModeInt = 0;
    }

    if (this->drawMode == Xml::ATTRIBUTE_VALUE_DRAWMODE_LINE) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        drawModeInt = 1;
    }

    if (this->drawMode == Xml::ATTRIBUTE_VALUE_DRAWMODE_POINT) {
        glPolygonMode(GL_FRONT, GL_POINT);
        glPolygonMode(GL_BACK, GL_POINT);
        drawModeInt = 2;
    }

    /* Enable depth comparisons */
    glEnable(GL_DEPTH_TEST);
}

GLboolean Scene::getLightingEnableStatus() {
    return this->lightingEnable;
}

void Scene::init() {
#ifdef ENGINE_VERBOSE
    std::cout << "Setting up globals" << std::endl;
#endif
    applyGlobals();
#ifdef ENGINE_VERBOSE
    std::cout << "Setting up lights" << std::endl;
#endif
    initLights();

#ifdef ENGINE_VERBOSE
    std::cout << "Setting up textures" << std::endl;
#endif
    initTextures();

#ifdef ENGINE_VERBOSE
    std::cout << "Assigning textures to respective appearances" << std::endl;
#endif
    initAppearanceTextures();

}

void Scene::initLights() {
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
        for (std::vector<Light*>::iterator it = lightVector.begin (); it != this->lightVector.end (); it++) {
            if ((*it)->getLightEnableStatus ()) {
                std::cout << "Light enabled: [" << (*it)->getID () << "]" << std::endl;
            }
        }
    }
#endif
}

/* Initialize textures (throws a fatal geException if texture file doesn't exist) */
void Scene::initTextures() {
    for (std::vector<Texture*>::iterator jt = textureVector.begin();
            jt != this->textureVector.end(); jt++) {
        (*jt)->loadTexture();
    }
}

/* Assign textures to appearances */
void Scene::initAppearanceTextures() {
    if (!this->textureVector.empty() && this->xmlAppearancesLoaded
            && this->xmlTexturesLoaded) {
        for (std::vector<Appearance*>::iterator it = appearanceVector.begin();
                it != this->appearanceVector.end(); it++) {

            std::string apperanceRef = (*it)->getTextureReference();

            for (std::vector<Texture*>::iterator jt = textureVector.begin();
                    jt != this->textureVector.end(); jt++) {
                if ((*jt)->getXmlId() == apperanceRef) {
                    (*it)->setTexture(*jt);

                    break;
                }
            }

        }
    }
}

/* Search initial camera and set current camera pointer to it */
void Scene::setInitialCamera() {
    if (cameraVector.empty()) {
        throw Exception("Camera vector is empty!", true);
    }

    if (currentCameraPointer != nullptr) {
        throw Exception("Current camera pointer is not NULL!", true);
    }

    std::vector<CameraInterface*>::iterator cameraIt;

    cameraIt = cameraVector.begin();
    while (cameraIt != cameraVector.end()) {
        if ((*cameraIt)->getID() == this->initialCamera) {
            currentCameraPointer = *cameraIt;
            break;
        }
        cameraIt++;
    }

    if (currentCameraPointer == nullptr) {
        throw Exception("Initial camera not found!", true);
    }
}

/* Draw lights (each light pushes and pops a matrix) */
void Scene::displayLights() {
    if (!this->lightVector.empty()) {
        for (std::vector<Light*>::iterator it = lightVector.begin();
                it != this->lightVector.end(); it++) {
            if ((*it)->getLightEnableStatus()) {
                (*it)->draw();
            }
        }
    }
}

/* Apply current camera view */
void Scene::applyCameraView() {
    if (this->currentCameraPointer != nullptr) {
        this->currentCameraPointer->applyView(this->aspectRatio);
    } else {
        throw Exception("The current camera pointer is NULL.", true);
    }
}

void Scene::display() {
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

void Scene::setViewRotate(GLdouble* input) {
    for (unsigned int i = 0; i < 16; i++) {
        customCameraMatrix[i] = input[i];
    }
}

GLdouble* Scene::getViewProjection() {
    return this->customCameraMatrix;
}

void Scene::update(unsigned long millis) {
    /* Call the animations update */
    if (!this->animationsVector.empty()) {
        for (std::vector<Animation*>::iterator it =
                this->animationsVector.begin();
                it != this->animationsVector.end(); ++it) {
            (*it)->updateAnimation(millis);
        }
    }

    /* Call the shader update */
    if (!this->waterLineVector.empty()) {
        for (std::vector<Primitives::WaterLine*>::iterator it =
                this->waterLineVector.begin();
                it != this->waterLineVector.end(); ++it) {
            (*it)->update(millis);
        }
    }
}

void Scene::activateCamera(unsigned int i) {
    if (i < this->cameraVector.size()) {
        currentCameraPointer = this->cameraVector.at(i);
        std::cout << ".";
        // CGFapplication::activeApp->forceRefresh();
    } else {
        throw Exception("Invalid camera chosen.", true);
    }
}

void Scene::setCurrentWindowSize(int windowX, int windowY) {
    this->windowSizeX = windowX;
    this->windowSizeY = windowY;
    this->aspectRatio = (double) windowX / (double) windowY;
}

Appearance* Scene::getAppearanceByString(std::string& in) {
    if (this->xmlAppearancesLoaded) {

        if (!this->appearanceVector.empty()) {
            for (std::vector<Appearance*>::iterator it =
                    this->appearanceVector.begin();
                    it != this->appearanceVector.end(); ++it) {
                if ((*it)->getAppearanceID() == in) {
                    return *it;
                }
            }

            throw Exception(
                    "XML: Reference to a non existing appearance [" + in + "].",
                    true);

        } else {
            throw Exception("Bug: appearances vector is empty!", true);
        }

    } else {
        throw Exception(
                "Bug: Attempting to find an apperance without appearances being loaded.",
                true);
        return nullptr;
    }
}

Animation* Scene::getAnimationByString(std::string& in) {
    if (this->xmlAnimationsLoaded) {

        if (!this->animationsVector.empty()) {
            for (std::vector<Animation*>::iterator it =
                    this->animationsVector.begin();
                    it != this->animationsVector.end(); ++it) {
                if ((*it)->getID() == in) {
                    return *it;
                }
            }

            throw Exception(
                    "XML: Reference to a non existing animation [" + in + "].",
                    true);

        } else {
            throw Exception(
                    "XML: Reference to an animation, but no animation exists!",
                    true);
        }

    } else {
        throw Exception(
                "Bug: Attempting to find an animation without animations being loaded.",
                true);
        return nullptr;
    }
}

void Scene::changeCameraToExernal() {
    currentCameraPointer = externalGuiCamera;
}

void Scene::changeCameraToSceneDefault() {
    std::vector<CameraInterface*>::iterator cameraIt;
    cameraIt = cameraVector.begin();
    while (cameraIt != cameraVector.end()) {
        if ((*cameraIt)->getID() == this->initialCamera) {
            currentCameraPointer = *cameraIt;
            break;
        }
        cameraIt++;
    }
}

unsigned int Scene::getNumberOfLights() {
    return this->numberOfLights;
}

int Scene::getPolygonMode() {
    if (drawModeInt >= 0 && drawModeInt <= 2) {
        return drawModeInt;
    }
    return 0;
}

void Scene::reSetPolygonMode(int in) {
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

bool Scene::getLightStatus(unsigned int number) {
    return lightVector[number]->getLightEnableStatus();
}

void Scene::disableLight(unsigned int number) {
    lightVector[number]->disable();
}

void Scene::enableLight(unsigned int number) {
    lightVector[number]->enable();
}

std::string Scene::getLightID(unsigned int number) {
    return this->lightVector[number]->getID();
}

unsigned int Scene::getNumberOfCameras() {
    return this->numberOfCameras;
}

void Scene::resetUserCamera() {
    externalGuiCamera->resetCamera();
}

std::string Scene::getCameraID(unsigned int number) {
    return this->cameraVector[number]->getID();
}

void Scene::setCamera(unsigned int number) {
    currentCameraPointer = cameraVector[number];
}

unsigned int Scene::getCurrentCamera() {
    for (unsigned int i = 0; i < cameraVector.size(); i++) {
        if (cameraVector[i] == currentCameraPointer) {
            return i;
        }
    }

    return 0;
}

void Scene::moveVehicleUp() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveUp();
        }
    }
}

void Scene::moveVehicleDown() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveDown();
        }
    }
}

void Scene::moveVehicleFront() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveFront();
        }
    }
}

void Scene::moveVehicleBack() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveBack();
        }
    }
}

void Scene::moveVehicleLeft() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveLeft();
        }
    }
}

void Scene::moveVehicleRight() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->moveRight();
        }
    }
}

void Scene::tiltVehicleLeft() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->tiltLeft();
        }
    }
}

void Scene::tiltVehicleRight() {
    if (!this->sceneVehicles.empty()) {
        for (std::vector<Primitives::Vehicle*>::iterator i =
                sceneVehicles.begin(); i != sceneVehicles.end(); i++) {
            (*i)->tiltRight();
        }
    }
}

Scene::~Scene() {
    if (graph != nullptr) {
        delete (graph);
    }

    if (doc != nullptr) {
        delete (doc);
    }
}

}
