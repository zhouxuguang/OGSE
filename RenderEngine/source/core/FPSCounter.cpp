
#include "FPSCounter.h"
#include <math.h>


FPSCounter::FPSCounter()
:	FPS(60), Primitive(0), StartTime(0), FramesCounted(0),
	PrimitivesCounted(0), PrimitiveAverage(0), PrimitiveTotal(0)
{

}

//! returns current fps
long long FPSCounter::getFPS() const
{
	return FPS;
}

//! returns current primitive count
long long FPSCounter::getPrimitive() const
{
	return Primitive;
}

//! returns average primitive count of last period
long long FPSCounter::getPrimitiveAverage() const
{
	return PrimitiveAverage;
}

//! returns accumulated primitive count since start
long long FPSCounter::getPrimitiveTotal() const
{
	return PrimitiveTotal;
}

//! to be called every frame
void FPSCounter::registerFrame(long long now, long long primitivesDrawn)
{
	++FramesCounted;
	PrimitiveTotal += primitivesDrawn;
	PrimitivesCounted += primitivesDrawn;
	Primitive = primitivesDrawn;

	const long long milliseconds = now - StartTime;

	if (milliseconds >= 1500 )
	{
		const float invMilli = milliseconds;
		
		FPS = ceil ( ( 1000 * FramesCounted ) * invMilli );
		PrimitiveAverage = ceil ( ( 1000 * PrimitivesCounted ) * invMilli );

		FramesCounted = 0;
		PrimitivesCounted = 0;
		StartTime = now;
	}
}
