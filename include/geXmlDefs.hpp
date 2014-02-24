/*
 * Eduardo Fernandes
 *
 * Yaf file definitions for parsing.
 * Error messages for the parser.
 */

#ifndef GEXMLDEFS_HPP_
#define GEXMLDEFS_HPP_

// DOCUMENT TYPE
#define XML_DOCUMENT_TYPE                                 "yaf"

// MAIN SECTION TAGS
#define XML_SECTION_GLOBALS                               "globals"
#define XML_SECTION_CAMERAS                               "cameras"
#define XML_SECTION_LIGHTS                                "lighting"
#define XML_SECTION_TEXTURES                              "textures"
#define XML_SECTION_APPEARANCES                           "appearances"
#define XML_SECTION_GRAPH                                 "graph"
#define XML_SECTION_ANIMATIONS                            "animations"

// BLOCK TAGS
#define XML_BLOCK_NODE                                    "node"
#define XML_BLOCK_TRANSFORMS	                          "transforms"
#define XML_BLOCK_CHILDREN                                "children"

// REFERENCE TAGS
#define XML_REFERENCE_APPEARANCE                          "appearanceref"
#define XML_REFERENCE_NODE                                "noderef"
#define XML_REFERENCE_ANIMATION                           "animationref"

// COMMON ATTRIBUTES
#define XML_ATTRIBUTE_ID                                  "id"
#define XML_ATTRIBUTE_FILE                                "file"
#define XML_ATTRIBUTE_ENABLED                             "enabled"
#define XML_ATTRIBUTE_VALUE_TRUE                          "true"
#define XML_ATTRIBUTE_VALUE_FALSE                         "false"
#define XML_ATTRIBUTE_ANGLE                               "angle"

// GLOBAL ATTRIBUTES
#define XML_ATTRIBUTE_BACKGROUND                          "background"

#define XML_ATTRIBUTE_DRAWMODE                            "drawmode"
#define XML_ATTRIBUTE_VALUE_DRAWMODE_FILL                 "fill"
#define XML_ATTRIBUTE_VALUE_DRAWMODE_LINE                 "line"
#define XML_ATTRIBUTE_VALUE_DRAWMODE_POINT                "point"

#define XML_ATTRIBUTE_SHADING                             "shading"
#define XML_ATTRIBUTE_VALUE_SHADING_FLAT                  "flat"
#define XML_ATTRIBUTE_VALUE_SHADING_GOURAUD               "gouraud"

#define XML_ATTRIBUTE_CULLFACE                            "cullface"
#define XML_ATTRIBUTE_VALUE_CULLFACE_NONE                 "none"
#define XML_ATTRIBUTE_VALUE_CULLFACE_BACK                 "back"
#define XML_ATTRIBUTE_VALUE_CULLFACE_FRONT                "front"
#define XML_ATTRIBUTE_VALUE_CULLFACE_BOTH                 "both"

#define XML_ATTRIBUTE_CULLORDER                           "cullorder"
#define XML_ATTRIBUTE_VALUE_CULLORDER_CCW                 "CCW"
#define XML_ATTRIBUTE_VALUE_CULLORDER_CW                  "CW"

// CAMERA TAGS
#define XML_ATTRIBUTE_INITIAL_CAMERA                      "initial"
#define XML_SECTION_CAMERA_PERSPECTIVE                    "perspective"
#define XML_SECTION_CAMERA_ORTHO                          "ortho"

// CAMERA ATTRIBUTES (COMMON)
#define XML_ATTRIBUTE_CAMERA_NEAR                         "near"
#define XML_ATTRIBUTE_CAMERA_FAR                          "far"

// PERSPECTIVE CAMERA ATTRIBUTES
#define XML_ATTRIBUTE_CAMERA_POSITION                     "pos"
#define XML_ATTRIBUTE_CAMERA_TARGET                       "target"
#define XML_ATTRIBUTE_CAMERA_ANGLE                        "angle"

// ORTHO CAMERA ATTRIBUTES
#define XML_ATTRIBUTE_CAMERA_LEFT                         "left"
#define XML_ATTRIBUTE_CAMERA_RIGHT                        "right"
#define XML_ATTRIBUTE_CAMERA_TOP                          "top"
#define XML_ATTRIBUTE_CAMERA_BOTTOM                       "bottom"

