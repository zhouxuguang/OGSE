#include "FrameBuffer.h"
#include "ColorRGBA.h"

NS_MATH3D_BEGIN

FrameBuffer::FrameBuffer(void)
{
	m_nHeight = 0;
	m_nWidth = 0;
	m_pColorBuf = NULL;
	m_pfDepthBuf = NULL;
}

FrameBuffer::~FrameBuffer(void)
{
	ClearBuffer();
	m_nHeight = 0;
	m_nWidth = 0;
}

void FrameBuffer::SetBufferSize(unsigned int nWidth,unsigned int nHeight)
{
	assert(nWidth > 0 && nHeight > 0);
	if (m_nWidth == nWidth && m_nHeight == nHeight)
	{
		return;
	}
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	ClearBuffer();

	m_pfDepthBuf = (float *)malloc(m_nHeight*m_nWidth*sizeof(float));
	m_pColorBuf = (unsigned char *)malloc(m_nHeight*m_nWidth*sizeof(unsigned char)*4);
}

void FrameBuffer::ClearBuffer()
{
	if (m_pColorBuf != NULL)
	{
		free(m_pColorBuf);
		m_pColorBuf = NULL;
	}
	if (m_pfDepthBuf != NULL)
	{
		free(m_pfDepthBuf);
		m_pfDepthBuf = NULL;
	}
}

void FrameBuffer::SetColor(unsigned int nX,unsigned int nY,const ColorRGBA& color)
{
	assert(nX < m_nWidth && nY < m_nHeight);
	int nOffset = (nY * m_nWidth + nX)*4;
	unsigned char* pBuf = m_pColorBuf + nOffset;
	pBuf[0] = color[0]*255;
	pBuf[1] = color[1]*255;
	pBuf[2] = color[2]*255;
	pBuf[3] = color[3]*255;
}

void FrameBuffer::SetDepth(unsigned int nX,unsigned int nY,float fDepth)
{
	assert(nX < m_nWidth && nY < m_nHeight);
	int nOffset = nY * m_nWidth + nX;
	float* pBuf = m_pfDepthBuf + nOffset;
	*pBuf = fDepth;
}

FrameBuffer* GetFrameBuffer()
{
	static FrameBuffer instance;
	return &instance;
}

NS_MATH3D_END
