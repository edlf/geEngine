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

typedef const char* const cString;

// DOCUMENT TYPE
cString DocumentType = "yaf";

namespace Blocks {
    cString Node = "node";
    cString NodeReference = "noderef";
    cString RootNodeID = "rootid";
    cString Children = "children";
}

namespace Nodes
{
    cString Graph = "graph";

    namespace Globals {
        cString RootNode = "globals";

        namespace Attributes {
            cString Background = "background";
            cString DrawMode = "drawmode";
            cString Shading = "shading";
            cString CullFace = "cullface";
            cString CullOrder = "cullorder";
        }

        namespace Values {
            cString DrawModeFill = "fill";
            cString DrawmodeLine = "line";
            cString DrawModePoint = "point";
            cString ShadingFlat = "flat";
            cString ShadingGouraud = "gouraud";
            cString CullFaceNone = "none";
            cString CullFaceBack = "back";
            cString CullFaceFront = "front";
            cString CullFaceBoth = "both";
            cString CullOrderCCW = "CCW";
            cString CullOrderCW = "CW";
        }
    }

    namespace Animations {
        cString RootNode = "animations";
        cString NodeReference = "animationref";
        cString Animation = "animation";

        namespace Attributes {
            cString Span = "span";
            cString Type = "type";
            cString x = "xx";
            cString y = "yy";
            cString z = "zz";
        }

        namespace Values {
            cString Linear = "linear";
        }
    }

    namespace Appearances {
        cString RootNode = "appearances";
        cString NodeReference = "appearanceref";

        cString Appearance = "appearance";
        cString Rectangle = "rectangle";
        cString Triangle = "triangle";
        cString Cylinder = "cylinder";
        cString Sphere = "sphere";
        cString Torus = "torus";
        cString Plane = "plane";
        cString Patch = "patch";
        cString Vehicle = "vehicle";
        cString WaterLine = "waterline";
        cString ControlPoint = "controlpoint";

        namespace Attributes {
            cString Angle = "angle";
            cString xy1 = "xy1";
            cString xy2 = "xy2";
            cString xyz1 = "xyz1";
            cString xyz2 = "xyz2";
            cString xyz3 = "xyz3";
            cString Base = "base";
            cString Top = "top";
            cString Height = "height";
            cString Slices = "slices";
            cString Stacks = "stacks";
            cString Radius = "radius";
            cString Inner = "inner";
            cString Outer = "outer";
            cString Loops = "loops";
            cString Parts = "parts";
            cString Order = "order";
            cString PartsU = "partsU";
            cString PartsV = "partsV";
            cString Compute = "compute";
            cString x = "x";
            cString y = "y";
            cString z = "z";
            cString HeightMap = "heightmap";
            cString TextureMap = "texturemap";
            cString FragmentShader = "fragmentshader";
            cString VertexShader = "vertexshader";
            cString Emissive = "emissive";
            cString Ambient = "ambient";
            cString Diffuse = "diffuse";
            cString Specular = "specular";
            cString Shininess = "shininess";
        }
    }

    namespace Cameras {
        cString RootNode = "cameras";
        cString Perspective = "perspective";
        cString Ortho = "ortho";

        namespace Attributes {
            cString InitialCamera = "initial";
            cString Near = "near";
            cString Far = "far";
            cString Position = "pos";
            cString Target = "target";
            cString Angle = "angle";
            cString Left = "left";
            cString Right = "right";
            cString Top = "top";
            cString Bottom = "bottom";
        }
    }

    namespace DisplayLists {
        cString RootNode  = "displaylist";
    }

    namespace Lights {
        cString RootNode = "lighting";
        cString Omni = "omni";
        cString Spot = "spot";

        namespace Attributes {
            cString Angle = "angle";
            cString DoubleSided = "doublesided";
            cString Local = "local";
            cString Location = "location";
            cString Ambient = "ambient";
            cString Diffuse = "diffuse";
            cString Specular = "specular";
            cString Exponent = "exponent";
            cString Direction = "direction";
        }
    }

    namespace Textures {
        cString RootNode = "textures";

        namespace Attributes {
            cString ID = "textureref";
            cString LenghtS = "texlength_s";
            cString LenghtT = "texlength_t";
        }
    }

    namespace Transforms {
        cString RootNode = "transforms";
        cString Translate = "translate";
        cString Rotate = "rotate";
        cString Scale = "scale";