// LIGHTING TAGS
#define XML_SECTION_LIGHTING_OMNI                         "omni"
#define XML_SECTION_LIGHTING_SPOT                         "spot"

// LIGHTING ATTRIBUTES
#define XML_ATTRIBUTE_LIGHTING_DOUBLESIDED                "doublesided"
#define XML_ATTRIBUTE_LIGHTING_LOCAL                      "local"

// OMNI LIGHTING ATTRIBUTES
#define XML_ATTRIBUTE_LIGHTING_LOCATION                   "location"
#define XML_ATTRIBUTE_LIGHTING_AMBIENT                    "ambient"
#define XML_ATTRIBUTE_LIGHTING_DIFFUSE                    "diffuse"
#define XML_ATTRIBUTE_LIGHTING_SPECULAR                   "specular"

// SPOT LIGHTING ATTRIBUTES
#define XML_ATTRIBUTE_LIGHTING_EXPONENT                   "exponent"
#define XML_ATTRIBUTE_LIGHTING_DIRECTION                  "direction"

// APPEARANCE TAG
#define XML_SECTION_APPEARANCE                            "appearance"

// APPEARANCE ATTRIBUTES
#define XML_ATTRIBUTE_APPEARANCE_EMISSIVE                 "emissive"
#define XML_ATTRIBUTE_APPEARANCE_AMBIENT                  "ambient"
#define XML_ATTRIBUTE_APPEARANCE_DIFFUSE                  "diffuse"
#define XML_ATTRIBUTE_APPEARANCE_SPECULAR                 "specular"
#define XML_ATTRIBUTE_APPEARANCE_SHININESS                "shininess"

// APPEARANCE ATTRIBUTES (OPTIONAL)
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_ID               "textureref"
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S         "texlength_s"
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T         "texlength_t"

// NODE ATTRIBUTES
#define XML_ATTRIBUTE_NODE_DISPLAYLIST                    "displaylist"

// GRAPH ATTRIBUTES
#define XML_ATTRIBUTE_GRAPH_ROOTID                        "rootid"

// TRANSFORM TAG
#define XML_SECTION_TRANSFORM_TRANSLATE                   "translate"
#define XML_SECTION_TRANSFORM_ROTATE                      "rotate"
#define XML_SECTION_TRANSFORM_SCALE                       "scale"

// TRANSFORM ATTRIBUTES
#define XML_ATTRIBUTE_TRANSFORM_TO                        "to"
#define XML_ATTRIBUTE_TRANSFORM_AXIS                      "axis"
#define XML_ATTRIBUTE_TRANSFORM_FACTOR                    "factor"

// OPENGL PRIMITIVE TAGS
#define XML_SECTION_PRIMITIVE_RECTANGLE                   "rectangle"
#define XML_SECTION_PRIMITIVE_TRIANGLE                    "triangle"
#define XML_SECTION_PRIMITIVE_CYLINDER                    "cylinder"
#define XML_SECTION_PRIMITIVE_SPHERE                      "sphere"
#define XML_SECTION_PRIMITIVE_TORUS                       "torus"
#define XML_SECTION_PRIMITIVE_PLANE                       "plane"
#define XML_SECTION_PRIMITIVE_PATCH                       "patch"
#define XML_SECTION_PRIMITIVE_VEHICLE                     "vehicle"
#define XML_SECTION_PRIMITIVE_WATERLINE                   "waterline"
#define XML_SECTION_PRIMITIVE_CONTROLPOINT                "controlpoint"

