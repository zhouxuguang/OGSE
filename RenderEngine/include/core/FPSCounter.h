
#ifndef __C_FPSCOUNTER_H_INCLUDED__
#define __C_FPSCOUNTER_H_INCLUDED__


class FPSCounter
{
public:
	FPSCounter();

	//! returns current fps
	long long getFPS() const;

	//! returns primitive count
	long long getPrimitive() const;

	//! returns average primitive count of last period
	long long getPrimitiveAverage() const;

	//! returns accumulated primitive count since start
	long long getPrimitiveTotal() const;

	//! to be called every frame
	void registerFrame(long long now, long long primitive);

private:

	unsigned short FPS;
	long long Primitive;
	long long StartTime;

	long long FramesCounted;
	long long PrimitivesCounted;
	long long PrimitiveAverage;
	long long PrimitiveTotal;
};


#endif 