        namespace Attributes {
            cString To = "to";
            cString Axis = "axis";
            cString Factor = "factor";
        }
    }
}

namespace GenericAttributes
{
cString ID = "id";
cString File = "file";
cString Enabled = "enabled";
cString ValueTrue = "true";
cString ValueFalse = "false";
}

namespace Errors
{
const std::string SECTION_GLOBALS =
        "XML: Globals have already been loaded!";

const std::string ATTRIBUTE_BACKGROUND =
        "Globals: Error while parsing the background values.";
const std::string ATTRIBUTE_DRAWMODE =
        "Globals: Error while parsing the draw mode value.";
const std::string ATTRIBUTE_SHADING =
        "Globals: Error while parsing the shading value.";
const std::string ATTRIBUTE_CULLFACE =
        "Globals: Error while parsing the cull face value.";
const std::string ATTRIBUTE_CULLORDER =
        "Globals: Error while parsing the cull order value.";

const std::string SECTION_CAMERA_ID =
        "Cameras: Error while reading the camera id string.";
const std::string ATTRIBUTE_INITIAL_CAMERA =
        "Cameras: Error while reading the initial camera string.";
const std::string ATTRIBUTE_CAMERA_NEAR =
        "Cameras: Error while reading the near value.";
const std::string ATTRIBUTE_CAMERA_FAR =
        "Cameras: Error while reading the far value.";
const std::string ATTRIBUTE_CAMERA_ANGLE =
        "Cameras: Error while reading the angle value.";
const std::string ATTRIBUTE_CAMERA_POSITION =
        "Cameras: Error while reading the position values.";
const std::string ATTRIBUTE_CAMERA_TARGET =
        "Cameras: Error while reading the target values.";
const std::string ATTRIBUTE_CAMERA_LEFT =
        "Cameras: Error while reading the left value.";
const std::string ATTRIBUTE_CAMERA_RIGHT =
        "Cameras: Error while reading the right value.";
const std::string ATTRIBUTE_CAMERA_TOP =
        "Cameras: Error while reading the top value.";
const std::string ATTRIBUTE_CAMERA_BOTTOM =
        "Cameras: Error while reading the bottom value.";

const std::string ATTRIBUTE_LIGHTING_DOUBLESIDED =
        "Lighting: Error while reading the double sided string.";
const std::string ATTRIBUTE_LIGHTING_LOCAL =
        "Lighting: Error while reading the local string.";
const std::string ATTRIBUTE_LIGHTING_ENABLED =
        "Lighting: Error while reading the lighting enabled string.";
const std::string ATTRIBUTE_LIGHTING_G_AMBIENT =
        "Lighting: Error while reading the ambient light values.";

const std::string SECTION_LIGHTING_ID =
        "Lighting: Error while reading the light id string.";
const std::string SECTION_LIGHTING_ENABLED =
        "Lighting: Error while reading the enabled string.";
const std::string ATTRIBUTE_LIGHTING_LOCATION =
        "Lighting: Error while reading the location values.";
const std::string ATTRIBUTE_LIGHTING_AMBIENT =
        "Lighting: Error while reading the ambient values.";
const std::string ATTRIBUTE_LIGHTING_DIFFUSE =
        "Lighting: Error while reading the diffuse values.";
const std::string ATTRIBUTE_LIGHTING_SPECULAR =
        "Lighting: Error while reading the specular values.";
const std::string SECTION_LIGHTING_ANGLE =
        "Lighting: Error while reading the angle value.";
const std::string ATTRIBUTE_LIGHTING_EXPONENT =
        "Lighting: Error while reading the exponent value.";
const std::string ATTRIBUTE_LIGHTING_DIRECTION =
        "Lighting: Error while reading the direction value.";

const std::string SECTION_TEXTURE_ID =
        "Textures: Error while reading the texture id string.";
const std::string SECTION_TEXTURE_FILE =
        "Textures: Error while reading the file name string.";

const std::string SECTION_APPEARANCE_ID =
        "Appearances: Error while reading the appearance id.";
const std::string ATTRIBUTE_APPEARANCE_EMISSIVE =
        "Appearances: Error while reading the emissive values.";
const std::string ATTRIBUTE_APPEARANCE_AMBIENT =
        "Appearances: Error while reading the ambient values.";
const std::string ATTRIBUTE_APPEARANCE_DIFFUSE =
        "Appearances: Error while reading the diffuse values.";
const std::string ATTRIBUTE_APPEARANCE_SPECULAR =
        "Appearances: Error while reading the specular values.";
const std::string ATTRIBUTE_APPEARANCE_SHININESS =
        "Appearances: Error while reading the shininess value.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_ID =
        "Appearances: Error while reading the texture id.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S =
        "Appearances: Error while reading the texture s length value.";
const std::string ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T =
        "Appearances: Error while reading the texture t length value.";

const std::string ATTRIBUTE_GRAPH_ROOTID =
        "Graph: Error while reading the root id.";
const std::string SECTION_GRAPH_ID =
        "Graph: Error while reading the node id.";
const std::string ATTRIBUTE_NODE_DISPLAYLIST =
        "Graph: Error while reading the display list.";
const std::string ATTRIBUTE_TRANSFORM_FACTOR =
        "Transform: Error while reading the scale factors.";
const std::string ATTRIBUTE_TRANSFORM_AXIS =
        "Transform: Error while reading transform axis.";
const std::string ATTRIBUTE_ANGLE =
        "Transform: Error while reading the angle value.";
const std::string ATTRIBUTE_TRANSFORM_TO =
        "Transform: Error while reading the translation factors.";
const std::string SECTION_GRAPH_APPEARANCE_ID =
        "Node: Error while reading the appearance reference id.";
const std::string SECTION_GRAPH_ANIMATION_ID =
        "Node: Error while reading the animation reference id.";
const std::string ATTRIBUTE_PRIMITIVE_XY1 =
        "Rectangle: Error while loading vertice 1.";
const std::string ATTRIBUTE_PRIMITIVE_XY2 =
        "Rectangle: Error while loading vertice 2.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ1 =
        "Triangle: Error while loading vertice 1.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ2 =
        "Triangle: Error while loading vertice 2.";
const std::string ATTRIBUTE_PRIMITIVE_XYZ3 =
        "Triangle: Error while loading vertice 3.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_BASE =
        "Cylinder: Error while loading base.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_TOP =
        "Cylinder: Error while loading top.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_HEIGHT =
        "Cylinder: Error while loading height.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_SLICES =
        "Cylinder: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_CYL_STACKS =
        "Cylinder: Error while loading stacks.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS =
        "Sphere: Error while loading radius.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_SLICES =
        "Sphere: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_SPHERE_STACKS =
        "Sphere: Error while loading stacks.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_INNER =
        "Torus: Error while loading inner value.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_OUTER =
        "Torus: Error while loading outer value.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_SLICES =
        "Torus: Error while loading slices.";
const std::string ATTRIBUTE_PRIMITIVE_TORUS_LOOPS =
        "Torus: Error while loading loops.";
const std::string ATTRIBUTE_PRIMITIVE_PLANE_PARTS =
        "Plane: Error while loading parts.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_ORDER =
        "Patch: Error while loading order.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_PARTSU =
        "Patch: Error while loading partsU.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_PARTSV =
        "Patch: Error while loading partsV.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE =
        "Patch: Error while loading compute.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_X =
        "Patch: Error while reading the x value.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_Y =
        "Patch: Error while reading the y value.";
const std::string ATTRIBUTE_PRIMITIVE_PATCH_Z =
        "Patch: Error while reading the z value.";
const std::string ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP =
        "Waterline: Could not read height map.";
const std::string ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP =
        "Waterline: Could not read texture map.";
const std::string ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER =
        "Waterline: Could not read fragment shader.";
const std::string ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR =
        "Waterline: Could not read vertex shader.";
const std::string SECTION_GRAPH_CHILDREN_ID =
        "Children: Could not read node reference id.";

const std::string ATTRIBUTE_ANIMATION_ID =
        "Animations: Error while reading the animation id string.";
const std::string ATTRIBUTE_ANIMATION_SPAN =
        "Animation: Error while reading animation span.";
const std::string ATTRIBUTE_ANIMATION_TYPE =
        "Animation: Error while reading animation type.";
const std::string ATTRIBUTE_ANIMATION_X =
        "Animation: Error while reading the x value.";
const std::string ATTRIBUTE_ANIMATION_Y =
        "Animation: Error while reading the y value.";
const std::string ATTRIBUTE_ANIMATION_Z =
        "Animation: Error while reading the z value.";
}


} // namespace xml
}

#endif /* Header guard */
