//
//  GLSLShadeSource.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "GLSLShadeSource.h"

#include "shaders/ccShader_Position_uColor.frag"

#include "shaders/ccShader_Position_uColor.vert"

//
#include "shaders/ccShader_PositionColor.frag"
#include "shaders/ccShader_PositionColor.vert"

//
#include "shaders/ccShader_PositionColorTextureAsPointsize.vert"

//
#include "shaders/ccShader_PositionTexture.frag"
#include "shaders/ccShader_PositionTexture.vert"

//
#include "shaders/ccShader_PositionTextureA8Color.frag"
#include "shaders/ccShader_PositionTextureA8Color.vert"

//
#include "shaders/ccShader_PositionTextureColor.frag"
#include "shaders/ccShader_PositionTextureColor.vert"

//
#include "shaders/ccShader_PositionTextureColor_noMVP.frag"
#include "shaders/ccShader_PositionTextureColor_noMVP.vert"

//
#include "shaders/ccShader_PositionTextureColorAlphaTest.frag"

//
#include "shaders/ccShader_PositionTexture_uColor.frag"
#include "shaders/ccShader_PositionTexture_uColor.vert"

#include "shaders/ccShader_PositionColorLengthTexture.frag"
#include "shaders/ccShader_PositionColorLengthTexture.vert"

#include "shaders/ccShader_UI_Gray.frag"
//
#include "shaders/ccShader_Label.vert"
#include "shaders/ccShader_Label_df.frag"
#include "shaders/ccShader_Label_df_glow.frag"
#include "shaders/ccShader_Label_normal.frag"
#include "shaders/ccShader_Label_outline.frag"

//
#include "shaders/ccShader_3D_PositionTex.vert"
#include "shaders/ccShader_3D_Color.frag"
#include "shaders/ccShader_3D_ColorTex.frag"
#include "shaders/ccShader_3D_PositionNormalTex.vert"
#include "shaders/ccShader_3D_ColorNormal.frag"
#include "shaders/ccShader_3D_ColorNormalTex.frag"
#include "shaders/ccShader_3D_Particle.vert"
#include "shaders/ccShader_3D_Particle.frag"
#include "shaders/ccShader_3D_Skybox.vert"
#include "shaders/ccShader_3D_Skybox.frag"
#include "shaders/ccShader_3D_Terrain.vert"
#include "shaders/ccShader_3D_Terrain.frag"
#include "shaders/ccShader_CameraClear.vert"
#include "shaders/ccShader_CameraClear.frag"

// ETC1 ALPHA support
#include "shaders/ccShader_ETC1AS_PositionTextureColor.frag"
#include "shaders/ccShader_ETC1AS_PositionTextureGray.frag"
