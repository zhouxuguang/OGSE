//
//  GLSLShadeSource.hpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef GLSLShadeSource_hpp
#define GLSLShadeSource_hpp

#include "GLPrecompile.h"

extern const GLchar * ccPosition_uColor_frag;
extern const GLchar * ccPosition_uColor_vert;

extern const GLchar * ccPositionColor_frag;
extern const GLchar * ccPositionColor_vert;

extern const GLchar * ccPositionColorTextureAsPointsize_vert;

extern const GLchar * ccPositionTexture_frag;
extern const GLchar * ccPositionTexture_vert;

extern const GLchar * ccPositionTextureA8Color_frag;
extern const GLchar * ccPositionTextureA8Color_vert;

extern const GLchar * ccPositionTextureColor_frag;
extern const GLchar * ccPositionTextureColor_vert;

extern const GLchar * ccPositionTextureColor_noMVP_frag;
extern const GLchar * ccPositionTextureColor_noMVP_vert;

extern const GLchar * ccPositionTextureColorAlphaTest_frag;

extern const GLchar * ccPositionTexture_uColor_frag;
extern const GLchar * ccPositionTexture_uColor_vert;

extern const GLchar * ccPositionColorLengthTexture_frag;
extern const GLchar * ccPositionColorLengthTexture_vert;

extern const GLchar * ccPositionTexture_GrayScale_frag;

extern const GLchar * ccLabelDistanceFieldNormal_frag;
extern const GLchar * ccLabelDistanceFieldGlow_frag;
extern const GLchar * ccLabelNormal_frag;
extern const GLchar * ccLabelOutline_frag;

extern const GLchar * ccLabel_vert;

extern const GLchar * cc3D_PositionTex_vert;
extern const GLchar * cc3D_SkinPositionTex_vert;
extern const GLchar * cc3D_ColorTex_frag;
extern const GLchar * cc3D_Color_frag;
extern const GLchar * cc3D_PositionNormalTex_vert;
extern const GLchar * cc3D_SkinPositionNormalTex_vert;
extern const GLchar * cc3D_ColorNormalTex_frag;
extern const GLchar * cc3D_ColorNormal_frag;
extern const GLchar * cc3D_Particle_vert;
extern const GLchar * cc3D_Particle_tex_frag;
extern const GLchar * cc3D_Particle_color_frag;
extern const GLchar * cc3D_Skybox_vert;
extern const GLchar * cc3D_Skybox_frag;
extern const GLchar * cc3D_Terrain_vert;
extern const GLchar * cc3D_Terrain_frag;
extern const GLchar * ccCameraClearVert;
extern const GLchar * ccCameraClearFrag;
// ETC1 ALPHA supports.
extern const GLchar* ccETC1ASPositionTextureColor_frag;
extern const GLchar* ccETC1ASPositionTextureGray_frag;

#endif /* GLSLShadeSource_hpp */
