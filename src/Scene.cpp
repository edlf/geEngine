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
    xmlRootElement = doc->FirstChildElement(Xml::DocumentType);

    if (xmlRootElement == nullptr) {
        throw Exception("No correct root element found!", true);
    }
}

void Scene::xmlLoadMainElements() {
    xmlGlobalsElement = xmlRootElement->FirstChildElement(Xml::Nodes::Globals::RootNode);
    xmlCamerasElement = xmlRootElement->FirstChildElement(Xml::Nodes::Cameras::RootNode);
    xmlLightingElement = xmlRootElement->FirstChildElement(Xml::Nodes::Lights::RootNode);
    xmlTexturesElement = xmlRootElement->FirstChildElement(
            Xml::Nodes::Textures::RootNode);
    xmlAppearancesElement = xmlRootElement->FirstChildElement(
            Xml::Nodes::Appearances::RootNode);
    xmlGraphElement = xmlRootElement->FirstChildElement(Xml::Nodes::Graph);
    xmlAnimationsElement = xmlRootElement->FirstChildElement(
            Xml::Nodes::Animations::RootNode);
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
        throw Exception(Xml::Errors::SECTION_GLOBALS, true);
    }

    color bgColor = getColorFromElementAttribute(xmlGlobalsElement,
            Xml::Nodes::Globals::Attributes::Background, Xml::Errors::ATTRIBUTE_BACKGROUND);
    setBackgroundColor(bgColor);

    std::string drawMode = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::Nodes::Globals::Attributes::DrawMode, Xml::Errors::ATTRIBUTE_DRAWMODE);
    setDrawMode(drawMode);

    std::string shadingMode = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::Nodes::Globals::Attributes::Shading, Xml::Errors::ATTRIBUTE_SHADING);
    setShadingMode(shadingMode);

    std::string cullFace = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::Nodes::Globals::Attributes::CullFace, Xml::Errors::ATTRIBUTE_CULLFACE);
    setCullFace(cullFace);

    std::string cullOrder = getStringFromElementAttribute(xmlGlobalsElement,
            Xml::Nodes::Globals::Attributes::CullOrder, Xml::Errors::ATTRIBUTE_CULLORDER);
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
            Xml::Nodes::Cameras::Attributes::InitialCamera, Xml::Errors::ATTRIBUTE_INITIAL_CAMERA);

    /* Get camera first child */
    TiXmlElement* xmlCameraElement = xmlCamerasElement->FirstChildElement();

    bool camerasLastRun = false;
    while (xmlCameraElement != nullptr && !camerasLastRun) {
        /* Checks if we are loading the last child, if so this will be the last run. */
        if (xmlCameraElement == xmlCamerasElement->LastChild()) {
            camerasLastRun = true;
        }

        /* Camera is perspective */
        if (strcmp(xmlCameraElement->Value(), Xml::Nodes::Cameras::Perspective)
                == 0) {
            std::string cameraId;
            float nearIn, farIn, angle;
            xyzPointDouble pos, target;

            cameraId = getStringFromElementAttribute(xmlCameraElement,
                    Xml::GenericAttributes::ID, Xml::Errors::SECTION_CAMERA_ID);
            nearIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Near,
                    Xml::Errors::ATTRIBUTE_CAMERA_NEAR);
            farIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Far, Xml::Errors::ATTRIBUTE_CAMERA_FAR);
            angle = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Angle,
                    Xml::Errors::ATTRIBUTE_CAMERA_ANGLE);
            pos = getDoublePointFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Position,
                    Xml::Errors::ATTRIBUTE_CAMERA_POSITION);
            target = getDoublePointFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Target,
                    Xml::Errors::ATTRIBUTE_CAMERA_TARGET);

            cameraVector.push_back(
                    new PerspectiveCamera(cameraId, nearIn, farIn, angle, pos,
                            target));

            numberOfPerspectiveCameras++;
        }

        /* Camera is ortho */
        if (strcmp(xmlCameraElement->Value(), Xml::Nodes::Cameras::Ortho) == 0) {
            std::string cameraId;
            float nearIn, farIn, left, right, top, bottom;

            cameraId = getStringFromElementAttribute(xmlCameraElement,
                    Xml::GenericAttributes::ID, Xml::Errors::SECTION_CAMERA_ID);
            nearIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Near,
                    Xml::Errors::ATTRIBUTE_CAMERA_NEAR);
            farIn = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Far, Xml::Errors::ATTRIBUTE_CAMERA_FAR);
            left = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Left,
                    Xml::Errors::ATTRIBUTE_CAMERA_LEFT);
            right = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Right,
                    Xml::Errors::ATTRIBUTE_CAMERA_RIGHT);
            top = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Top, Xml::Errors::ATTRIBUTE_CAMERA_TOP);
            bottom = getFloatFromElementAttribute(xmlCameraElement,
                    Xml::Nodes::Cameras::Attributes::Bottom,
                    Xml::Errors::ATTRIBUTE_CAMERA_BOTTOM);

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
            Xml::Nodes::Lights::Attributes::DoubleSided,
            Xml::Errors::ATTRIBUTE_LIGHTING_DOUBLESIDED);
    lightingDoubleSided = validateBoolean(lightingDS);

    std::string lightingL = getStringFromElementAttribute(xmlLightingElement,
            Xml::Nodes::Lights::Attributes::Local, Xml::Errors::ATTRIBUTE_LIGHTING_LOCAL);
    lightingLocal = validateBoolean(lightingL);

    std::string lightingE = getStringFromElementAttribute(xmlLightingElement,
            Xml::GenericAttributes::Enabled, Xml::Errors::ATTRIBUTE_LIGHTING_ENABLED);
    lightingEnable = validateBoolean(lightingE);

    color ambientC = getColorFromElementAttribute(xmlLightingElement,
            Xml::Nodes::Lights::Attributes::Ambient,
            Xml::Errors::ATTRIBUTE_LIGHTING_G_AMBIENT);
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
        if (strcmp(xmlLightElement->Value(), Xml::Nodes::Lights::Omni) == 0) {
            std::string lightId;
            bool lightEnable;
            xyzPointFloat location;
            color ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement,
                    Xml::GenericAttributes::ID, Xml::Errors::SECTION_LIGHTING_ID);

            std::string enabled = getStringFromElementAttribute(xmlLightElement,
                    Xml::GenericAttributes::Enabled,
                    Xml::Errors::SECTION_LIGHTING_ENABLED);
            lightEnable = validateBoolean(enabled);

            location = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Location,
                    Xml::Errors::ATTRIBUTE_LIGHTING_LOCATION);
            ambient = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Ambient,
                    Xml::Errors::ATTRIBUTE_LIGHTING_AMBIENT);
            diffuse = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Diffuse,
                    Xml::Errors::ATTRIBUTE_LIGHTING_DIFFUSE);
            specular = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Specular,
                    Xml::Errors::ATTRIBUTE_LIGHTING_SPECULAR);

            lightVector.push_back(
                    new OmniLight(lightId, numberOfLights, lightEnable,
                            location, ambient, diffuse, specular));
            numberOfOmniLights++;
        }

        /* Spot light */
        if (strcmp(xmlLightElement->Value(), Xml::Nodes::Lights::Spot) == 0) {
            std::string lightId;
            bool lightEnable;
            GLfloat angle, exponent;
            xyzPointFloat location;
            xyzPointFloat direction;
            color ambient, diffuse, specular;

            lightId = getStringFromElementAttribute(xmlLightElement,
                    Xml::GenericAttributes::ID, Xml::Errors::SECTION_LIGHTING_ID);

            std::string enabled = getStringFromElementAttribute(xmlLightElement,
                    Xml::GenericAttributes::Enabled,
                    Xml::Errors::SECTION_LIGHTING_ENABLED);
            lightEnable = validateBoolean(enabled);

            location = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Location,
                    Xml::Errors::ATTRIBUTE_LIGHTING_LOCATION);
            ambient = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Ambient,
                    Xml::Errors::ATTRIBUTE_LIGHTING_AMBIENT);
            diffuse = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Diffuse,
                    Xml::Errors::ATTRIBUTE_LIGHTING_DIFFUSE);
            specular = getColorFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Specular,
                    Xml::Errors::ATTRIBUTE_LIGHTING_SPECULAR);
            angle = getFloatFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Angle, Xml::Errors::SECTION_LIGHTING_ANGLE);
            exponent = getFloatFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Exponent,
                    Xml::Errors::ATTRIBUTE_LIGHTING_EXPONENT);
            direction = getFloatPointFromElementAttribute(xmlLightElement,
                    Xml::Nodes::Lights::Attributes::Direction,
                    Xml::Errors::ATTRIBUTE_LIGHTING_DIRECTION);

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
                Xml::GenericAttributes::ID, Xml::Errors::SECTION_TEXTURE_ID);
        textureFileName = getStringFromElementAttribute(xmlTextureElement,
                Xml::GenericAttributes::File, Xml::Errors::SECTION_TEXTURE_FILE);

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
                Xml::GenericAttributes::ID, Xml::Errors::SECTION_APPEARANCE_ID);
        emissive = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::Nodes::Appearances::Attributes::Emissive,
                Xml::Errors::ATTRIBUTE_APPEARANCE_EMISSIVE);
        ambient = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::Nodes::Appearances::Attributes::Ambient,
                Xml::Errors::ATTRIBUTE_APPEARANCE_AMBIENT);
        diffuse = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::Nodes::Appearances::Attributes::Diffuse,
                Xml::Errors::ATTRIBUTE_APPEARANCE_DIFFUSE);
        specular = getColorFromElementAttribute(xmlAppearanceElement,
                Xml::Nodes::Appearances::Attributes::Specular,
                Xml::Errors::ATTRIBUTE_APPEARANCE_SPECULAR);
        shininess = getFloatFromElementAttribute(xmlAppearanceElement,
                Xml::Nodes::Appearances::Attributes::Shininess,
                Xml::Errors::ATTRIBUTE_APPEARANCE_SHININESS);
        hasTexture = getAttributeExistence(xmlAppearanceElement,
                Xml::Nodes::Textures::Attributes::ID);

        if (hasTexture) {
            textureId = getStringFromElementAttribute(xmlAppearanceElement,
                    Xml::Nodes::Textures::Attributes::ID,
                    Xml::Errors::ATTRIBUTE_APPEARANCE_TEXTURE_ID);
            texlength_s = getFloatFromElementAttribute(xmlAppearanceElement,
                    Xml::Nodes::Textures::Attributes::LenghtS,
                    Xml::Errors::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S);
            texlength_t = getFloatFromElementAttribute(xmlAppearanceElement,
                    Xml::Nodes::Textures::Attributes::LenghtT,
                    Xml::Errors::ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T);

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
            Xml::Nodes::DisplayLists::RootNode, Xml::Errors::ATTRIBUTE_GRAPH_ROOTID);

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
                Xml::GenericAttributes::ID, Xml::Errors::SECTION_GRAPH_ID);

        /* Display list */
        bool displayList;
        bool displayListAttribute = getAttributeExistence(xmlNodeElement,
                Xml::Nodes::DisplayLists::RootNode);

        if (displayListAttribute) {
            std::string displayListTemp;
            displayListTemp = getStringFromElementAttribute(xmlNodeElement,
                    Xml::Nodes::DisplayLists::RootNode,
                    Xml::Errors::ATTRIBUTE_NODE_DISPLAYLIST);
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
                Xml::Nodes::Transforms::RootNode);
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
                    Xml::Nodes::Transforms::Scale) == 0) {
                /* Factor */
                xyzPointDouble scaleFactor;
                scaleFactor = getDoublePointFromElementAttribute(
                        xmlTransformElement, Xml::Nodes::Transforms::Attributes::Factor,
                        Xml::Errors::ATTRIBUTE_TRANSFORM_FACTOR);

                temporaryNode->addTransform(
                        new TransformScale(scaleFactor, numberOfTransforms));
                numberOfTransforms++;
            }

            if (strcmp(xmlTransformElement->Value(),
                    Xml::Nodes::Transforms::Rotate) == 0) {
                std::string axis;
                int axisInt;
                float angle;

                /* Axis */
                axis = getStringFromElementAttribute(xmlTransformElement,
                        Xml::Nodes::Transforms::Attributes::Axis,
                        Xml::Errors::ATTRIBUTE_TRANSFORM_AXIS);

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
                        Xml::Nodes::Appearances::Attributes::Angle, Xml::Errors::ATTRIBUTE_ANGLE);

                temporaryNode->addTransform(
                        new TransformRotate(axisInt, angle,
                                numberOfTransforms));
                numberOfTransforms++;
            }

            /* Translate */
            if (strcmp(xmlTransformElement->Value(),
                    Xml::Nodes::Transforms::Translate) == 0) {
                xyzPointDouble translate;
                translate = getDoublePointFromElementAttribute(
                        xmlTransformElement, Xml::Nodes::Transforms::Attributes::To,
                        Xml::Errors::ATTRIBUTE_TRANSFORM_TO);

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
                xmlNodeElement->FirstChildElement(Xml::Nodes::Appearances::NodeReference);
        if (xmlAppearancerefElement == nullptr) {

        } else {
            std::string appearanceId;
            /* Id */
            appearanceId = getStringFromElementAttribute(
                    xmlAppearancerefElement, Xml::GenericAttributes::ID,
                    Xml::Errors::SECTION_GRAPH_APPEARANCE_ID);

            temporaryNode->setAppearance(getAppearanceByString(appearanceId));
        }
        /* End appearanceref */

        /*************************************** Begin animationref ***************************************/
        TiXmlElement* xmlAnimationrefElement =
                xmlNodeElement->FirstChildElement(Xml::Nodes::Animations::NodeReference);
        if (xmlAnimationrefElement == nullptr) {

        } else {
            /* Id */
            std::string animationId;
            animationId = getStringFromElementAttribute(xmlAnimationrefElement,
                    Xml::GenericAttributes::ID, Xml::Errors::SECTION_GRAPH_ANIMATION_ID);

            temporaryNode->setAnimation(getAnimationByString(animationId));
        }
        /* End animationref */

        /*************************************** Begin children ***************************************/
        /* Get node first child (children) */
        int numberOfChildren = 0;
        TiXmlElement* xmlChildrensElement = xmlNodeElement->FirstChildElement(
                Xml::Blocks::Children);
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
                    Xml::Nodes::Appearances::Rectangle) == 0) {
                xyPointDouble pt1, pt2;

                /* XY1 */
                pt1 = get2DdPointFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::xy1,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_XY1);

                /* XY2 */
                pt2 = get2DdPointFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::xy2,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_XY2);

                /* Create rectangle and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Rectangle(pt1, pt2);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Triangle*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::Nodes::Appearances::Triangle) == 0) {
                xyzPointDouble point1, point2, point3;
                point1 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::xyz1,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_XYZ1);
                point2 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::xyz2,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_XYZ2);
                point3 = getDoublePointFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::xyz3,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_XYZ3);

                /* Create triangle and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Triangle(point1, point2, point3);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Cylinder*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::Nodes::Appearances::Cylinder) == 0) {
                float base, top, height;
                unsigned int slices, stacks;

                base = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Base,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_CYL_BASE);
                top = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Top,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_CYL_TOP);
                height = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Height,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_CYL_HEIGHT);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Slices,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_CYL_SLICES);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Stacks,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_CYL_STACKS);

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
                    Xml::Nodes::Appearances::Sphere) == 0) {
                float radius;
                unsigned int slices, stacks;

                radius = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Radius,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Slices,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_SPHERE_SLICES);
                stacks = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Stacks,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_SPHERE_STACKS);

                /* Create sphere and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Sphere(radius, slices, stacks);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /*Torus*/
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::Nodes::Appearances::Torus) == 0) {
                float inner, outer;
                unsigned int slices, loops;

                inner = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Inner,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_TORUS_INNER);
                outer = getFloatFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Outer,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_TORUS_OUTER);
                slices = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Slices,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_TORUS_SLICES);
                loops = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Loops,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_TORUS_LOOPS);

                /* Create torus and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Torus(inner, outer, slices, loops);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /* Plane */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::Nodes::Appearances::Plane) == 0) {
                unsigned int parts;

                parts = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Parts,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_PLANE_PARTS);

                /* Create plane and store pointer in graph */
                Primitives::PrimitiveInterface* primitiveTemp =
                        new Primitives::Plane(parts);
                temporaryNode->addPrimitive(primitiveTemp);
                primitiveTemp = nullptr;

                numberOfChildren++;
            }

            /* Patch */
            if (strcmp(xmlChildrenElement->Value(),
                    Xml::Nodes::Appearances::Patch) == 0) {
                unsigned int order, partsU, partsV, compute;

                order = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Order,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_ORDER);
                partsU = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::PartsU,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_PARTSU);
                partsV = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::PartsV,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_PARTSV);
                compute = getUnsignedIntFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::Compute,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE);

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
                            Xml::Nodes::Appearances::ControlPoint) == 0) {
                        xyzPointDouble point;
                        point.x = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::Nodes::Appearances::Attributes::x,
                                Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_X);
                        point.y = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::Nodes::Appearances::Attributes::y,
                                Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_Y);
                        point.z = getFloatFromElementAttribute(
                                xmlPatchPointElement,
                                Xml::Nodes::Appearances::Attributes::z,
                                Xml::Errors::ATTRIBUTE_PRIMITIVE_PATCH_Z);

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
                    Xml::Nodes::Appearances::Vehicle) == 0) {

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
                    Xml::Nodes::Appearances::WaterLine) == 0) {
                std::string hmap, tmap, fshader, vshader;

                hmap = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::HeightMap,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP);
                tmap = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::TextureMap,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP);
                fshader = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::FragmentShader,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER);
                vshader = getStringFromElementAttribute(xmlChildrenElement,
                        Xml::Nodes::Appearances::Attributes::VertexShader,
                        Xml::Errors::ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR);

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
            if (strcmp(xmlChildrenElement->Value(), Xml::Blocks::NodeReference) == 0) {
                std::string nodeReferenceId;
                nodeReferenceId = getStringFromElementAttribute(
                        xmlChildrenElement, Xml::GenericAttributes::ID,
                        Xml::Errors::SECTION_GRAPH_CHILDREN_ID);

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
                Xml::GenericAttributes::ID, Xml::Errors::ATTRIBUTE_ANIMATION_ID);
        animationSpan = getFloatFromElementAttribute(xmlAnimationElement,
                Xml::Nodes::Animations::Attributes::Span,
                Xml::Errors::ATTRIBUTE_ANIMATION_SPAN);
        animationType = getStringFromElementAttribute(xmlAnimationElement,
                Xml::Nodes::Animations::Attributes::Type,
                Xml::Errors::ATTRIBUTE_ANIMATION_TYPE);

        /* Check the animation type string */
        if (animationType == Xml::Nodes::Animations::Values::Linear) {
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
                    Xml::Nodes::Appearances::ControlPoint) == 0) {
                xyzPointDouble point;

                point.x = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::Nodes::Animations::Attributes::x,
                        Xml::Errors::ATTRIBUTE_ANIMATION_X);
                point.y = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::Nodes::Animations::Attributes::y,
                        Xml::Errors::ATTRIBUTE_ANIMATION_Y);
                point.z = getFloatFromElementAttribute(xmlAnimationPointElement,
                        Xml::Nodes::Animations::Attributes::z,
                        Xml::Errors::ATTRIBUTE_ANIMATION_Z);

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
    if (input == Xml::GenericAttributes::ValueTrue) {
        return true;
    }

    if (input == Xml::GenericAttributes::ValueFalse) {
        return false;
    }

    throw Exception("Invalid boolean value!", true);
    return false;
}

