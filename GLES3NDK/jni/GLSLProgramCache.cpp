//
//  GLSLProgramCache.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "GLSLProgramCache.h"
#include "GLSLProgram.h"
#include "GLSLShadeSource.h"

const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR = "#ShaderETC1ASPositionTextureColor";
const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR_NO_MVP = "#ShaderETC1ASPositionTextureColor_noMVP";

const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY = "#ShaderETC1ASPositionTextureGray";
const char* SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY_NO_MVP = "#ShaderETC1ASPositionTextureGray_noMVP";

const char* SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP = "ShaderPositionTextureColor_noMVP";
const char* SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST = "ShaderPositionTextureColorAlphaTest";
const char* SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV = "ShaderPositionTextureColorAlphaTest_NoMV";
const char* SHADER_NAME_POSITION_COLOR = "ShaderPositionColor";
const char* SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE = "ShaderPositionColorTexAsPointsize";
const char* SHADER_NAME_POSITION_COLOR_NO_MVP = "ShaderPositionColor_noMVP";

const char* SHADER_NAME_POSITION_TEXTURE = "ShaderPositionTexture";
const char* SHADER_NAME_POSITION_TEXTURE_U_COLOR = "ShaderPositionTexture_uColor";
const char* SHADER_NAME_POSITION_TEXTURE_A8_COLOR = "ShaderPositionTextureA8Color";
const char* SHADER_NAME_POSITION_U_COLOR = "ShaderPosition_uColor";
const char* SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR = "ShaderPositionLengthTextureColor";
const char* SHADER_NAME_POSITION_GRAYSCALE = "ShaderUIGrayScale";
const char* SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL = "ShaderLabelDFNormal";
const char* SHADER_NAME_LABEL_DISTANCEFIELD_GLOW = "ShaderLabelDFGlow";
const char* SHADER_NAME_LABEL_NORMAL = "ShaderLabelNormal";
const char* SHADER_NAME_LABEL_OUTLINE = "ShaderLabelOutline";

const char* SHADER_3D_POSITION = "Shader3DPosition";
const char* SHADER_3D_POSITION_TEXTURE = "Shader3DPositionTexture";
const char* SHADER_3D_SKINPOSITION_TEXTURE = "Shader3DSkinPositionTexture";
const char* SHADER_3D_POSITION_NORMAL = "Shader3DPositionNormal";
const char* SHADER_3D_POSITION_NORMAL_TEXTURE = "Shader3DPositionNormalTexture";
const char* SHADER_3D_SKINPOSITION_NORMAL_TEXTURE = "Shader3DSkinPositionNormalTexture";
const char* SHADER_3D_POSITION_BUMPEDNORMAL_TEXTURE = "Shader3DPositionBumpedNormalTexture";
const char* SHADER_3D_SKINPOSITION_BUMPEDNORMAL_TEXTURE = "Shader3DSkinPositionBumpedNormalTexture";
const char* SHADER_3D_PARTICLE_COLOR = "Shader3DParticleColor";
const char* SHADER_3D_PARTICLE_TEXTURE = "Shader3DParticleTexture";
const char* SHADER_3D_SKYBOX = "Shader3DSkybox";
const char* SHADER_3D_TERRAIN = "Shader3DTerrain";
const char* SHADER_CAMERA_CLEAR = "ShaderCameraClear";

enum {
    kShaderType_PositionTextureColor,
    kShaderType_PositionTextureColor_noMVP,
    kShaderType_PositionTextureColorAlphaTest,
    kShaderType_PositionTextureColorAlphaTestNoMV,
    kShaderType_PositionColor,
    kShaderType_PositionColorTextureAsPointsize,
    kShaderType_PositionColor_noMVP,
    kShaderType_PositionTexture,
    kShaderType_PositionTexture_uColor,
    kShaderType_PositionTextureA8Color,
    kShaderType_Position_uColor,
    kShaderType_PositionLengthTextureColor,
    kShaderType_LabelDistanceFieldNormal,
    kShaderType_LabelDistanceFieldGlow,
    kShaderType_UIGrayScale,
    kShaderType_LabelNormal,
    kShaderType_LabelOutline,
    kShaderType_3DPosition,
    kShaderType_3DPositionTex,
    kShaderType_3DSkinPositionTex,
    kShaderType_3DPositionNormal,
    kShaderType_3DPositionNormalTex,
    kShaderType_3DSkinPositionNormalTex,
    kShaderType_3DPositionBumpedNormalTex,
    kShaderType_3DSkinPositionBumpedNormalTex,
    kShaderType_3DParticleTex,
    kShaderType_3DParticleColor,
    kShaderType_3DSkyBox,
    kShaderType_3DTerrain,
    kShaderType_CameraClear,
    // ETC1 ALPHA supports.
    kShaderType_ETC1ASPositionTextureColor,
    kShaderType_ETC1ASPositionTextureColor_noMVP,
    