// OPENGL PRIMITIVE ATTRIBUTES
#define XML_ATTRIBUTE_PRIMITIVE_XY1                       "xy1"
#define XML_ATTRIBUTE_PRIMITIVE_XY2                       "xy2"
#define XML_ATTRIBUTE_PRIMITIVE_XYZ1                      "xyz1"
#define XML_ATTRIBUTE_PRIMITIVE_XYZ2                      "xyz2"
#define XML_ATTRIBUTE_PRIMITIVE_XYZ3                      "xyz3"
#define XML_ATTRIBUTE_PRIMITIVE_BASE                      "base"
#define XML_ATTRIBUTE_PRIMITIVE_TOP                       "top"
#define XML_ATTRIBUTE_PRIMITIVE_HEIGHT                    "height"
#define XML_ATTRIBUTE_PRIMITIVE_SLICES                    "slices"
#define XML_ATTRIBUTE_PRIMITIVE_STACKS                    "stacks"
#define XML_ATTRIBUTE_PRIMITIVE_RADIUS                    "radius"
#define XML_ATTRIBUTE_PRIMITIVE_INNER                     "inner"
#define XML_ATTRIBUTE_PRIMITIVE_OUTER                     "outer"
#define XML_ATTRIBUTE_PRIMITIVE_LOOPS                     "loops"
#define XML_ATTRIBUTE_PRIMITIVE_PARTS                     "parts"
#define XML_ATTRIBUTE_PRIMITIVE_ORDER                     "order"
#define XML_ATTRIBUTE_PRIMITIVE_PARTSU                    "partsU"
#define XML_ATTRIBUTE_PRIMITIVE_PARTSV                    "partsV"
#define XML_ATTRIBUTE_PRIMITIVE_COMPUTE                   "compute"
#define XML_ATTRIBUTE_PRIMITIVE_X                         "x"
#define XML_ATTRIBUTE_PRIMITIVE_Y                         "y"
#define XML_ATTRIBUTE_PRIMITIVE_Z                         "z"
#define XML_ATTRIBUTE_PRIMITIVE_HEIGHTMAP                 "heightmap"
#define XML_ATTRIBUTE_PRIMITIVE_TEXTUREMAP                "texturemap"
#define XML_ATTRIBUTE_PRIMITIVE_FRAGMENTSHADER            "fragmentshader"
#define XML_ATTRIBUTE_PRIMITIVE_VERTEXSHADER              "vertexshader"

// ANIMATION ATTRIBUTES
#define XML_SECTION_ANIMATION                             "animation"
#define XML_ATTRIBUTE_ANIMATION_SPAN                      "span"
#define XML_ATTRIBUTE_ANIMATION_TYPE                      "type"
#define XML_VALUE_ANIMATION_LINEAR                        "linear"
#define XML_ATTRIBUTE_ANIMATION_X                         "xx"
#define XML_ATTRIBUTE_ANIMATION_Y                         "yy"
#define XML_ATTRIBUTE_ANIMATION_Z                         "zz"

// ERRORS
#define XML_SECTION_GLOBALS_ERROR                         "XML: Globals have already been loaded!"

#define XML_ATTRIBUTE_BACKGROUND_ERROR                    "Globals: Error while parsing the background values."
#define XML_ATTRIBUTE_DRAWMODE_ERROR                      "Globals: Error while parsing the draw mode value."
#define XML_ATTRIBUTE_SHADING_ERROR                       "Globals: Error while parsing the shading value."
#define XML_ATTRIBUTE_CULLFACE_ERROR                      "Globals: Error while parsing the cull face value."
#define XML_ATTRIBUTE_CULLORDER_ERROR                     "Globals: Error while parsing the cull order value."

#define XML_SECTION_CAMERA_ID_ERROR                       "Cameras: Error while reading the camera id string."
#define XML_ATTRIBUTE_INITIAL_CAMERA_ERROR                "Cameras: Error while reading the initial camera string."
#define XML_ATTRIBUTE_CAMERA_NEAR_ERROR                   "Cameras: Error while reading the near value."
#define XML_ATTRIBUTE_CAMERA_FAR_ERROR                    "Cameras: Error while reading the far value."
#define XML_ATTRIBUTE_CAMERA_ANGLE_ERROR                  "Cameras: Error while reading the angle value."
#define XML_ATTRIBUTE_CAMERA_POSITION_ERROR               "Cameras: Error while reading the position values."
#define XML_ATTRIBUTE_CAMERA_TARGET_ERROR                 "Cameras: Error while reading the target values."
#define XML_ATTRIBUTE_CAMERA_LEFT_ERROR                   "Cameras: Error while reading the left value."
#define XML_ATTRIBUTE_CAMERA_RIGHT_ERROR                  "Cameras: Error while reading the right value."
#define XML_ATTRIBUTE_CAMERA_TOP_ERROR                    "Cameras: Error while reading the top value."
#define XML_ATTRIBUTE_CAMERA_BOTTOM_ERROR                 "Cameras: Error while reading the bottom value."

