#include "AlphaBlend.h"

NS_MATH3D_BEGIN

static void GetFactor(const ColorRGBA& srcColor,const ColorRGBA& dstColor,
					  AlphaBlend::BLEND_TYPE eSrc,float& fR,float &fG,float &fB,float &fA)
{
	switch (eSrc)
	{
	case AlphaBlend::ZERO:
		fR = 0.0;
		fG = 0.0;
		fB = 0.0;
		fA = 0.0;
		break;

	case AlphaBlend::ONE:
		fR = 1.0;
		fG = 1.0;
		fB = 1.0;
		fA = 1.0;
		break;

	case AlphaBlend::SRC_COLOR:
		fR = srcColor[0];
		fG = srcColor[1];
		fB = srcColor[2];
		fA = srcColor[3];
		break;

	case AlphaBlend::ONE_MINUS_SRC_COLOR:
		fR = 1.0-srcColor[0];
		fG = 1.0-srcColor[1];
		fB = 1.0-srcColor[2];
		fA = 1.0-srcColor[3];
		break;

	case AlphaBlend::DST_COLOR:
		fR = dstColor[0];
		fG = dstColor[1];
		fB = dstColor[2];
		fA = dstColor[3];
		break;

	case AlphaBlend::ONE_MINUR_DST_COLOR:
		fR = 1.0-dstColor[0];
		fG = 1.0-dstColor[1];
		fB = 1.0-dstColor[2];
		fA = 1.0-dstColor[3];
		break;

	case AlphaBlend::SRC_ALPHA:
		fR = srcColor[3];
		fG = fR;
		fB = fR;
		fA = fR;
		break;

	case AlphaBlend::ONE_MINUS_SRC_ALPHA:
		fR = 1.0-srcColor[3];
		fG = fR;
		fB = fR;
		fA = fR;
		break;

	case AlphaBlend::DST_ALPHA:
		fR = dstColor[3];
		fG = fR;
		fB = fR;
		fA = fR;
		break;

	case AlphaBlend::ONE_MINUS_DST_ALPHA:
		fR = 1.0-dstColor[3];
		fG = fR;
		fB = fR;
		fA = fR;
		break;
	}
}

ColorRGBA AlphaBlend::ColorBlend(const ColorRGBA& srcColor,BLEND_TYPE eSrc,
								 const ColorRGBA& dstColor,BLEND_TYPE eDst)
{
	//首先将混合因子设为默认情况
	float fSrcR = 1.0;
	float fSrcG = 1.0;
	float fSrcB = 1.0;
	float fSrcA = 1.0;

	float fDstR = 0.0;
	float fDstG = 0.0;
	float fDstB = 0.0;
	float fDstA = 0.0;

	GetFactor(srcColor,dstColor,eSrc,fSrcR,fSrcG,fSrcB,fSrcA);
	GetFactor(srcColor,dstColor,eDst,fDstR,fDstG,fDstB,fDstA);

	float fRed = srcColor[0]*fSrcR + dstColor[0]*fDstR;
	float fGreen = srcColor[1]*fSrcG + dstColor[1]*fDstG;
	float fBlue = srcColor[2]*fSrcB + dstColor[2]*fDstB;
	float fAlpha = srcColor[3]*fSrcA + dstColor[3]*fDstA;

	return ColorRGBA(fRed,fGreen,fBlue,fAlpha);
	
}

NS_MATH3D_END