    kShaderType_ETC1ASPositionTextureGray,
    kShaderType_ETC1ASPositionTextureGray_noMVP,
    kShaderType_MAX,
};

static GLSLProgramCache *_sharedGLProgramCache = NULL;

GLSLProgramCache* GLSLProgramCache::GetInstance()
{
    if (!_sharedGLProgramCache)
    {
        _sharedGLProgramCache = new(std::nothrow) GLSLProgramCache();
        if (!_sharedGLProgramCache->init())
        {
            delete _sharedGLProgramCache;
        }
    }
    return _sharedGLProgramCache;
}

GLSLProgramCache::GLSLProgramCache()
: _programs()
{
    init();
}

GLSLProgramCache::~GLSLProgramCache()
{
//    for(auto& program : _programs)
//    {
//        program.second->release();
//    }
}

bool GLSLProgramCache::init()
{
    LoadDefaultGLSLPrograms();
    
    return true;
}

void GLSLProgramCache::LoadDefaultGLSLPrograms()
{
    // Position Texture Color shader
    GLSLProgram *p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTextureColor);
    _programs.emplace(SHADER_NAME_POSITION_TEXTURE_COLOR, p);
    
    // Position Texture Color without MVP shader
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTextureColor_noMVP);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, p);
    
    // Position Texture Color alpha test
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTextureColorAlphaTest);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST, p);
    
    // Position Texture Color alpha test
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTextureColorAlphaTestNoMV);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV, p);
    //
    // Position, Color shader
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionColor);
    _programs.emplace(::SHADER_NAME_POSITION_COLOR, p);
    
    // Position, Color, PointSize shader
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionColorTextureAsPointsize);
    _programs.emplace(::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE, p);
    
    //
    // Position, Color shader no MVP
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionColor_noMVP);
    _programs.emplace(::SHADER_NAME_POSITION_COLOR_NO_MVP, p);
    
    //
    // Position Texture shader
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTexture);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE, p);
    
    //
    // Position, Texture attribs, 1 Color as uniform shader
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTexture_uColor);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE_U_COLOR, p);
    
    //
    // Position Texture A8 Color shader
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionTextureA8Color);
    _programs.emplace(::SHADER_NAME_POSITION_TEXTURE_A8_COLOR, p);
    
    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_Position_uColor);
    _programs.emplace(::SHADER_NAME_POSITION_U_COLOR, p);
    
    //
    // Position, Length(TexCoords, Color (used by Draw Node basically )
    //
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_PositionLengthTextureColor);
    _programs.emplace(::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_LabelDistanceFieldNormal);
    _programs.emplace(::SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_LabelDistanceFieldGlow);
    _programs.emplace(::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_UIGrayScale);
    _programs.emplace(::SHADER_NAME_POSITION_GRAYSCALE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_LabelNormal);
    _programs.emplace(::SHADER_NAME_LABEL_NORMAL, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_LabelOutline);
    _programs.emplace(::SHADER_NAME_LABEL_OUTLINE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DPosition);
    _programs.emplace(::SHADER_3D_POSITION, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DPositionTex);
    _programs.emplace(::SHADER_3D_POSITION_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DSkinPositionTex);
    _programs.emplace(::SHADER_3D_SKINPOSITION_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DPositionNormal);
    _programs.emplace(::SHADER_3D_POSITION_NORMAL, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DPositionNormalTex);
    _programs.emplace(::SHADER_3D_POSITION_NORMAL_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DSkinPositionNormalTex);
    _programs.emplace(::SHADER_3D_SKINPOSITION_NORMAL_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DPositionBumpedNormalTex);
    _programs.emplace(::SHADER_3D_POSITION_BUMPEDNORMAL_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DSkinPositionBumpedNormalTex);
    _programs.emplace(::SHADER_3D_SKINPOSITION_BUMPEDNORMAL_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DParticleColor);
    _programs.emplace(::SHADER_3D_PARTICLE_COLOR, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DParticleTex);
    _programs.emplace(::SHADER_3D_PARTICLE_TEXTURE, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DSkyBox);
    _programs.emplace(::SHADER_3D_SKYBOX, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_3DTerrain);
    _programs.emplace(::SHADER_3D_TERRAIN, p);
    
    p = new (std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_CameraClear);
    _programs.emplace(::SHADER_CAMERA_CLEAR, p);
    
    /// ETC1 ALPHA supports.
    p = new(std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_ETC1ASPositionTextureColor);
    _programs.emplace(::SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR, p);
    
    p = new(std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_ETC1ASPositionTextureColor_noMVP);
    _programs.emplace(::SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR_NO_MVP, p);
    
    /// ETC1 Gray supports.
    p = new(std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_ETC1ASPositionTextureGray);
    _programs.emplace(::SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY, p);
    
    p = new(std::nothrow) GLSLProgram();
    LoadDefaultGLSLProgram(p, kShaderType_ETC1ASPositionTextureGray_noMVP);
    _programs.emplace(::SHADER_NAME_ETC1AS_POSITION_TEXTURE_GRAY_NO_MVP, p);
}