#define XML_ATTRIBUTE_LIGHTING_DOUBLESIDED_ERROR          "Lighting: Error while reading the double sided string."
#define XML_ATTRIBUTE_LIGHTING_LOCAL_ERROR                "Lighting: Error while reading the local string."
#define XML_ATTRIBUTE_LIGHTING_ENABLED_ERROR              "Lighting: Error while reading the lighting enabled string."
#define XML_ATTRIBUTE_LIGHTING_G_AMBIENT_ERROR            "Lighting: Error while reading the ambient light values."

#define XML_SECTION_LIGHTING_ID_ERROR                     "Lighting: Error while reading the light id string."
#define XML_SECTION_LIGHTING_ENABLED_ERROR                "Lighting: Error while reading the enabled string."
#define XML_ATTRIBUTE_LIGHTING_LOCATION_ERROR             "Lighting: Error while reading the location values."
#define XML_ATTRIBUTE_LIGHTING_AMBIENT_ERROR              "Lighting: Error while reading the ambient values."
#define XML_ATTRIBUTE_LIGHTING_DIFFUSE_ERROR              "Lighting: Error while reading the diffuse values."
#define XML_ATTRIBUTE_LIGHTING_SPECULAR_ERROR             "Lighting: Error while reading the specular values."
#define XML_SECTION_LIGHTING_ANGLE_ERROR                  "Lighting: Error while reading the angle value."
#define XML_ATTRIBUTE_LIGHTING_EXPONENT_ERROR             "Lighting: Error while reading the exponent value."
#define XML_ATTRIBUTE_LIGHTING_DIRECTION_ERROR            "Lighting: Error while reading the direction value."

#define XML_SECTION_TEXTURE_ID_ERROR                      "Textures: Error while reading the texture id string."
#define XML_SECTION_TEXTURE_FILE_ERROR                    "Textures: Error while reading the file name string."

#define XML_SECTION_APPEARANCE_ID_ERROR                   "Appearances: Error while reading the appearance id."
#define XML_ATTRIBUTE_APPEARANCE_EMISSIVE_ERROR           "Appearances: Error while reading the emissive values."
#define XML_ATTRIBUTE_APPEARANCE_AMBIENT_ERROR            "Appearances: Error while reading the ambient values."
#define XML_ATTRIBUTE_APPEARANCE_DIFFUSE_ERROR            "Appearances: Error while reading the diffuse values."
#define XML_ATTRIBUTE_APPEARANCE_SPECULAR_ERROR           "Appearances: Error while reading the specular values."
#define XML_ATTRIBUTE_APPEARANCE_SHININESS_ERROR          "Appearances: Error while reading the shininess value."
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_ID_ERROR         "Appearances: Error while reading the texture id."
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_S_ERROR   "Appearances: Error while reading the texture s length value."
#define XML_ATTRIBUTE_APPEARANCE_TEXTURE_LENGHT_T_ERROR   "Appearances: Error while reading the texture t length value."

