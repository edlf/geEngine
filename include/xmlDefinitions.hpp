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

typedef const std::string cString;

// DOCUMENT TYPE
cString DocumentType = "yaf";

namespace Blocks {
    cString Graph = "graph";
    cString Node = "node";
    cString NodeReference = "noderef";
    cString Children = "children";

    namespace Errors {
        cString InvalidNodeRefID = "Children: Could not read node reference id.";
    }
}

namespace Nodes
{
    cString RootNodeID = "rootid";

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

        namespace Errors {
            cString AlreadyLoaded = "XML: Globals have already been loaded!";
            cString InvalidBackgrond = "Globals: Error while parsing the background values.";
            cString InvalidDrawMode = "Globals: Error while parsing the draw mode value.";
            cString InvalidShading = "Globals: Error while parsing the shading value.";
            cString InvalidCullFace = "Globals: Error while parsing the cull face value.";
            cString InvalidCullOrder = "Globals: Error while parsing the cull order value.";
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

        namespace Errors {
            cString InvalidID = "Animations: Error while reading the animation id string.";
            cString InvalidSpan = "Animation: Error while reading animation span.";
            cString InvalidType = "Animation: Error while reading animation type.";
            cString InvalidX = "Animation: Error while reading the x value.";
            cString InvalidY = "Animation: Error while reading the y value.";
            cString InvalidZ = "Animation: Error while reading the z value.";
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

        namespace Errors {
            cString InvalidAppearanceID = "Appearances: Error while reading the appearance id.";
            cString InvalidEmissive = "Appearances: Error while reading the emissive values.";
            cString InvalidAmbient = "Appearances: Error while reading the ambient values.";
            cString InvalidDiffuse = "Appearances: Error while reading the diffuse values.";
            cString InvalidSpecular = "Appearances: Error while reading the specular values.";
            cString InvalidShininess = "Appearances: Error while reading the shininess value.";
            cString InvalidTextureID = "Appearances: Error while reading the texture id.";
            cString InvalidTextureLenghtS = "Appearances: Error while reading the texture s length value.";
            cString InvalidTextureLenghtT = "Appearances: Error while reading the texture t length value.";
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

        namespace Errors {
            cString InvalidCameraID = "Cameras: Error while reading the camera id string.";
            cString InvalidInitialCamera = "Cameras: Error while reading the initial camera string.";
            cString InvalidNear = "Cameras: Error while reading the near value.";
            cString InvalidFar = "Cameras: Error while reading the far value.";
            cString InvalidAngle = "Cameras: Error while reading the angle value.";
            cString InvalidPosition = "Cameras: Error while reading the position values.";
            cString InvalidTarget = "Cameras: Error while reading the target values.";
            cString InvalidLeft = "Cameras: Error while reading the left value.";
            cString InvalidRight = "Cameras: Error while reading the right value.";
            cString InvalidTop = "Cameras: Error while reading the top value.";
            cString InvalidBottom = "Cameras: Error while reading the bottom value.";
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

