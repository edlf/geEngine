/*
 * Eduardo Fernandes
 *
 * Scene graph methods.
 */

#include <SceneGraph.hpp>

namespace ge {
// Static members
std::stack<Appearance*> Node::appearanceStack;
bool  Node::creatingDisplayList = false;
GLint Node::displayListCount = 1;

Node::Node(std::string& in, bool displayList) {
    this->ID = in;
    this->precalcDone = false;
    this->nodeAppearance = NULL;
    this->useDisplayList = displayList;

    /* Copy paste is cheap */
    transformationsMatrix[0] = 1;
    transformationsMatrix[1] = 0;
    transformationsMatrix[2] = 0;
    transformationsMatrix[3] = 0;

    transformationsMatrix[4] = 0;
    transformationsMatrix[5] = 1;
    transformationsMatrix[6] = 0;
    transformationsMatrix[7] = 0;

    transformationsMatrix[8] = 0;
    transformationsMatrix[9] = 0;
    transformationsMatrix[10] = 1;
    transformationsMatrix[11] = 0;

    transformationsMatrix[12] = 0;
    transformationsMatrix[13] = 0;
    transformationsMatrix[14] = 0;
    transformationsMatrix[15] = 1;

    this->displayListID = -1;
    this->displayListCreated = false;

    this->nodeAnimation = NULL;
}

void Node::addTransform(geTransform* in) {
    transformList.push_back(in);
}

void Node::addPrimitive(Primitives::Base* in) {
    this->primitiveVector.push_back(in);
}

void Node::addChildrenID(std::string& in) {
    childrenIdVector.push_back(in);
}

void Node::setAnimation(geAnimation* in) {
    this->nodeAnimation = in;
}

std::vector<std::string>& Node::getChildrenIDVector() {
    return this->childrenIdVector;
}

std::vector<Node*>& Node::getChildrenVector() {
    return this->childrenVector;
}

void Node::setAppearance(Appearance* in) {
    this->nodeAppearance = in;
}

Appearance* Node::getAppearance() {
    return this->nodeAppearance;
}

std::string Node::getNodeID() {
    return this->ID;
}

void Node::calculateNodeMatrix() {
    if (precalcDone) {
        return;
    }

    /* No transforms, keep identity matrix */
    if (transformList.empty()) {
        return;
    }

    for (std::list<geTransform*>::iterator k = transformList.begin(); k != transformList.end(); k++) {
        setTransformationMatrix(multiplyMatrix((*k)->getTransformationMatrix(), transformationsMatrix));
    }

    this->precalcDone = true;
}

void Node::setTransformationMatrix(GLfloat* in) {
    for (unsigned int i = 0; i < 16; i++) {
        transformationsMatrix[i] = in[i];
    }
}

GLfloat* Node::multiplyMatrix(GLfloat* left, GLfloat* right) {
    GLfloat* output = new GLfloat[16];

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++) {

            GLfloat sum = 0.0;

            for (int k = 0; k < 4; k++) {
                sum = sum + left[i * 4 + k] * right[k * 4 + j];
            }

            output[i * 4 + j] = sum;
        }
    }

    return output;
}

/**** geNode: runtime ****/
void Node::draw() {
    /* Display List limits NOT respected */

    /*
     * Check if we need to create a display list
     * The node needs to have the display list flag defined to true
     * We must not be currently creating a display list
     * And we must not already have an display list created
     */

    if (this->useDisplayList && !(this->creatingDisplayList) && (!this->displayListCreated)) {
        this->creatingDisplayList = true;

        this->displayListID = this->displayListCount;

        glNewList(this->displayListCount, GL_COMPILE_AND_EXECUTE);
        drawHelper();
        glEndList();

        this->displayListCreated = true;
        this->creatingDisplayList = false;
        this->displayListCount++;

    } else {
        if (this->displayListCreated) {
            glCallList(this->displayListID);
        } else {
            drawHelper();
        }
    }
}

void Node::drawHelper() {
    /* Push the stacks */
    glPushMatrix();
    /* If node has an appearance push it to the stack */
    if (this->nodeAppearance != NULL) {
        appearanceStack.push(nodeAppearance);
    }

    /* Apply animation transform (if any) */
    if (this->nodeAnimation != NULL) {
        this->nodeAnimation->applyAnimation();
    }

    /* Apply node transform matrix */
    glMultMatrixf(transformationsMatrix);

    /* Apply apperance */
    appearanceStack.top()->apply();

    /* If there are primitives draw them */
    if (!this->primitiveVector.empty()) {
        for (std::vector<Primitives::Base*>::iterator it = this->primitiveVector.begin(); it != this->primitiveVector.end(); it++) {
            (*it)->draw(appearanceStack.top()->getTextureSWrap(), appearanceStack.top()->getTextureTWrap());
        }
    }

    /* Check if we have to go deeper */
    if (!childrenVector.empty()) {
        for (std::vector<Node*>::iterator it = this->childrenVector.begin(); it != this->childrenVector.end(); it++) {
            /* Call this method again but from the child node */
            (*it)->draw();
        }
    }

    /* Done, pop the stacks */
    if (this->nodeAppearance != NULL) {
        appearanceStack.pop();
    }
    glPopMatrix();
}
/**** geNode: runtime (end) ****/

