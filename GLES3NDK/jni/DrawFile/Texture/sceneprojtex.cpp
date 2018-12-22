#include "sceneprojtex.h"
#include "ImageLoad.h"

extern std::string strVert;
extern std::string strFrag;

SceneProjTex::SceneProjTex()
{
    prog.InitWithShader(strVert.c_str(), strFrag.c_str());
    prog.LinkProgram();

	GLenum drawBuffers[] = {GL_NONE};
	glDrawBuffers(1, drawBuffers);
}

void SceneProjTex::Init()
{
    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, Matrix4x4::IDENTITY);
    plane = new VBOPlane(100.0f,100.0f,1,1);

    projection = Matrix4x4::IDENTITY;

    angle = (float)( 90.0 * 3.1415926 / 180.0 );

    Vector3 projPos = Vector3(2.0f,5.0f,5.0f);
    Vector3 projAt = Vector3(-2.0f,-4.0f,0.0f);
    Vector3 projUp = Vector3(0.0f,1.0f,0.0f);
    Matrix4x4 projView;
    Matrix4x4::CreateLookAt(projPos, projAt, projUp, projView);
    
    Matrix4x4 projProj;
    Matrix4x4::CreatePerspective(30.0f, 1.0f, 0.2f, 1000.0f, projProj);
    
    Matrix4x4 transMat;
    Matrix4x4::CreateTranslate(Vector3(0.5,0.5,0.5), transMat);
    Matrix4x4 scaleMat;
    Matrix4x4::CreateScale(Vector3(0.5,0.5,0.5), scaleMat);
    Matrix4x4 projScaleTrans = transMat * scaleMat;
    
    Matrix4x4 projMat = projScaleTrans * projProj * projView;
    prog.SetUniformMatrix4f("ProjectorMatrix", 1, true, &projMat[0][0]);

    // Load texture file
    const char * texName = "/sdcard/texture/flower.png";

    // Copy file to OpenGL
    glActiveTexture(GL_TEXTURE0);
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    
    //LoadTexture2dWithSkia(texName, tid);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    
    //GL_CLAMP_TO_BORDER  在移动端不支持
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    prog.SetUniformVariable("ProjectorTex", 0);
    prog.SetUniformVariable("Light.Position", 0.0f,0.0f,0.0f,1.0f );
    prog.SetUniformVariable("Light.Intensity", 1.0f,1.0f,1.0f);
}

void SceneProjTex::update( float t )
{
    angle += 0.0001f;
    //if( angle > TWOPI) angle -= TWOPI;
}

void SceneProjTex::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vector3 cameraPos = Vector3( 10.0f * cos(angle), 2.0f, 10.0f * sin(angle));
    Matrix4x4::CreateLookAt(cameraPos, Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f), view);

    prog.SetUniformVariable("Material.Kd", 0.5f, 0.2f, 0.1f);
    prog.SetUniformVariable("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.SetUniformVariable("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Material.Shininess", 100.0f);

    model = Matrix4x4::IDENTITY;
    Matrix4x4 tranMat;
    Matrix4x4::CreateTranslate(Vector3(0.0f,-1.0f,0.0f), tranMat);
    Matrix4x4 rotateMat;
    Matrix4x4::CreateRotation(Vector3(1.0f,0.0f,0.0f), -90.0f, rotateMat);
    model = tranMat * rotateMat;
    setMatrices();
    teapot->render();

    prog.SetUniformVariable("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.SetUniformVariable("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.SetUniformVariable("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.SetUniformVariable("Material.Shininess", 1.0f);
    model = Matrix4x4::IDENTITY;
    tranMat = Matrix4x4::IDENTITY;
    Matrix4x4::CreateTranslate(Vector3(0.0f,-0.75f,0.0f), tranMat);
    model *= tranMat;
    setMatrices();
    plane->render();
}

void SceneProjTex::setMatrices()
{
    Matrix4x4 mv = view * model;
    prog.SetUniformMatrix4f("ModelMatrix",1, true, &model[0][0]);
    prog.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);
    Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
    prog.SetUniformMatrix3f("NormalMatrix",1,true, &matNormal[0][0]);
    Matrix4x4 mvp = projection * mv;
    prog.SetUniformMatrix4f("MVP", 1, true, &mvp[0][0]);
}

void SceneProjTex::Resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    
    Matrix4x4::CreatePerspective(50.0f, (float)w/h, 0.3f, 1000.0f, projection);
}
