#include "sceneshadow.h"

#include <cstdio>

#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

USING_NS_MATH3D

#define stringify_1(x...) #x 
//#define STRINGIFY(x...)   stringify_1(x)

#define STRINGIFY(x) #x

const char * pszFirstVert = STRINGIFY(
precision mediump float;

in vec3 LightIntensity;

layout( location = 0 ) out vec4 FragColor;

void main() {
	FragColor = vec4(LightIntensity, 1.0);
}

);

const char* pszFirstFrag = "#version 300 es \n"
"precision mediump float;\n"
"layout(location = 0) out float fragmentdepth; \n"

"void main(){ \n"
	"fragmentdepth = gl_FragCoord.z; \n"
"} \n";

extern std::string strVert;
extern std::string strFrag;


std::string strVert2;
std::string strFrag2;

//void GetShaderFile2(AAssetManager* mgr,const char* vertFile,const char* fragFile)
//{
//	//打开顶点shader
//	AAsset* asset = AAssetManager_open(mgr,vertFile,AASSET_MODE_UNKNOWN);
//	off_t lenght = AAsset_getLength(asset);
//	strVert2.resize(lenght+1,0);
//	memcpy((char*)strVert2.data(),AAsset_getBuffer(asset),lenght);
//	AAsset_close(asset);
//
//	asset = AAssetManager_open(mgr,fragFile,AASSET_MODE_UNKNOWN);
//	lenght = AAsset_getLength(asset);
//	strFrag2.resize(lenght+1,0);
//	memcpy((char*)strFrag2.data(),AAsset_getBuffer(asset),lenght);
//	AAsset_close(asset);
//}

SceneShadowMap::SceneShadowMap()
{
	width = 800;
	height = 600;
	shadowMapWidth = 1024;
	shadowMapHeight = 1980;

	//初始化shader
	prog.InitWithShader(strVert.c_str(), strFrag.c_str());
	prog.LinkProgram();

	//GetShaderFile2(mgr,"shadowmap.vert","shadowmap2.frag");
	solidProg.InitWithShader(strVert2.c_str(), strFrag2.c_str());
	solidProg.LinkProgram();
}

void SceneShadowMap::Init()
{

	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);

	angle = M_PI/4.0f;

	teapot = new VBOTeapot(14, Matrix4x4::IDENTITY);
	plane = new VBOPlane(40.0f, 40.0f, 2, 2);
	float scale = 2.0f;
	torus = new VBOTorus(0.7f * scale,0.3f * scale,50,50);
	// Set up the framebuffer object
	setupFBO();

	shadowBias = Matrix4x4( 0.5f,0.0f,0.0f,0.0f,
		0.0f,0.5f,0.0f,0.0f,
		0.0f,0.0f,0.5f,0.0f,
		0.5f,0.5f,0.5f,1.0f
		);

	float c = 1.65f;
	Vector3 lightPos = Vector3(0.0f,c * 5.25f, c * 7.5f);  // World coords

	Matrix4x4::CreateLookAt(lightPos,Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f),view);
	Matrix4x4::CreatePerspective(50.0f, 1.0f, 1.0f, 25.0f,projection);
	lightPV = shadowBias * projection * view;

	solidProg.SetUniformVariable("Light.Intensity",0.85f,0.85f,0.85f);
	solidProg.SetUniformVariable("ShadowMap", 0);

	//es 不支持
	//glLineWidth(4.5f);
	glPointSize(3.5f);
}

void SceneShadowMap::spitOutDepthBuffer() 
{
	int size = shadowMapWidth * shadowMapHeight;
	float * buffer = new float[size];
	unsigned char * imgBuffer = new unsigned char[size * 4];

	//glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,GL_FLOAT,buffer);

	for( int i = 0; i < shadowMapHeight; i++ )
		for( int j = 0; j < shadowMapWidth; j++ )
		{
			int imgIdx = 4 * ((i*shadowMapWidth) + j);
			int bufIdx = ((shadowMapHeight - i - 1) * shadowMapWidth) + j;

			// This is just to make a more visible image.  Scale so that
			// the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
			// be tweaked for different light configurations.
			float minVal = 0.88f;
			float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
			unsigned char val = (unsigned char)(scale * 255);
			imgBuffer[imgIdx] = val;
			imgBuffer[imgIdx+1] = val;
			imgBuffer[imgIdx+2] = val;
			imgBuffer[imgIdx+3] = 0xff;
		}

		//QImage img(imgBuffer, shadowMapWidth, shadowMapHeight, QImage::Format_RGB32);
		//img.save("depth.png", "PNG");

		delete [] buffer;
		delete [] imgBuffer;
		//exit(1);
}

