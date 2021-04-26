#include "UniformShader.h"

extern std::string strVert;
extern std::string strFrag;

UniformShader::UniformShader(void)
{
	mProgram.InitWithShader(strVert.c_str(), strFrag.c_str());

	mProgram.LinkProgram();
}

UniformShader::~UniformShader(void)
{
}

void UniformShader::InitUniformBlockBuffer()
{
	// Allocate space for the buffer
	GLint blockSize;
	mProgram.GetActiveUniformBlockiv("BlobSettings",GL_UNIFORM_BLOCK_DATA_SIZE,&blockSize);
	GLubyte * blockBuffer;
	blockBuffer = (GLubyte *) malloc(blockSize);

	// Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	mProgram.GetUniformIndices(4,names,indices);

	GLint offset[4];
	mProgram.GetActiveUniformsiv(4, indices, GL_UNIFORM_OFFSET, offset);

	// Store data within the buffer at the appropriate offsets
	GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};
	GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

	memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

	// Create the buffer object and copy the data
	GLuint uboHandle;
	glGenBuffers( 1, &uboHandle );
	glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );
	glBufferData( GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW );

	// Bind the buffer object to the uniform block
	GLuint blockIndex = mProgram.GetUniformBlockIndex("BlobSettings");
	glBindBufferBase( GL_UNIFORM_BUFFER, blockIndex, uboHandle );
}

void UniformShader::Init()
{
	InitUniformBlockBuffer();

	/////////////////// Create the VBO ////////////////////
	float positionData[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f,  0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f
	};
	float tcData[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};


	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint tcBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tcData, GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays( 1, &vaoHandle );
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex texture coords

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UniformShader::Resize(int width,int height)
{
	glViewport(0,0,width,height);
}

void UniformShader::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 6 );
}
