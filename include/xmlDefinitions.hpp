/*
 * Eduardo Fernandes
 *
 * Yaf file definitions for parsing.
 * Error messages for the parser.
 */

#ifndef XMLDEFS_HPP_
#define XMLDEFS_HPP_

namespace ge {
namespace Xml {

// DOCUMENT TYPE
const char* const DOCUMENT_TYPE = "yaf";

// MAIN SECTION TAGS
const char* const SECTION_GLOBALS = "globals";
const char* const SECTION_CAMERAS = "cameras";
const char* const SECTION_LIGHTS = "lighting";
const char* const SECTION_TEXTURES = "textures";
const char* const SECTION_APPEARANCES = "appearances";
const char* const SECTION_GRAPH = "graph";
const char* const SECTION_ANIMATIONS = "animations";

// BLOCK TAGS
const char* const BLOCK_NODE = "node";
const char* const BLOCK_TRANSFORMS = "transforms";
const char* const BLOCK_CHILDREN = "children";

// REFERENCE TAGS
const char* const REFERENCE_APPEARANCE = "appearanceref";
const char* const REFERENCE_NODE = "noderef";
const char* const REFERENCE_ANIMATION = "animationref";

// COMMON ATTRIBUTES
const char* const ATTRIBUTE_ID = "id";
const char* const ATTRIBUTE_FILE = "file";
const char* const ATTRIBUTE_ENABLED = "enabled";
const char* const ATTRIBUTE_VALUE_TRUE = "true";
const char* const ATTRIBUTE_VALUE_FALSE = "false";
const char* const ATTRIBUTE_ANGLE = "angle";

// GLOBAL ATTRIBUTES
const char* const ATTRIBUTE_BACKGROUND = "background";

const char* const ATTRIBUTE_DRAWMODE = "drawmode";
const char* const ATTRIBUTE_VALUE_DRAWMODE_FILL = "fill";
const char* const ATTRIBUTE_VALUE_DRAWMODE_LINE = "line";
const char* const ATTRIBUTE_VALUE_DRAWMODE_POINT = "point";

const char* const ATTRIBUTE_SHADING = "shading";
const char* const ATTRIBUTE_VALUE_SHADING_FLAT = "flat";
const char* const ATTRIBUTE_VALUE_SHADING_GOURAUD = "gouraud";

const char* const ATTRIBUTE_CULLFACE = "cullface";
const char* const ATTRIBUTE_VALUE_CULLFACE_NONE = "none";
const char* const ATTRIBUTE_VALUE_CULLFACE_BACK = "back";
const char* const ATTRIBUTE_VALUE_CULLFACE_FRONT = "front";
const char* const ATTRIBUTE_VALUE_CULLFACE_BOTH = "both";

const char* const ATTRIBUTE_CULLORDER = "cullorder";
const char* const ATTRIBUTE_VALUE_CULLORDER_CCW = "CCW";
const char* const ATTRIBUTE_VALUE_CULLORDER_CW = "CW";

// CAMERA TAGS
const char* const ATTRIBUTE_INITIAL_CAMERA = "initial";
const char* const SECTION_CAMERA_PERSPECTIVE = "perspective";
const char* const SECTION_CAMERA_ORTHO = "ortho";

// CAMERA ATTRIBUTES (COMMON)
const char* const ATTRIBUTE_CAMERA_NEAR = "near";
const char* const ATTRIBUTE_CAMERA_FAR = "far";

// PERSPECTIVE CAMERA ATTRIBUTES
const char* const ATTRIBUTE_CAMERA_POSITION = "pos";
const char* const ATTRIBUTE_CAMERA_TARGET = "target";
const char* const ATTRIBUTE_CAMERA_ANGLE = "angle";

// ORTHO CAMERA ATTRIBUTES
const char* const ATTRIBUTE_CAMERA_LEFT = "left";
const char* const ATTRIBUTE_CAMERA_RIGHT = "right";
const char* const ATTRIBUTE_CAMERA_TOP = "top";
const char* const ATTRIBUTE_CAMERA_BOTTOM = "bottom";

// LIGHTING TAGS
const char* const SECTION_LIGHTING_OMNI = "omni";
const char* const SECTION_LIGHTING_SPOT = "spot";

// LIGHTING ATTRIBUTES
const char* const ATTRIBUTE_LIGHTING_DOUBLESIDED = "doublesided";
const char* const ATTRIBUTE_LIGHTING_LOCAL = "local";

// OMNI LIGHTING ATTRIBUTES;
const char* const ATTRIBUTE_LIGHTING_LOCATION = "location";
const char* const ATTRIBUTE_LIGHTING_AMBIENT = "ambient";
const char* const ATTRIBUTE_LIGHTING_DIFFUSE = "diffuse";
const char* const ATTRIBUTE_LIGHTING_SPECULAR = "specular";

// SPOT LIGHTING ATTRIBUTES
const char* const ATTRIBUTE_LIGHTING_EXPONENT = "exponent";
const char* const ATTRIBUTE_LIGHTING_DIRECTION = "direction";

// APPEARANCE TAG
const char* const SECTION_APPEARANCE = "appearance";

// APPEARANCE ATTRIBUTES
const char* const ATTRIBUTE_APPEARANCE_EMISSIVE = "emissive";
const char* const ATTRIBUTE_APPEARANCE_AMBIENT = "ambient";
const char* const ATTRIBUTE_APPEARANCE_DIFFUSE = "diffuse";
const char* const ATTRIBUTE_APPEARANCE_SPECULAR = "specular";
const char* const ATTRIBUTE_APPEARANCE_SHININESS = "shininess";

// APPEARANCE ATTRIBUTES (OPTIONAL)
const char* const ATTRIBUTE_APPEARANCE_TEXTURE_ID = "textureref";
const char* const ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S = "texlength_s";
const char* const ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T = "texlength_t";

// NODE ATTRIBUTES
const char* const ATTRIBUTE_NODE_DISPLAYLIST = "displaylist";

// GRAPH ATTRIBUTES
const char* const ATTRIBUTE_GRAPH_ROOTID = "rootid";

// TRANSFORM TAG
const char* const SECTION_TRANSFORM_TRANSLATE = "translate";
const char* const SECTION_TRANSFORM_ROTATE = "rotate";
const char* const SECTION_TRANSFORM_SCALE = "scale";

// TRANSFORM ATTRIBUTES
const char* const ATTRIBUTE_TRANSFORM_TO = "to";
const char* const ATTRIBUTE_TRANSFORM_AXIS = "axis";
const char* const ATTRIBUTE_TRANSFORM_FACTOR = "factor";

// OPENGL PRIMITIVE TAGS
const char* const SECTION_PRIMITIVE_RECTANGLE = "rectangle";
const char* const SECTION_PRIMITIVE_TRIANGLE = "triangle";
const char* const SECTION_PRIMITIVE_CYLINDER = "cylinder";
const char* const SECTION_PRIMITIVE_SPHERE = "sphere";
const char* const SECTION_PRIMITIVE_TORUS = "torus";
const char* const SECTION_PRIMITIVE_PLANE = "plane";
const char* const SECTION_PRIMITIVE_PATCH = "patch";
const char* const SECTION_PRIMITIVE_VEHICLE = "vehicle";
const char* const SECTION_PRIMITIVE_WATERLINE = "waterline";
const char* const SECTION_PRIMITIVE_CONTROLPOINT = "controlpoint";

// OPENGL PRIMITIVE ATTRIBUTES
const char* const ATTRIBUTE_PRIMITIVE_XY1 = "xy1";
const char* const ATTRIBUTE_PRIMITIVE_XY2 = "xy2";
const char* const ATTRIBUTE_PRIMITIVE_XYZ1 = "xyz1";
const char* const ATTRIBUTE_PRIMITIVE_XYZ2 = "xyz2";
const char* const ATTRIBUTE_PRIMITIVE_XYZ3 = "xyz3";
const char* const ATTRIBUTE_PRIMITIVE_BASE = "base";
const char* const ATTRIBUTE_PRIMITIVE_TOP = "top";
const char* const ATTRIBUTE_PRIMITIVE_HEIGHT = "height";
const char* const ATTRIBUTE_PRIMITIVE_SLICES = "slices";
const char* const ATTRIBUTE_PRIMITIVE_STACKS = "stacks";
const char* const ATTRIBUTE_PRIMITIVE_RADIUS = "radius";
const char* const ATTRIBUTE_PRIMITIVE_INNER = "inner";
const char* const ATTRIBUTE_PRIMITIVE_OUTER = "outer";
const char* const ATTRIBUTE_PRIMITIVE_LOOPS = "loops";
const char* const ATTRIBUTE_PRIMITIVE_PARTS = "parts";
const char* const ATTRIBUTE_PRIMITIVE_ORDER = "order";
const char* const ATTRIBUTE_PRIMITIVE_PARTSU = "partsU";
const char* const ATTRIBUTE_PRIMITIVE_PARTSV = "partsV";
const char* const ATTRIBUTE_PRIMITIVE_COMPUTE = "compute";
const char* const ATTRIBUTE_PRIMITIVE_X = "x";
const char* const ATTRIBUTE_PRIMITIVE_Y = "y";
const char* const ATTRIBUTE_PRIMITIVE_Z = "z";
const char* const ATTRIBUTE_PRIMITIVE_HEIGHTMAP = "heightmap";
const char* const ATTRIBUTE_PRIMITIVE_TEXTUREMAP = "texturemap";
const char* const ATTRIBUTE_PRIMITIVE_FRAGMENTSHADER = "fragmentshader";
const char* const ATTRIBUTE_PRIMITIVE_VERTEXSHADER = "vertexshader";

// ANIMATION ATTRIBUTES
const char* const SECTION_ANIMATION = "animation";
const char* const ATTRIBUTE_ANIMATION_SPAN = "span";
const char* const ATTRIBUTE_ANIMATION_TYPE = "type";
const char* const VALUE_ANIMATION_LINEAR = "linear";
const char* const ATTRIBUTE_ANIMATION_X = "xx";
const char* const ATTRIBUTE_ANIMATION_Y = "yy";
const char* const ATTRIBUTE_ANIMATION_Z = "zz";

// ERRORS
const std::string SECTION_GLOBALS_ERROR =
        "XML: Globals have already been loaded!";

const std::string ATTRIBUTE_BACKGROUND_ERROR =
        "Globals: Error while parsing the background values.";
const std::string ATTRIBUTE_DRAWMODE_ERROR =
        "Globals: Error while parsing the draw mode value.";
const std::string ATTRIBUTE_SHADING_ERROR =
        "Globals: Error while parsing the shading value.";
const std::string ATTRIBUTE_CULLFACE_ERROR =
        "Globals: Error while parsing the cull face value.";
const std::string ATTRIBUTE_CULLORDER_ERROR =
        "Globals: Error while parsing the cull order value.";

const std::string SECTION_CAMERA_ID_ERROR =
        "Cameras: Error while reading the camera id string.";
const std::string ATTRIBUTE_INITIAL_CAMERA_ERROR =
        "Cameras: Error while reading the initial camera string.";
const std::string ATTRIBUTE_CAMERA_NEAR_ERROR =
        "Cameras: Error while reading the near value.";
const std::string ATTRIBUTE_CAMERA_FAR_ERROR =
        "Cameras: Error while reading the far value.";
const std::string ATTRIBUTE_CAMERA_ANGLE_ERROR =
        "Cameras: Error while reading the angle value.";
const std::string ATTRIBUTE_CAMERA_POSITION_ERROR =
        "Cameras: Error while reading the position values.";
const std::string ATTRIBUTE_CAMERA_TARGET_ERROR =
        "Cameras: Error while reading the target values.";
const std::string ATTRIBUTE_CAMERA_LEFT_ERROR =
        "Cameras: Error while reading the left value.";
const std::string ATTRIBUTE_CAMERA_RIGHT_ERROR =
        "Cameras: Error while reading the right value.";
const std::string ATTRIBUTE_CAMERA_TOP_ERROR =
        "Cameras: Error while reading the top value.";
const std::string ATTRIBUTE_CAMERA_BOTTOM_ERROR =
        "Cameras: Error while reading the bottom value.";

const std::string ATTRIBUTE_LIGHTING_DOUBLESIDED_ERROR =
        "Lighting: Error while reading the double sided string.";
const std::string ATTRIBUTE_LIGHTING_LOCAL_ERROR =
        "Lighting: Error while reading the local string.";
const std::string ATTRIBUTE_LIGHTING_ENABLED_ERROR =
        "Lighting: Error while reading the lighting enabled string.";
const std::string ATTRIBUTE_LIGHTING_G_AMBIENT_ERROR =
        "Lighting: Error while reading the ambient light values.";

const std::string SECTION_LIGHTING_ID_ERROR =
        "Lighting: Error while reading the light id string.";
const std::string SECTION_LIGHTING_ENABLED_ERROR =
        "Lighting: Error while reading the enabled string.";
const std::string ATTRIBUTE_LIGHTING_LOCATION_ERROR =
        "Lighting: Error while reading the location values.";
const std::string ATTRIBUTE_LIGHTING_AMBIENT_ERROR =
        "Lighting: Error while reading the ambient values.";
const std::string ATTRIBUTE_LIGHTING_DIFFUSE_ERROR =
        "Lighting: Error while reading the diffuse values.";
const std::string ATTRIBUTE_LIGHTING_SPECULAR_ERROR =
        "Lighting: Error while reading the specular values.";
const std::string SECTION_LIGHTING_ANGLE_ERROR =
        "Lighting: Error while reading the angle value.";
const std::string ATTRIBUTE_LIGHTING_EXPONENT_ERROR =
        "Lighting: Error while reading the exponent value.";
const std::string ATTRIBUTE_LIGHTING_DIRECTION_ERROR =
        "Lighting: Error while reading the direction value.";

const std::string SECTION_TEXTURE_ID_ERROR =
        "Textures: Error while reading the texture id string.";
const std::string SECTION_TEXTURE_FILE_ERROR =
        "Textures: Error while reading the file name string.";

const std::string SECTION_APPEARANCE_ID_ERROR =
        "Appearances: Error while reading the appearance id.";
const std::string ATTRIBUTE_APPEARANCE_EMISSIVE_ERROR =
        "Appearances: Error while reading the emissive values.";
const std::string ATTRIBUTE_APPEARANCE_AMBIENT_ERROR =
        "Appearances: Error while reading the ambient values.";
const std::string ATTRIBUTE_APPEARANCE_DIFFUSE_ERROR =
        "Appearances: Error while reading the diffuse values.";
const std::string ATTRIBUTE_APPEARANCE_SPECULAR_ERROR =
        "Appearances: Error while reading the specular values.";
const std::string ATTRIBUTE_APPEARANCE_SHININESS_ERROR =
        "Appearances: Error while reading the shininess value.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_ID_ERROR =
        "Appearances: Error while reading the texture id.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S_ERROR =
        "Appearances: Error while reading the texture s length value.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T_ERROR =
        "Appearances: Error while reading the texture t length value.";

const std::string ATTRIBUTE_GRAPH_ROOTID_ERROR =
        "Graph: Error while reading the root id.";
const std::string SECTION_GRAPH_ID_ERROR =
        "Graph: Error while reading the node id.";
const std::string ATTRIBUTE_NODE_DISPLAYLIST_ERROR =
        "Graph: Error while reading the display list.";
const std::string ATTRIBUTE_TRANSFORM_FACTOR_ERROR =
        "Transform: Error while reading the scale factors.";
const std::string ATTRIBUTE_TRANSFORM_AXIS_ERROR =
        "Transform: Error while reading transform axis.";
const std::string ATTRIBUTE_ANGLE_ERROR =
        "Transform: Error while reading the angle value.";
const std::string ATTRIBUTE_TRANSFORM_TO_ERROR =
        "Transform: Error while reading the translation factors.";
const std::string SECTION_GRAPH_APPEARANCE_ID_ERROR =
        "Node: Error while reading the appearance reference id.";
const std::string SECTION_GRAPH_ANIMATION_ID_ERROR =
        "Node: Error while reading the animation reference id.";
const std::string ATTRIBUTE_PRIMITIVE_XY1_ERROR =
        "Rectangle: Error while loading vertice 1.";
const std::string ATTRIBUTE_PRIMITIVE_XY2_ERROR =
        "Rectangle: Error while loading vertice 2.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ1_ERROR =
        "Triangle: Error while loading vertice 1.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ2_ERROR =
        "Triangle: Error while loading vertice 2.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ3_ERROR =
        "Triangle: Error while loading vertice 3.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_BASE_ERROR =
        "Cylinder: Error while loading base.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_TOP_ERROR =
        "Cylinder: Error while loading top.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_HEIGHT_ERROR =
        "Cylinder: Error while loading height.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_SLICES_ERROR =
        "Cylinder: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_STACKS_ERROR =
        "Cylinder: Error while loading stacks.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS_ERROR =
        "Sphere: Error while loading radius.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_SLICES_ERROR =
        "Sphere: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_STACKS_ERROR =
        "Sphere: Error while loading stacks.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_INNER_ERROR =
        "Torus: Error while loading inner value.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_OUTER_ERROR =
        "Torus: Error while loading outer value.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_SLICES_ERROR =
        "Torus: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_LOOPS_ERROR =
        "Torus: Error while loading loops.";
const std::string ATTRIBUTE_PRIMITIVE_PLANE_PARTS_ERROR =
        "Plane: Error while loading parts.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_ORDER_ERROR =
        "Patch: Error while loading order.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_PARTSU_ERROR =
        "Patch: Error while loading partsU.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_PARTSV_ERROR =
        "Patch: Error while loading partsV.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE_ERROR =
        "Patch: Error while loading compute.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_X_ERROR =
        "Patch: Error while reading the x value.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_Y_ERROR =
        "Patch: Error while reading the y value.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_Z_ERROR =
        "Patch: Error while reading the z value.";
const std::string ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP_ERROR =
        "Waterline: Could not read height map.";
const std::string ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP_ERROR =
        "Waterline: Could not read texture map.";
const std::string ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER_ERROR =
        "Waterline: Could not read fragment shader.";
const std::string ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR =
        "Waterline: Could not read vertex shader.";
const std::string SECTION_GRAPH_CHILDREN_ID_ERROR =
        "Children: Could not read node reference id.";

const std::string ATTRIBUTE_ANIMATION_ID_ERROR =
        "Animations: Error while reading the animation id string.";
const std::string ATTRIBUTE_ANIMATION_SPAN_ERROR =
        "Animation: Error while reading animation span.";
const std::string ATTRIBUTE_ANIMATION_TYPE_ERROR =
        "Animation: Error while reading animation type.";
const std::string ATTRIBUTE_ANIMATION_X_ERROR =
        "Animation: Error while reading the x value.";
const std::string ATTRIBUTE_ANIMATION_Y_ERROR =
        "Animation: Error while reading the y value.";
const std::string ATTRIBUTE_ANIMATION_Z_ERROR =
        "Animation: Error while reading the z value.";

} // namespace xml
}

#endif /* Header guard */