#define XML_ATTRIBUTE_GRAPH_ROOTID_ERROR                  "Graph: Error while reading the root id."
#define XML_SECTION_GRAPH_ID_ERROR                        "Graph: Error while reading the node id."
#define XML_ATTRIBUTE_NODE_DISPLAYLIST_ERROR              "Graph: Error while reading the display list."
#define XML_ATTRIBUTE_TRANSFORM_FACTOR_ERROR              "Transform: Error while reading the scale factors."
#define XML_ATTRIBUTE_TRANSFORM_AXIS_ERROR                "Transform: Error while reading transform axis."
#define XML_ATTRIBUTE_ANGLE_ERROR                         "Transform: Error while reading the angle value."
#define XML_ATTRIBUTE_TRANSFORM_TO_ERROR                  "Transform: Error while reading the translation factors."
#define XML_SECTION_GRAPH_APPEARANCE_ID_ERROR             "Node: Error while reading the appearance reference id."
#define XML_SECTION_GRAPH_ANIMATION_ID_ERROR              "Node: Error while reading the animation reference id."
#define XML_ATTRIBUTE_PRIMITIVE_XY1_ERROR                 "Rectangle: Error while loading vertice 1."
#define XML_ATTRIBUTE_PRIMITIVE_XY2_ERROR                 "Rectangle: Error while loading vertice 2."
#define XML_ATTRIBUTE_PRIMITIVE_XYZ1_ERROR                "Triangle: Error while loading vertice 1."
#define XML_ATTRIBUTE_PRIMITIVE_XYZ2_ERROR                "Triangle: Error while loading vertice 2."
#define XML_ATTRIBUTE_PRIMITIVE_XYZ3_ERROR                "Triangle: Error while loading vertice 3."
#define XML_ATTRIBUTE_PRIMITIVE_CYL_BASE_ERROR            "Cylinder: Error while loading base."
#define XML_ATTRIBUTE_PRIMITIVE_CYL_TOP_ERROR             "Cylinder: Error while loading top."
#define XML_ATTRIBUTE_PRIMITIVE_CYL_HEIGHT_ERROR          "Cylinder: Error while loading height."
#define XML_ATTRIBUTE_PRIMITIVE_CYL_SLICES_ERROR          "Cylinder: Error while loading slices."
#define XML_ATTRIBUTE_PRIMITIVE_CYL_STACKS_ERROR          "Cylinder: Error while loading stacks."
#define XML_ATTRIBUTE_PRIMITIVE_SPHERE_RADIUS_ERROR       "Sphere: Error while loading radius."
#define XML_ATTRIBUTE_PRIMITIVE_SPHERE_SLICES_ERROR       "Sphere: Error while loading slices."
#define XML_ATTRIBUTE_PRIMITIVE_SPHERE_STACKS_ERROR       "Sphere: Error while loading stacks."
#define XML_ATTRIBUTE_PRIMITIVE_TORUS_INNER_ERROR         "Torus: Error while loading inner value."
#define XML_ATTRIBUTE_PRIMITIVE_TORUS_OUTER_ERROR         "Torus: Error while loading outer value."
#define XML_ATTRIBUTE_PRIMITIVE_TORUS_SLICES_ERROR        "Torus: Error while loading slices."
#define XML_ATTRIBUTE_PRIMITIVE_TORUS_LOOPS_ERROR         "Torus: Error while loading loops."
#define XML_ATTRIBUTE_PRIMITIVE_PLANE_PARTS_ERROR         "Plane: Error while loading parts."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_ORDER_ERROR         "Patch: Error while loading order."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_PARTSU_ERROR        "Patch: Error while loading partsU."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_PARTSV_ERROR        "Patch: Error while loading partsV."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_COMPUTE_ERROR       "Patch: Error while loading compute."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_X_ERROR             "Patch: Error while reading the x value."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_Y_ERROR             "Patch: Error while reading the y value."
#define XML_ATTRIBUTE_PRIMITIVE_PATCH_Z_ERROR             "Patch: Error while reading the z value."
#define XML_ATTRIBUTE_PRIMITIVE_WL_HEIGHTMAP_ERROR        "Waterline: Could not read height map."
#define XML_ATTRIBUTE_PRIMITIVE_WL_TEXTUREMAP_ERROR       "Waterline: Could not read texture map."
#define XML_ATTRIBUTE_PRIMITIVE_WL_FRAGMENTSHADER_ERROR   "Waterline: Could not read fragment shader."
#define XML_ATTRIBUTE_PRIMITIVE_WL_VERTEXSHADER_ERRROR    "Waterline: Could not read vertex shader."
#define XML_SECTION_GRAPH_CHILDREN_ID_ERROR               "Children: Could not read node reference id."

#define XML_ATTRIBUTE_ANIMATION_ID_ERROR                  "Animations: Error while reading the animation id string."
#define XML_ATTRIBUTE_ANIMATION_SPAN_ERROR                "Animation: Error while reading animation span."
#define XML_ATTRIBUTE_ANIMATION_TYPE_ERROR                "Animation: Error while reading animation type."
#define XML_ATTRIBUTE_ANIMATION_X_ERROR                   "Animation: Error while reading the x value."
#define XML_ATTRIBUTE_ANIMATION_Y_ERROR                   "Animation: Error while reading the y value."
#define XML_ATTRIBUTE_ANIMATION_Z_ERROR                   "Animation: Error while reading the z value."

#endif /* GEXMLDEFS_HPP_ */

