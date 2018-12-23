#ifndef MATH3D_IMAGE_INCLUDE_3D_FG56HJ
#define MATH3D_IMAGE_INCLUDE_3D_FG56HJ

#include "PixelFormat.h"

class MATH3D_API Image
{
public:
	Image(void);

	~Image(void);

protected:
	// The width of the image in pixels
	size_t mWidth;
	// The height of the image in pixels
	size_t mHeight;
	// The depth of the image
	size_t mDepth;
	
	// The number of mipmaps the image contains
	size_t mNumMipmaps;
	// Image specific flags.
	int mFlags;

	// The pixel format of the image
	PixelFormat mFormat;

	// The number of bytes per pixel
	unsigned char mPixelSize;

	size_t mBufSize;
	unsigned char* mBuffer;
};

#endif