void SceneShadowMap::setupFBO()
{
	//GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
	// The depth buffer texture
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth,
		shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//opengl es不支持纹理边框颜色
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);    

	// Assign the depth buffer texture to texture channel 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	// Create and set up the FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = {GL_NONE};
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( result == GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("Framebuffer is complete.\n");
	} 
	else 
	{
		printf("Framebuffer is not complete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneShadowMap::update( float t )
{
	angle += 0.003f;
	//if( angle > TWOPI) angle -= TWOPI;
}

void SceneShadowMap::Draw()
{
	static int i = 0;
	prog.Use();
	// Pass 1 (shadow map generation)
	float c = 1.65f;
	Vector3 lightPos = Vector3(0.0f,c * 5.25f, c * 7.5f);  // World coords
	Matrix4x4::CreateLookAt(lightPos,Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f),view);
	Matrix4x4::CreatePerspective(50.0f, 1.0f, 1.0f, 25.0f,projection);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,shadowMapWidth,shadowMapHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawScene(prog);
	glFlush();
	glFinish();
	//spitOutDepthBuffer(); // This is just used to get an image of the depth buffer
	prog.UseFixedFunction();

	// Pass 2 (render)
	solidProg.Use();
	c = 1.0f;
	Vector3 cameraPos(c * 11.5f * cos(angle),c * 7.0f,c * 11.5f * sin(angle));

	Matrix4x4::CreateLookAt(cameraPos,Vector3(0.0f,0.0f,0.0f),Vector3(0.0f,1.0f,0.0f),view);

	c = 1.65f;
	Vector4 lightPos1 = Vector4(0.0f,c * 5.25f, c * 7.5f, 1.0f);  // World coords
	lightPos1 = view * lightPos1;
	prog.SetUniformVariable("Light.Position", lightPos1.x,lightPos1.y,lightPos1.z,lightPos1.w);

	Matrix4x4::CreatePerspective(50.0f, (float)width/height, 0.1f, 100.0f,projection);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,width,height);
	glDisable(GL_CULL_FACE);
	drawScene(solidProg);
}

void SceneShadowMap::drawScene(GLSLProgram &prog)
{
	//1
	float redColor = 0.7f;
	float greenColor = 0.5f;
	float blueColor = 0.3f;
	prog.SetUniformVariable("Material.Ka", redColor * 0.05f,greenColor * 0.05f, blueColor * 0.05f);
	prog.SetUniformVariable("Material.Kd", redColor,greenColor, blueColor);
	prog.SetUniformVariable("Material.Ks", 0.9f,0.9f,0.9f);
	prog.SetUniformVariable("Material.Shininess", 150.0f);
	model = Matrix4x4::IDENTITY;

	Matrix4x4 matTrans;
	Matrix4x4::CreateTranslate(Vector3(0.0f,0.0f,0.0f),matTrans);
	model *= matTrans;

	Matrix4x4 matRotate;
	Matrix4x4::CreateRotation(Vector3(1.0f,0.0f,0.0f),-90.0f,matRotate);
	model *= matRotate;
	setMatrices(prog);
	teapot->render();

	//2
	prog.SetUniformVariable("Material.Ka", redColor * 0.05f,greenColor * 0.05f, blueColor * 0.05f);
	prog.SetUniformVariable("Material.Kd", redColor,greenColor, blueColor);
	prog.SetUniformVariable("Material.Ks", 0.9f,0.9f,0.9f);
	prog.SetUniformVariable("Material.Shininess", 150.0f);
	model = Matrix4x4::IDENTITY;

	Matrix4x4::CreateTranslate(Vector3(0.0f,2.0f,5.0f),matTrans);
	model *= matTrans;
	Matrix4x4::CreateRotation(Vector3(1.0f,0.0f,0.0f),-45.0f,matRotate);
	setMatrices(prog);
	torus->render();

	//3
	prog.SetUniformVariable("Material.Kd", 0.25f, 0.25f, 0.25f);
	prog.SetUniformVariable("Material.Ks", 0.0f, 0.0f, 0.0f);
	prog.SetUniformVariable("Material.Ka", 0.05f, 0.05f, 0.05f);
	prog.SetUniformVariable("Material.Shininess", 1.0f);
	model = Matrix4x4::IDENTITY;
	Matrix4x4::CreateTranslate(Vector3(0.0f,0.0f,0.0f),matTrans);
	model *= matTrans;
	setMatrices(prog);
	plane->render();

	//4
	model = Matrix4x4::IDENTITY;
	Matrix4x4::CreateTranslate(Vector3(-5.0f,5.0f,0.0f),matTrans);
	model *= matTrans;
	Matrix4x4::CreateRotation(Vector3(0.0f,0.0f,1.0f),-45.0f,matRotate);
	model *= matRotate;
	setMatrices(prog);
	plane->render();

	//5
	model = Matrix4x4::IDENTITY;
	Matrix4x4::CreateTranslate(Vector3(0.0f,5.0f,-5.0f),matTrans);
	model *= matTrans;
	Matrix4x4::CreateRotation(Vector3(1.0f,0.0f,0.0f),90.0f,matRotate);
	model *= matRotate;
	setMatrices(prog);
	plane->render();
	model = Matrix4x4::IDENTITY;
}

void SceneShadowMap::setMatrices(GLSLProgram &prog)
{
	Matrix4x4 mv = view * model;
	prog.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);
	Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
	prog.SetUniformMatrix3f("NormalMatrix",1,true, &matNormal[0][0]);
	Matrix4x4 mvp = projection * mv;
	prog.SetUniformMatrix4f("MVP", 1, true, &mvp[0][0]);

	Matrix4x4 matShadow = lightPV * model;
	prog.SetUniformMatrix4f("ShadowMatrix", 1, true, (const float*)&(matShadow));
}

void SceneShadowMap::Resize(int w, int h)
{
	glViewport(0,0,w,h);
	width = w;
	height = h;
}
