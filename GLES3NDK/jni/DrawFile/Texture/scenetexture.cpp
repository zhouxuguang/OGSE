#include "scenetexture.h"
#include "Matrix3x3.h"
#include "Vector4.h"
#include "Vector3.h"

#include "ImageLoad.h"


extern std::string strVert;
extern std::string strFrag;

SceneTexture::SceneTexture()
{
	prog.InitWithShader(strVert.c_str(), strFrag.c_str());
	prog.LinkProgram();
}

void SceneTexture::Init()
{
	glClearColor(1.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);

    cube = new VBOCube();

    model = Matrix4x4::IDENTITY;

    angle = 0.0;

    prog.SetUniformVariable("Light.Intensity", 1.0f,1.0f,1.0f );

    // Load texture file
    std::string texName = "/sdcard/texture/brick1.jpg";
    

    std::string texName1 = __FILE__;
//    __android_log_print(ANDROID_LOG_INFO,"GLES2","cpp file path : %s", texName1.c_str());
//
//    __android_log_print(ANDROID_LOG_INFO,"GLES2","width = %d, height = %d, file not found ",bitmap.width(), bitmap.height());

    //fclose(fp);

    // Copy file to OpenGL
    glActiveTexture(GL_TEXTURE0);
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    //LoadTexture2dWithSkia(texName.c_str(),tid);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    prog.SetUniformVariable("BrickTex", 0);

    texName = "/sdcard/texture/moss.png";

    // Copy file to OpenGL
    glActiveTexture(GL_TEXTURE1);
    GLuint tidMoss;
    glGenTextures(1, &tidMoss);
    glBindTexture(GL_TEXTURE_2D, tidMoss);
    //LoadTexture2dWithSkia(texName.c_str(),tidMoss);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    prog.SetUniformVariable("MossTex", 1);
}

#define TWOPI (M_PI * 2)

void SceneTexture::update( float t )
{
    angle += 0.01f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneTexture::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.SetUniformVariable("Light.Position", 0.0f,0.0f,0.0f,1.0f );
    prog.SetUniformVariable("Material.Kd", 0.9f, 0.9f, 0.9f);
    prog.SetUniformVariable("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.SetUniformVariable("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Material.Shininess", 100.0f);

    Matrix4x4::CreateLookAt(Vector3(1.0f,1.5f,2.0f), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f), view);
    setMatrices();
    cube->render();
}

void SceneTexture::setMatrices()
{
    Matrix4x4 mv = view * model;
    prog.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);
    Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
    prog.SetUniformMatrix3f("NormalMatrix",1,true, &matNormal[0][0]);
    Matrix4x4 mvp = projection * mv;
    prog.SetUniformMatrix4f("MVP", 1, true, &mvp[0][0]);
}

void SceneTexture::Resize(int w, int h)
{
    glViewport(0,0,w,h);
    Matrix4x4::CreatePerspective(60.0f, (float)w/h, 0.3f, 100.0f, projection);
}
