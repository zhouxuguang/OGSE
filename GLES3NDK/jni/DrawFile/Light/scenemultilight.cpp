#include "scenemultilight.h"
#include "Vector3.h"
#include "Matrix4x4.h"


#define TWOPI (M_PI * 2)

extern std::string strVert;
extern std::string strFrag;

extern std::string mediaPath;

SceneMultiLight::SceneMultiLight()
{
	prog.InitWithShader(strVert.c_str(), strFrag.c_str());
	prog.LinkProgram();
}

void SceneMultiLight::Init()
{
    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(10.0f, 10.0f, 100, 100);
    //__android_log_print(ANDROID_LOG_INFO,"GLES2","content =sss");
    mediaPath = "/sdcard/";
    mediaPath += "media/pig_triangulated.obj";
    mesh = new VBOMesh(mediaPath.c_str(),true);
    //__android_log_print(ANDROID_LOG_INFO,"GLES2","content =sss");
    
    Matrix4x4::CreateLookAt(Vector3(0.5f,0.75f,0.75f), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f), view);

    char name[20] = {0};
    float x, z;
    for( int i = 0; i < 5; i++ ) {
        snprintf(name, 20, "lights[%d].Position", i);
        x = 2.0 * cos((TWOPI / 5) * i);
        z = 2.0 * sin((TWOPI / 5) * i);
        Vector4 vecLightPos = view * Vector4(x, 1.2f, z + 1.0f, 1.0f);
        prog.SetUniformVariable(name, vecLightPos.x, vecLightPos.y, vecLightPos.z, vecLightPos.w);
    }

    //光源
    prog.SetUniformVariable("lights[0].Intensity", 0.0f,0.8f,0.8f );
    prog.SetUniformVariable("lights[1].Intensity", 0.0f,0.0f,0.8f );
    prog.SetUniformVariable("lights[2].Intensity", 0.8f,0.0f,0.0f );
    prog.SetUniformVariable("lights[3].Intensity", 0.0f,0.8f,0.0f );
    prog.SetUniformVariable("lights[4].Intensity", 0.8f,0.8f,0.8f );
    
}

void SceneMultiLight::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //设置材质参数
    prog.SetUniformVariable("Kd", 0.4f, 0.4f, 0.4f);
    prog.SetUniformVariable("Ks", 0.9f, 0.9f, 0.9f);
    prog.SetUniformVariable("Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Shininess", 180.0f);

    Matrix4x4::CreateRotation(Vector3(0.0f,1.0f,0.0f), 90.0f, model);
    mesh->render();

    prog.SetUniformVariable("Kd", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Ks", 0.9f, 0.9f, 0.9f);
    prog.SetUniformVariable("Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Shininess", 180.0f);

    Matrix4x4::CreateTranslate(Vector3(0.0f,-0.45f,0.0f), model);
    setMatrices();
    plane->render();
}

void SceneMultiLight::setMatrices()
{
    Matrix4x4 mv = view * model;
    prog.SetUniformMatrix4f("ModelViewMatrix", 1, true, (const float*)&mv);
    
    Matrix3x3 normalMat = mv.GetMatrix3().Inverse().Transpose();
    prog.SetUniformMatrix3f("NormalMatrix",1, true, (const float*)&normalMat);
    prog.SetUniformMatrix4f("MVP",1, true, &(projection * mv)[0][0]);
}

void SceneMultiLight::Resize(int w, int h)
{
    glViewport(0,0,w,h);
    Matrix4x4::CreatePerspective(70.0f, (float)w/h, 0.3f, 100.0f, projection);
}