/* Prints a 4x4 matrix from a 1x16 GLfloat array */
void Node::printMatrix16x1(GLfloat* in) {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    std::cout << "|" << in[0] << " " << in[1] << " " << in[2] << " " << in[3] << "|" << std::endl;
    std::cout << "|" << in[4] << " " << in[5] << " " << in[6] << " " << in[7] << "|" << std::endl;
    std::cout << "|" << in[8] << " " << in[9] << " " << in[10] << " " << in[11] << "|" << std::endl;
    std::cout << "|" << in[12] << " " << in[13] << " " << in[14] << " " << in[15] << "|" << std::endl;
    std::cout << std::endl;

    std::cout.unsetf(std::ios::floatfield);
    std::cout.unsetf(std::ios::fixed);
}

Node::~Node() {

}

/******************** GRAPH ********************/

SceneGraph::SceneGraph(std::string& root) {
    this->rootID = root;
    this->rootNode = NULL;

    this->identityMatrix[0] = 1;
    this->identityMatrix[1] = 0;
    this->identityMatrix[2] = 0;
    this->identityMatrix[3] = 0;

    this->identityMatrix[4] = 0;
    this->identityMatrix[5] = 1;
    this->identityMatrix[6] = 0;
    this->identityMatrix[7] = 0;

    this->identityMatrix[8] = 0;
    this->identityMatrix[9] = 0;
    this->identityMatrix[10] = 1;
    this->identityMatrix[11] = 0;

    this->identityMatrix[12] = 0;
    this->identityMatrix[13] = 0;
    this->identityMatrix[14] = 0;
    this->identityMatrix[15] = 1;

    /* Set appearance defaults */
    this->emissive.r = 1;
    this->emissive.g = 0;
    this->emissive.b = 0;
    this->emissive.a = 1;

    this->ambient.r = 1;
    this->ambient.g = 0;
    this->ambient.b = 0;
    this->ambient.a = 1;

    this->specular.r = 1;
    this->specular.g = 0;
    this->specular.b = 0;
    this->specular.a = 1;

    this->shininess = 2;

    const std::string internalAppearanceName = "_INTERNAL_ROOT_APPEARANCE";
    this->defaultRootAppearance = new Appearance(internalAppearanceName, emissive, ambient, diffuse, specular, shininess);

    this->firstRun = true;
}

void SceneGraph::setRootNode(Node* root) {
    if (this->rootNode == NULL && root != NULL && (root->getNodeID() == this->rootID)) {
        this->rootNode = root;
    } else {
        throw geException("Null pointer, root already set or rootID mismatch.", true);
    }
}

void SceneGraph::importNodesPointerVector(std::vector<Node*>& inputNodes) {
    if (!inputNodes.empty()) {

        /* Find root node and then remove the pointer from the inputNodes vector */
        for (std::vector<Node*>::iterator k = inputNodes.begin(); k != inputNodes.end();) {
            if (this->rootID == (*k)->getNodeID()) {
                setRootNode((*k));
                k = inputNodes.erase(k);
            } else {
                ++k;
            }
        }

        /* Check if we got a rootNode, if so start a recursive search for children */
        if (rootNode == NULL) {
            throw geException("Root node not found!", true);
        }

        /* Check if root node has apperance */
        if (rootNode->getAppearance() == NULL) {
            rootNode->setAppearance(defaultRootAppearance);
        }

        importNodesPointerVectorHelper(inputNodes, rootNode);

        /* Clean unprocessed Nodes Vector */
        inputNodes.clear();

    } else {
        throw geException("No nodes to process received.", true);
    }
}

void SceneGraph::importNodesPointerVectorHelper(std::vector<Node*>& inputNodes, Node* parent) {
    parent->calculateNodeMatrix();

    /* Current node has no children so bail out */
    if (parent->getChildrenIDVector().empty()) {
        return;
    }

    if (!inputNodes.empty()) {
        /* Find children by ID then add pointer to vector */
        for (std::vector<std::string>::iterator si = parent->getChildrenIDVector().begin(); si != parent->getChildrenIDVector().end(); si++) {

            for (std::vector<Node*>::iterator sp = inputNodes.begin(); sp != inputNodes.end(); sp++) {
                if ((*sp)->getNodeID() == (*si)) {
                    /* Children matches */
                    parent->getChildrenVector().push_back(*sp);

                }

            }

        }

        /* Recursive search */
        for (std::vector<Node*>::iterator it = parent->getChildrenVector().begin(); it != parent->getChildrenVector().end(); it++) {
            importNodesPointerVectorHelper(inputNodes, *it);
        }

    } else {
        return;
    }

}

void SceneGraph::draw() {
    if (this->firstRun) {
        glGenLists(MAX_DISPLAY_LISTS);
        this->firstRun = false;
    }

    glPushMatrix();
    this->rootNode->draw();
    glPopMatrix();
}
}