        namespace Errors {
            cString InvalidTextureID = "Textures: Error while reading the texture id string.";
            cString InvalidFileName = "Textures: Error while reading the file name string.";
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
cString ATTRIBUTE_LIGHTING_DOUBLESIDED =
        "Lighting: Error while reading the double sided string.";
cString ATTRIBUTE_LIGHTING_LOCAL =
        "Lighting: Error while reading the local string.";
cString ATTRIBUTE_LIGHTING_ENABLED =
        "Lighting: Error while reading the lighting enabled string.";
cString ATTRIBUTE_LIGHTING_G_AMBIENT =
        "Lighting: Error while reading the ambient light values.";

cString SECTION_LIGHTING_ID =
        "Lighting: Error while reading the light id string.";
cString SECTION_LIGHTING_ENABLED =
        "Lighting: Error while reading the enabled string.";
cString ATTRIBUTE_LIGHTING_LOCATION =
        "Lighting: Error while reading the location values.";
cString ATTRIBUTE_LIGHTING_AMBIENT =
        "Lighting: Error while reading the ambient values.";
cString ATTRIBUTE_LIGHTING_DIFFUSE =
        "Lighting: Error while reading the diffuse values.";
cString ATTRIBUTE_LIGHTING_SPECULAR =
        "Lighting: Error while reading the specular values.";
cString SECTION_LIGHTING_ANGLE =
        "Lighting: Error while reading the angle value.";
cString ATTRIBUTE_LIGHTING_EXPONENT =
        "Lighting: Error while reading the exponent value.";
cString ATTRIBUTE_LIGHTING_DIRECTION =
        "Lighting: Error while reading the direction value.";
cString ATTRIBUTE_GRAPH_ROOTID =
        "Graph: Error while reading the root id.";
cString SECTION_GRAPH_ID =
        "Graph: Error while reading the node id.";
cString ATTRIBUTE_NODE_DISPLAYLIST =
        "Graph: Error while reading the display list.";
cString ATTRIBUTE_TRANSFORM_FACTOR =
        "Transform: Error while reading the scale factors.";
cString ATTRIBUTE_TRANSFORM_AXIS =
        "Transform: Error while reading transform axis.";
cString ATTRIBUTE_ANGLE =
        "Transform: Error while reading the angle value.";
cString ATTRIBUTE_TRANSFORM_TO =
        "Transform: Error while reading the translation factors.";
cString SECTION_GRAPH_APPEARANCE_ID =
        "Node: Error while reading the appearance reference id.";
cString SECTION_GRAPH_ANIMATION_ID =
        "Node: Error while reading the animation reference id.";
cString ATTRIBUTE_PRIMITIVE_XY1 =
        "Rectangle: Error while loading vertice 1.";
cString ATTRIBUTE_PRIMITIVE_XY2 =
        "Rectangle: Error while loading vertice 2.";
cString ATTRIBUTE_PRIMITIVE_XYZ1 =
        "Triangle: Error while loading vertice 1.";
cString ATTRIBUTE_PRIMITIVE_XYZ2 =
        "Triangle: Error while loading vertice 2.";
cString ATTRIBUTE_PRIMITIVE_XYZ3 =
        "Triangle: Error while loading vertice 3.";
cString ATTRIBUTE_PRIMITIVE_CYL_BASE =
        "Cylinder: Error while loading base.";
cString ATTRIBUTE_PRIMITIVE_CYL_TOP =
        "Cylinder: Error while loading top.";
cString ATTRIBUTE_PRIMITIVE_CYL_HEIGHT =
        "Cylinder: Error while loading height.";
cString ATTRIBUTE_PRIMITIVE_CYL_SLICES =
        "Cylinder: Error while loading slices.";
cString ATTRIBUTE_PRIMITIVE_CYL_STACKS =
        "Cylinder: Error while loading stacks.";
cString ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS =
        "Sphere: Error while loading radius.";
cString ATTRIBUTE_PRIMITIVE_SPHERE_SLICES =
        "Sphere: Error while loading slices.";
cString ATTRIBUTE_PRIMITIVE_SPHERE_STACKS =
        "Sphere: Error while loading stacks.";
cString ATTRIBUTE_PRIMITIVE_TORUS_INNER =
        "Torus: Error while loading inner value.";
cString ATTRIBUTE_PRIMITIVE_TORUS_OUTER =
        "Torus: Error while loading outer value.";
cString ATTRIBUTE_PRIMITIVE_TORUS_SLICES =
        "Torus: Error while loading slices.";
cString ATTRIBUTE_PRIMITIVE_TORUS_LOOPS =
        "Torus: Error while loading loops.";
cString ATTRIBUTE_PRIMITIVE_PLANE_PARTS =
        "Plane: Error while loading parts.";
cString ATTRIBUTE_PRIMITIVE_PATCH_ORDER =
        "Patch: Error while loading order.";
cString ATTRIBUTE_PRIMITIVE_PATCH_PARTSU =
        "Patch: Error while loading partsU.";
cString ATTRIBUTE_PRIMITIVE_PATCH_PARTSV =
        "Patch: Error while loading partsV.";
cString ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE =
        "Patch: Error while loading compute.";
cString ATTRIBUTE_PRIMITIVE_PATCH_X =
        "Patch: Error while reading the x value.";
cString ATTRIBUTE_PRIMITIVE_PATCH_Y =
        "Patch: Error while reading the y value.";
cString ATTRIBUTE_PRIMITIVE_PATCH_Z =
        "Patch: Error while reading the z value.";
cString ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP =
        "Waterline: Could not read height map.";
cString ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP =
        "Waterline: Could not read texture map.";
cString ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER =
        "Waterline: Could not read fragment shader.";
cString ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR =
        "Waterline: Could not read vertex shader.";
}


} // namespace xml
}

#endif /* Header guard */
