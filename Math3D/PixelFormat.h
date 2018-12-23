#ifndef MATH32_PIXELFORMAT_INCLUDE_H_FG5GK
#define MATH32_PIXELFORMAT_INCLUDE_H_FG5GK

#include "Math3DCommon.h"

enum PixelFormat
{
	/// Unknown pixel format.
	PF_UNKNOWN = 0,
	/// 8-bit pixel format, all bits luminance.
	PF_L8 = 1,
	PF_BYTE_L = PF_L8,
	/// 16-bit pixel format, all bits luminance.
	PF_L16 = 2,
	PF_SHORT_L = PF_L16,
	/// 8-bit pixel format, all bits alpha.
	PF_A8 = 3,
	PF_BYTE_A = PF_A8,
	
	/// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
	PF_R5G6B5 = 6,

	/// 24-bit pixel format, 8 bits for red, green and blue.
	PF_R8G8B8 = 10,
	/// 24-bit pixel format, 8 bits for blue, green and red.
	PF_B8G8R8 = 11,
	/// 32-bit pixel format, 8 bits for alpha, red, green and blue.
	PF_A8R8G8B8 = 12,
	
	/// 32-bit pixel format, 8 bits for red, green, blue and alpha.
	PF_R8G8B8A8 = 28,
	
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	/// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	PF_BYTE_RGB = PF_R8G8B8,
	/// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	PF_BYTE_BGR = PF_B8G8R8,

	/// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
	PF_BYTE_RGBA = PF_R8G8B8A8,
#else
	/// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	PF_BYTE_RGB = PF_B8G8R8,
	/// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	PF_BYTE_BGR = PF_R8G8B8,
	/// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
	PF_BYTE_BGRA = PF_A8R8G8B8,
	/// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
	PF_BYTE_RGBA = PF_A8B8G8R8,
#endif        
	
};


#endif