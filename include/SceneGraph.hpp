/*
 * Eduardo Fernandes
 *
 * Scene graph class, includes the graph itself and the node class.
 */

#ifndef GEGRAPH_HPP_
#define GEGRAPH_HPP_

#include <Animation.hpp>
#include <Appearance.hpp>
#include <Primitives.hpp>
#include "includes.hpp"
#include "geTransform.hpp"
#include "geTexture.hpp"

namespace ge {

class Node {
protected:
    /* Node related */
    std::string ID;
    bool precalcDone;

    bool useDisplayList;
    bool displayListCreated;
    GLint displayListID;

    std::vector<std::string> childrenIdVector;
    std::vector<Node*> childrenVector;
    std::list<geTransform*> transformList;

    /* OpenGL related */
    /* The transformationsMatrix only includes current node transforms */
    GLdouble transformationsMatrix[16]; /* Ma */

    std::vector<GLdouble*> transformationMatrixVector;

    Appearance* nodeAppearance;
    geAnimation* nodeAnimation;

    std::vector<Primitives::PrimitiveInterface*> primitiveVector;

    void setTransformationMatrix(GLdouble* in);

    /* Result must be deleted */
    GLdouble* multiplyMatrix(GLdouble* left, GLdouble* right);

    /* Graph related */
    /* Appearance stack */
    static std::stack<Appearance*> appearanceStack;

    /* Creating display List (Avoid creating display lists inside of display lists) */
    static bool creatingDisplayList;

    static GLint displayListCount;

    /* Debug */
    void printMatrix16x1(GLdouble* in);

public:
    /* Constructor */
    Node(std::string& in, bool displayList);
    virtual ~Node();

    /* Input */
    void addTransform(geTransform* in);
    void setAppearance(Appearance* in);
    void addPrimitive(Primitives::PrimitiveInterface* in);
    void addChildrenID(std::string& in);
    void setAnimation(geAnimation* in);
    std::vector<std::string>& getChildrenIDVector();
    std::vector<Node*>& getChildrenVector();

    /* Precalc */
    void calculateNodeMatrix();

    /* Output */
    std::string getNodeID();
    Appearance* getAppearance();
    unsigned int getNodeDepth();

    /* Runtime (Draw method) */
    void draw();
    void drawHelper();
};

class SceneGraph {
private:
    std::string rootID;
    Node* rootNode;

    /* Internal */
    void setRootNode(Node* root);

    GLdouble identityMatrix[16];

    /* Default appearance values */
    GLfloat shininess;
    color emissive, ambient, diffuse, specular;
    Appearance* defaultRootAppearance;

    bool firstRun;

public:
    SceneGraph(std::string& root);
    virtual ~SceneGraph();

    void importNodesPointerVector(std::vector<Node*>& inputNodes);
    void importNodesPointerVectorHelper(std::vector<Node*>& unprocessedNodes,
            Node* parent);

    /* Recursive draw function */
    void draw();
};

} //namespace ge

#endif /* GEGRAPH_HPP_ */
