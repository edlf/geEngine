/*
 * Eduardo Fernandes
 *
 * Scene graph class, includes the graph itself and the node class.
 */

#ifndef GEGRAPH_HPP_
#define GEGRAPH_HPP_

#include "includes.hpp"
#include "gePrimitive.hpp"
#include "geTransform.hpp"
#include "geAppearance.hpp"
#include "geTexture.hpp"
#include "geAnimation.hpp"

using namespace std;

class geNode {
protected:
    /* Node related */
    string ID;
    bool precalcDone;

    bool useDisplayList;
    bool displayListCreated;
    GLint displayListID;

    vector<string> childrenIdVector;
    vector<geNode*> childrenVector;
    list<geTransform*> transformList;

    /* OpenGL related */
    /* The transformationsMatrix only includes current node transforms */
    GLfloat transformationsMatrix[16]; /* Ma */

    vector<GLfloat*> transformationMatrixVector;

    geAppearance* nodeAppearance;
    geAnimation* nodeAnimation;

    vector<gePrimitive*> primitiveVector;

    void setTransformationMatrix(GLfloat* in);

    /* Result must be deleted */
    GLfloat* multiplyMatrix(GLfloat* left, GLfloat* right);

    /* Graph related */
    /* Appearance stack */
    static stack<geAppearance*> appearanceStack;

    /* Creating display List (Avoid creating display lists inside of display lists) */
    static bool creatingDisplayList;

    static GLint displayListCount;

    /* Debug */
    void printMatrix16x1(GLfloat* in);
public:
    /* Constructor */
    geNode(string in, bool displayList);

    /* Input */
    void addTransform(geTransform* in);
    void setAppearance(geAppearance* in);
    void addPrimitive(gePrimitive* in);
    void addChildrenID(string in);
    void setAnimation(geAnimation* in);
    vector<string>& getChildrenIDVector();
    vector<geNode*>& getChildrenVector();

    /* Precalc */
    void calculateNodeMatrix();

    /* Output */
    string getNodeID();
    geAppearance* getAppearance();
    unsigned int getNodeDepth();

    /* Runtime (Draw method) */
    void draw();
    void drawHelper();

    ~geNode();
};

class geGraph {
private:
    string rootID;
    geNode* rootNode;

    /* Internal */
    void setRootNode(geNode* root);

    GLfloat identityMatrix[16];

    /* Default appearance values */
    GLfloat shininess;
    geColor emissive, ambient, diffuse, specular;
    geAppearance* defaultRootAppearance;

    bool firstRun;

public:
    geGraph(string root);

    void importNodesPointerVector(std::vector<geNode*>& inputNodes);
    void importNodesPointerVectorHelper(std::vector<geNode*>& unprocessedNodes, geNode* parent);

    /* Recursive draw function */
    void draw();
};

#endif /* GEGRAPH_HPP_ */