void Scene::setDrawMode(std::string& input) {
    if ((input == Xml::Nodes::Globals::Values::DrawModeFill)
            || (input == Xml::Nodes::Globals::Values::DrawmodeLine)
            || (input == Xml::Nodes::Globals::Values::DrawModePoint)) {
        this->drawMode = input;

    } else {
        throw Exception("Draw mode string not recognized!", true);
    }
}

void Scene::setShadingMode(std::string& input) {
    if ((input == Xml::Nodes::Globals::Values::ShadingFlat)
            || (input == Xml::Nodes::Globals::Values::ShadingGouraud)) {
        this->shadingMode = input;
    } else {
        throw Exception("Shading mode string not recognized!", true);
    }
}

void Scene::setCullFace(std::string& input) {
    if ((input == Xml::Nodes::Globals::Values::CullFaceNone)
            || (input == Xml::Nodes::Globals::Values::CullFaceBack)
            || (input == Xml::Nodes::Globals::Values::CullFaceFront)
            || (input == Xml::Nodes::Globals::Values::CullFaceBoth)) {
        this->cullFace = input;
    } else {
        throw Exception("Cull face string not recognized!", true);
    }
}

void Scene::setCullOrder(std::string& input) {
    if ((input == Xml::Nodes::Globals::Values::CullOrderCCW)
            || (input == Xml::Nodes::Globals::Values::CullOrderCW)) {
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
    if (this->cullOrder == Xml::Nodes::Globals::Values::CullOrderCCW) {
        glFrontFace(GL_CCW);
    } else {
        glFrontFace(GL_CW);

    }

    /* Shading mode [OK] */
    if (this->shadingMode == Xml::Nodes::Globals::Values::ShadingFlat) {
        glShadeModel(GL_FLAT);

    } else {
        glShadeModel(GL_SMOOTH);

    }

    /* Cull face [OK] */
    if (this->cullFace == Xml::Nodes::Globals::Values::CullFaceNone) {
        glCullFace(GL_NONE);
    }

    if (this->cullFace == Xml::Nodes::Globals::Values::CullFaceBack) {
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    }

    if (this->cullFace == Xml::Nodes::Globals::Values::CullFaceFront) {
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Cull order [OK] */
    if (this->cullFace == Xml::Nodes::Globals::Values::CullFaceBoth) {
        glCullFace(GL_BACK);
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    /* Draw mode */
    if (this->drawMode == Xml::Nodes::Globals::Values::DrawModeFill) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        drawModeInt = 0;
    }

    if (this->drawMode == Xml::Nodes::Globals::Values::DrawmodeLine) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        drawModeInt = 1;
    }

    if (this->drawMode == Xml::Nodes::Globals::Values::DrawModePoint) {
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
