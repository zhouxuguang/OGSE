#include "Vector2.h"

NS_MATH3D_BEGIN

//Vector2::Vector2(void)
//{
//}



Vector2::Vector2()
{

}

Vector2::Vector2(const Real fX, const Real fY) : x( fX ), y( fY )
{

}

Vector2::Vector2(const Real scaler) : x( scaler), y( scaler )
{

}

Vector2::Vector2(const Real afCoordinate[2]) : x( afCoordinate[0] ),
y( afCoordinate[1] )
{

}

Vector2::Vector2(const int afCoordinate[2])
{
	x = (Real)afCoordinate[0];
	y = (Real)afCoordinate[1];
}

Vector2::Vector2(Real* const r) : x( r[0] ), y( r[1] )
{

}

Vector2::~Vector2(void)
{

}

void Vector2::Swap(Vector2& other)
{
	std::swap(x, other.x);
	std::swap(y, other.y);
}

Real Vector2::Length() const
{
	return sqrt( x * x + y * y );
}

Real Vector2::SquaredLength() const
{
	return x * x + y * y;
}

Real Vector2::Distance(const Vector2& rhs) const
{
	return (*this - rhs).Length();
}

Real Vector2::SquaredDistance(const Vector2& rhs) const
{
	return (*this - rhs).SquaredLength();
}

Real Vector2::DotProduct(const Vector2& vec) const
{
	return x * vec.x + y * vec.y;
}

Real Vector2::Normalise()
{
	Real fLength = sqrt( x * x + y * y);
	if ( fLength > Real(0.0f) )
	{
		Real fInvLength = 1.0f / fLength;
		x *= fInvLength;
		y *= fInvLength;
	}

	return fLength;
}

Real Vector2::CrossProduct(const Vector2& rkVector) const
{
	return x * rkVector.y - y * rkVector.x;
}

Vector2 Vector2::Reflect(const Vector2& normal) const
{
	return Vector2( *this - ( 2 * this->DotProduct(normal) * normal ) );
}

NS_MATH3D_END