void GLSLProgramCache::LoadDefaultGLSLProgram(GLSLProgram *p, int type)
{
    switch (type)
    {
        case kShaderType_PositionTextureColor:
            p->InitWithShader(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
            break;
        case kShaderType_PositionTextureColor_noMVP:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert, ccPositionTextureColor_noMVP_frag);
            break;
        case kShaderType_PositionTextureColorAlphaTest:
            p->InitWithShader(ccPositionTextureColor_vert, ccPositionTextureColorAlphaTest_frag);
            break;
        case kShaderType_PositionTextureColorAlphaTestNoMV:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert, ccPositionTextureColorAlphaTest_frag);
            break;
        case kShaderType_PositionColor:
            p->InitWithShader(ccPositionColor_vert ,ccPositionColor_frag);
            break;
        case kShaderType_PositionColorTextureAsPointsize:
            p->InitWithShader(ccPositionColorTextureAsPointsize_vert ,ccPositionColor_frag);
            break;
        case kShaderType_PositionColor_noMVP:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert ,ccPositionColor_frag);
            break;
        case kShaderType_PositionTexture:
            p->InitWithShader(ccPositionTexture_vert ,ccPositionTexture_frag);
            break;
        case kShaderType_PositionTexture_uColor:
            p->InitWithShader(ccPositionTexture_uColor_vert, ccPositionTexture_uColor_frag);
            break;
        case kShaderType_PositionTextureA8Color:
            p->InitWithShader(ccPositionTextureA8Color_vert, ccPositionTextureA8Color_frag);
            break;
        case kShaderType_Position_uColor:
            p->InitWithShader(ccPosition_uColor_vert, ccPosition_uColor_frag);
            //p->bindAttribLocation("aVertex", GLProgram::VERTEX_ATTRIB_POSITION);
            break;
        case kShaderType_PositionLengthTextureColor:
            p->InitWithShader(ccPositionColorLengthTexture_vert, ccPositionColorLengthTexture_frag);
            break;
        case kShaderType_LabelDistanceFieldNormal:
            p->InitWithShader(ccLabel_vert, ccLabelDistanceFieldNormal_frag);
            break;
        case kShaderType_LabelDistanceFieldGlow:
            p->InitWithShader(ccLabel_vert, ccLabelDistanceFieldGlow_frag);
            break;
        case kShaderType_UIGrayScale:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert,
                                  ccPositionTexture_GrayScale_frag);
            break;
        case kShaderType_LabelNormal:
            p->InitWithShader(ccLabel_vert, ccLabelNormal_frag);
            break;
        case kShaderType_LabelOutline:
            p->InitWithShader(ccLabel_vert, ccLabelOutline_frag);
            break;
        case kShaderType_3DPosition:
            p->InitWithShader(cc3D_PositionTex_vert, cc3D_Color_frag);
            break;
        case kShaderType_3DPositionTex:
            p->InitWithShader(cc3D_PositionTex_vert, cc3D_ColorTex_frag);
            break;
        case kShaderType_3DSkinPositionTex:
            p->InitWithShader(cc3D_SkinPositionTex_vert, cc3D_ColorTex_frag);
            break;
        case kShaderType_3DPositionNormal:
        {
            //std::string def = getShaderMacrosForLight();
            //p->InitWithShader((def + std::string(cc3D_PositionNormalTex_vert)).c_str(), (def + std::string(cc3D_ColorNormal_frag)).c_str());
        }
            break;
        case kShaderType_3DPositionNormalTex:
        {
            //std::string def = getShaderMacrosForLight();
            //p->initWithByteArrays((def + std::string(cc3D_PositionNormalTex_vert)).c_str(), (def + std::string(cc3D_ColorNormalTex_frag)).c_str());
        }
            break;
        case kShaderType_3DSkinPositionNormalTex:
        {
            //std::string def = getShaderMacrosForLight();
            //p->initWithByteArrays((def + std::string(cc3D_SkinPositionNormalTex_vert)).c_str(), (def + std::string(cc3D_ColorNormalTex_frag)).c_str());
        }
            break;
        case kShaderType_3DPositionBumpedNormalTex:
        {
            //std::string def = getShaderMacrosForLight();
            //std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
            //p->initWithByteArrays((def + normalMapDef + std::string(cc3D_PositionNormalTex_vert)).c_str(), (def + normalMapDef + std::string(cc3D_ColorNormalTex_frag)).c_str());
        }
            break;
        case kShaderType_3DSkinPositionBumpedNormalTex:
        {
            //std::string def = getShaderMacrosForLight();
            //std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
            //p->initWithByteArrays((def + normalMapDef + std::string(cc3D_SkinPositionNormalTex_vert)).c_str(), (def + normalMapDef + std::string(cc3D_ColorNormalTex_frag)).c_str());
        }
            break;
        case kShaderType_3DParticleTex:
        {
            p->InitWithShader(cc3D_Particle_vert, cc3D_Particle_tex_frag);
        }
            break;
        case kShaderType_3DParticleColor:
            p->InitWithShader(cc3D_Particle_vert, cc3D_Particle_color_frag);
            break;
        case kShaderType_3DSkyBox:
            p->InitWithShader(cc3D_Skybox_vert, cc3D_Skybox_frag);
            break;
        case kShaderType_3DTerrain:
            p->InitWithShader(cc3D_Terrain_vert, cc3D_Terrain_frag);
            break;
        case kShaderType_CameraClear:
            p->InitWithShader(ccCameraClearVert, ccCameraClearFrag);
            break;
            /// ETC1 ALPHA supports.
        case kShaderType_ETC1ASPositionTextureColor:
            p->InitWithShader(ccPositionTextureColor_vert, ccETC1ASPositionTextureColor_frag);
            break;
        case kShaderType_ETC1ASPositionTextureColor_noMVP:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert, ccETC1ASPositionTextureColor_frag);
            break;
            /// ETC1 GRAY supports.
        case kShaderType_ETC1ASPositionTextureGray:
            p->InitWithShader(ccPositionTextureColor_vert, ccETC1ASPositionTextureGray_frag);
            break;
        case kShaderType_ETC1ASPositionTextureGray_noMVP:
            p->InitWithShader(ccPositionTextureColor_noMVP_vert, ccETC1ASPositionTextureGray_frag);
            break;
        default:
            break;
            return;
    }
    
    p->LinkProgram();
    //p->updateUniforms();
}

GLSLProgram* GLSLProgramCache::GetGLProgram(const std::string &key)
{
    std::unordered_map<std::string, GLSLProgram*>::iterator it = _programs.find(key);
    if( it != _programs.end() )
        return it->second;
    return NULL;
}

void GLSLProgramCache::AddGLProgram(GLSLProgram* program, const std::string &key)
{
    GLSLProgram * prev = GetGLProgram(key);
    if( prev == program )
        return;
    
    _programs.erase(key);
    delete prev;
    
    if (program)
    {
        //program->retain();
    }
    _programs[key] = program;
}
