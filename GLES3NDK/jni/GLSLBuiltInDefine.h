//
//  GLSLBuiltInDefine.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef GLSLBuiltInDefine_h
#define GLSLBuiltInDefine_h

/**
 @name Built Shader types
 @{
 */
/** ETC1 ALPHA supports for 2d */
static const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR;
static const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR_NO_MVP;

static const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY;
static const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY_NO_MVP;

/**Built in shader for 2d. Support Position, Texture and Color vertex attribute.*/
static const char* SHADER_NAME_POSITION_TEXTURE_COLOR;
/**Built in shader for 2d. Support Position, Texture and Color vertex attribute, but without multiply vertex by MVP matrix.*/
static const char* SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP;
/**Built in shader for 2d. Support Position, Texture vertex attribute, but include alpha test.*/
static const char* SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST;
/**Built in shader for 2d. Support Position, Texture and Color vertex attribute, include alpha test and without multiply vertex by MVP matrix.*/
static const char* SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV;
/**Built in shader for 2d. Support Position, Color vertex attribute.*/
static const char* SHADER_NAME_POSITION_COLOR;
/**Built in shader for 2d. Support Position, Color, Texture vertex attribute. texture coordinate will used as point size.*/
static const char* SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE;
/**Built in shader for 2d. Support Position, Color vertex attribute, without multiply vertex by MVP matrix.*/
static const char* SHADER_NAME_POSITION_COLOR_NO_MVP;
/**Built in shader for 2d. Support Position, Texture vertex attribute.*/
static const char* SHADER_NAME_POSITION_TEXTURE;
/**Built in shader for 2d. Support Position, Texture vertex attribute. with a specified uniform as color*/
static const char* SHADER_NAME_POSITION_TEXTURE_U_COLOR;
/**Built in shader for 2d. Support Position, Texture and Color vertex attribute. but alpha will be the multiplication of color attribute and texture.*/
static const char* SHADER_NAME_POSITION_TEXTURE_A8_COLOR;
/**Built in shader for 2d. Support Position, with color specified by a uniform.*/
static const char* SHADER_NAME_POSITION_U_COLOR;
/**Built in shader for draw a sector with 90 degrees with center at bottom left point.*/
static const char* SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR;

/**Built in shader for ui effects */
static const char* SHADER_NAME_POSITION_GRAYSCALE;
/** @{
 Built in shader for label and label with effects.
 */
static const char* SHADER_NAME_LABEL_NORMAL;
static const char* SHADER_NAME_LABEL_OUTLINE;
static const char* SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL;
static const char* SHADER_NAME_LABEL_DISTANCEFIELD_GLOW;

/**Built in shader used for 3D, support Position vertex attribute, with color specified by a uniform.*/
static const char* SHADER_3D_POSITION;
/**Built in shader used for 3D, support Position and Texture vertex attribute, with color specified by a uniform.*/
static const char* SHADER_3D_POSITION_TEXTURE;
/**
 Built in shader used for 3D, support Position (Skeletal animation by hardware skin) and Texture vertex attribute,
 with color specified by a uniform.
 */
static const char* SHADER_3D_SKINPOSITION_TEXTURE;
/**
 Built in shader used for 3D, support Position and Normal vertex attribute, used in lighting. with color specified by a uniform.
 */
static const char* SHADER_3D_POSITION_NORMAL;
/**
 Built in shader used for 3D, support Position, Normal, Texture vertex attribute, used in lighting. with color specified by a uniform.
 */
static const char* SHADER_3D_POSITION_NORMAL_TEXTURE;
/**
 Built in shader used for 3D, support Position(skeletal animation by hardware skin), Normal, Texture vertex attribute,
 used in lighting. with color specified by a uniform.
 */
static const char* SHADER_3D_SKINPOSITION_NORMAL_TEXTURE;
/**
 Built in shader used for 3D, support Position, Bumped Normal, Texture vertex attribute, used in lighting. with color specified by a uniform.
 */
static const char* SHADER_3D_POSITION_BUMPEDNORMAL_TEXTURE;
/**
 Built in shader used for 3D, support Position(skeletal animation by hardware skin), Bumped Normal, Texture vertex attribute,
 used in lighting. with color specified by a uniform.
 */
static const char* SHADER_3D_SKINPOSITION_BUMPEDNORMAL_TEXTURE;
/**
 Built in shader for particles, support Position and Texture, with a color specified by a uniform.
 */
static const char* SHADER_3D_PARTICLE_TEXTURE;
/**
 Built in shader for particles, support Position, with a color specified by a uniform.
 */
static const char* SHADER_3D_PARTICLE_COLOR;

/**
 Built in shader for skybox
 */
static const char* SHADER_3D_SKYBOX;

/**
 Built in shader for terrain
 */
static const char* SHADER_3D_TERRAIN;

/**
 Built in shader for camera clear
 */
static const char* SHADER_CAMERA_CLEAR;
/**
 end of built shader types.
 @}
 */


#endif /* GLSLBuiltInDefine_h */
