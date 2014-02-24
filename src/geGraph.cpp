/*
 * Eduardo Fernandes
 *
 * Scene graph methods.
 */

#include "geGraph.hpp"

/* Can't be in the header file, why? I don't know */
stack<geAppearance*> geNode::appearanceStack;
bool geNode::creatingDisplayList = false;
GLint geNode::displayListCount = 1;

geNode::geNode(string in, bool displayList) {
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

void geNode::addTransform(geTransform* in) {
    transformList.push_back(in);
}

void geNode::addPrimitive(gePrimitive* in) {
    this->primitiveVector.push_back(in);
}

void geNode::addChildrenID(string in) {
    childrenIdVector.push_back(in);
}

void geNode::setAnimation(geAnimation* in) {
    this->nodeAnimation = in;
}

vector<string>& geNode::getChildrenIDVector() {
    return this->childrenIdVector;
}

vector<geNode*>& geNode::getChildrenVector() {
    return this->childrenVector;
}

void geNode::setAppearance(geAppearance* in) {
    this->nodeAppearance = in;
}

geAppearance* geNode::getAppearance() {
    return this->nodeAppearance;
}

string geNode::getNodeID() {
    return this->ID;
}

void geNode::calculateNodeMatrix() {
    if (precalcDone) {
        return;
    }

    /* No transforms, keep identity matrix */
    if (transformList.empty()) {
        return;
    }

    for (list<geTransform*>::iterator k = transformList.begin(); k != transformList.end(); k++) {
        setTransformationMatrix(multiplyMatrix((*k)->getTransformationMatrix(), transformationsMatrix));
    }

    this->precalcDone = true;
}

void geNode::setTransformationMatrix(GLfloat* in) {
    for (unsigned int i = 0; i < 16; i++) {
        transformationsMatrix[i] = in[i];
    }
}

GLfloat* geNode::multiplyMatrix(GLfloat* left, GLfloat* right) {
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
void geNode::draw() {
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

void geNode::drawHelper() {
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
        for (vector<gePrimitive*>::iterator it = this->primitiveVector.begin(); it != this->primitiveVector.end(); it++) {
            (*it)->draw(appearanceStack.top()->getTextureSWrap(), appearanceStack.top()->getTextureTWrap());
        }
    }

    /* Check if we have to go deeper */
    if (!childrenVector.empty()) {
        for (vector<geNode*>::iterator it = this->childrenVector.begin(); it != this->childrenVector.end(); it++) {
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
void geNode::printMatrix16x1(GLfloat* in) {
    cout.precision(5);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    cout << "|" << in[0] << " " << in[1] << " " << in[2] << " " << in[3] << "|" << endl;
    cout << "|" << in[4] << " " << in[5] << " " << in[6] << " " << in[7] << "|" << endl;
    cout << "|" << in[8] << " " << in[9] << " " << in[10] << " " << in[11] << "|" << endl;
    cout << "|" << in[12] << " " << in[13] << " " << in[14] << " " << in[15] << "|" << endl;
    cout << endl;

    std::cout.unsetf(std::ios::floatfield);
    std::cout.unsetf(std::ios::fixed);
}

geNode::~geNode() {

}

/******************** GRAPH ********************/

geGraph::geGraph(string root) {
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

    this->defaultRootAppearance = new geAppearance("_INTERNAL_ROOT_APPEARANCE", emissive, ambient, diffuse, specular, shininess);

    this->firstRun = true;
}

void geGraph::setRootNode(geNode* root) {
    if (this->rootNode == NULL && root != NULL && (root->getNodeID() == this->rootID)) {
        this->rootNode = root;
    } else {
        throw geException("Null pointer, root already set or rootID mismatch.", true);
    }
}

void geGraph::importNodesPointerVector(std::vector<geNode*>& inputNodes) {
    if (!inputNodes.empty()) {

        /* Find root node and then remove the pointer from the inputNodes vector */
        for (vector<geNode*>::iterator k = inputNodes.begin(); k != inputNodes.end();) {
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

void geGraph::importNodesPointerVectorHelper(std::vector<geNode*>& inputNodes, geNode* parent) {
    parent->calculateNodeMatrix();

    /* Current node has no children so bail out */
    if (parent->getChildrenIDVector().empty()) {
        return;
    }

    if (!inputNodes.empty()) {
        /* Find children by ID then add pointer to vector */
        for (vector<string>::iterator si = parent->getChildrenIDVector().begin(); si != parent->getChildrenIDVector().end(); si++) {

            for (vector<geNode*>::iterator sp = inputNodes.begin(); sp != inputNodes.end(); sp++) {
                if ((*sp)->getNodeID() == (*si)) {
                    /* Children matches */
                    parent->getChildrenVector().push_back(*sp);

                }

            }

        }

        /* Recursive search */
        for (vector<geNode*>::iterator it = parent->getChildrenVector().begin(); it != parent->getChildrenVector().end(); it++) {
            importNodesPointerVectorHelper(inputNodes, *it);
        }

    } else {
        return;
    }

}

void geGraph::draw() {
    if (this->firstRun) {
        glGenLists(MAX_DISPLAY_LISTS);
        this->firstRun = false;
    }

    glPushMatrix();
    this->rootNode->draw();
    glPopMatrix();
}
