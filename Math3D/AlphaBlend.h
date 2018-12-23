#ifndef MATH3D_ALPHABLEND_INCLUDE_H_
#define MATH3D_ALPHABLEND_INCLUDE_H_

#include "ColorRGBA.h"

NS_MATH3D_BEGIN

//颜色混合算法的类
class MATH3D_API AlphaBlend
{
public:
	enum BLEND_TYPE
	{
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUR_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA
	};

	static ColorRGBA ColorBlend(
		const ColorRGBA& srcColor,
		BLEND_TYPE eSrc,
		const ColorRGBA& dstColor,
		BLEND_TYPE eDst);
};

NS_MATH3D_END

#endif
