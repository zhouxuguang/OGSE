#include "scenenormalmap.h"

#include <stdio.h>
#include <stdlib.h>

#include "ImageLoad.h"

extern std::string strVert;
extern std::string strFrag;

SceneNormalMap::SceneNormalMap()
{
    prog.InitWithShader(strVert.c_str(), strFrag.c_str());
    prog.LinkProgram();
}

void SceneNormalMap::Init()
{
    //光照计算需要开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    angle = (float)( (100.0)* 3.141592 / 180.0 );

    ogre = new VBOMesh("/sdcard/mesh/bs_ears.obj");
    
    Matrix4x4::CreateLookAt(Vector3(-1.0f,0.25f,2.5f), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f), view);

    prog.SetUniformVariable("Light.Intensity", 0.9f, 0.9f, 0.9f);

    GLuint texIDs[2];
    glGenTextures(2, texIDs);

    // Load diffuse texture file
    const char * texName = "/sdcard/texture/ogre_diffuse.png";
    //LoadTexture2dWithSkia(texName,texIDs[0]);

    // Copy diffuse texture to OpenGL
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIDs[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.SetUniformVariable("ColorTex", 0);

    // Load normal texture file
    texName = "/sdcard/texture/ogre_normalmap.png";
    //LoadTexture2dWithSkia(texName,texIDs[1]);

    // Copy normal texture to OpenGL
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texIDs[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.SetUniformVariable("NormalMapTex", 1);
}

void SceneNormalMap::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.SetUniformVariable("Light.Position", 10.0f * cos(angle), 1.0f, 10.0f * sin(angle), 1.0f);
    prog.SetUniformVariable("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.SetUniformVariable("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Material.Shininess", 1.0f);
    
    model = Matrix4x4::IDENTITY;
    setMatrices();
    ogre->render();
}

void SceneNormalMap::setMatrices()
{
    Matrix4x4 mv = view * model;
    prog.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);
    Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
    prog.SetUniformMatrix3f("NormalMatrix",1,true, &matNormal[0][0]);
    Matrix4x4 mvp = projection * mv;
    prog.SetUniformMatrix4f("MVP", 1, true, &mvp[0][0]);
}

void SceneNormalMap::Resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    float c = 2.0f;
    
    Matrix4x4::CreateOrthographic(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f, projection);
}
