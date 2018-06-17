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
    GLfloat transformationsMatrix[16]; /* Ma */

    std::vector<GLfloat*> transformationMatrixVector;

    Appearance* nodeAppearance;
    geAnimation* nodeAnimation;

    std::vector<Primitives::Base*> primitiveVector;

    void setTransformationMatrix(GLfloat* in);

    /* Result must be deleted */
    GLfloat* multiplyMatrix(GLfloat* left, GLfloat* right);

    /* Graph related */
    /* Appearance stack */
    static std::stack<Appearance*> appearanceStack;

    /* Creating display List (Avoid creating display lists inside of display lists) */
    static bool creatingDisplayList;

    static GLint displayListCount;

    /* Debug */
    void printMatrix16x1(GLfloat* in);
public:
    /* Constructor */
    Node(std::string& in, bool displayList);

    /* Input */
    void addTransform(geTransform* in);
    void setAppearance(Appearance* in);
    void addPrimitive(Primitives::Base* in);
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

    ~Node();
};

class SceneGraph {
private:
    std::string rootID;
    Node* rootNode;

    /* Internal */
    void setRootNode(Node* root);

    GLfloat identityMatrix[16];

    /* Default appearance values */
    GLfloat shininess;
    geColor emissive, ambient, diffuse, specular;
    Appearance* defaultRootAppearance;

    bool firstRun;

public:
    SceneGraph(std::string& root);

    void importNodesPointerVector(std::vector<Node*>& inputNodes);
    void importNodesPointerVectorHelper(std::vector<Node*>& unprocessedNodes, Node* parent);

    /* Recursive draw function */
    void draw();
};

} //namespace ge

#endif /* GEGRAPH_HPP_ */
