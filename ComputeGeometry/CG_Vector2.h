/*******************************************************************************
* 版权所有(C) pyhcx 2013
* 文件名称	: Vector2.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2013年10月23日
* 内容摘要	: 主要用于二维矢量的计算
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __VECTOR2_H_2E808F26_70F1_4B31_99FD_D31AAB367DD7__
#define __VECTOR2_H_2E808F26_70F1_4B31_99FD_D31AAB367DD7__

/**********************************  头文件 ************************************/


#include "CG_Common.h"


class COMPUTEGEOMETRY_API CG_Vector2
{

public:
	double x;
	double y;

public:
    CG_Vector2()
    {
    }

    CG_Vector2( const double fX, const double fY )
        : x( fX ), y( fY )
    {
    }

    inline explicit CG_Vector2( const double afCoordinate[2] )
        : x( afCoordinate[0] ),
          y( afCoordinate[1] )
    {
    }

    inline explicit CG_Vector2( const int afCoordinate[2] )
    {
        x = (double)afCoordinate[0];
        y = (double)afCoordinate[1];
        
    }

	inline explicit CG_Vector2(const SPOINT& ptEnd)
	{
		x = ptEnd.x;
		y = ptEnd.y;
	}

	inline explicit CG_Vector2(const SPOINT& ptStart,const SPOINT& ptEnd)
	{
		x = ptEnd.x - ptStart.x;
		y = ptEnd.y - ptStart.y;
	}


    inline explicit CG_Vector2( const double scaler )
        : x( scaler )
        , y( scaler )
        
    {
    }

	~CG_Vector2(void)
	{
	}

	inline void swap(CG_Vector2& other)
	{
		std::swap(x, other.x);
		std::swap(y, other.y);
	}

	inline double operator [] ( const size_t i ) const
    {
        assert( i < 2 );

        return *(&x+i);
    }

	inline double& operator [] ( const size_t i )
    {
        assert( i < 2 );

        return *(&x+i);
    }

	inline double* ptr()
	{
		return &x;
	}

	inline const double* ptr() const
	{
		return &x;
	}

    inline CG_Vector2& operator = ( const CG_Vector2& rkVector )
    {
        x = rkVector.x;
        y = rkVector.y;
        //z = rkVector.z;

        return *this;
    }

    inline CG_Vector2& operator = ( const double fScaler )
    {
        x = fScaler;
        y = fScaler;

        return *this;
    }

    inline bool operator == ( const CG_Vector2& rkVector ) const
    {
        return ( x == rkVector.x && y == rkVector.y );
    }

    inline bool operator != ( const CG_Vector2& rkVector ) const
    {
        return ( x != rkVector.x || y != rkVector.y );
    }

    inline CG_Vector2 operator + ( const CG_Vector2& rkVector ) const
    {
        return CG_Vector2(
            x + rkVector.x,
            y + rkVector.y);
    }

    inline CG_Vector2 operator - ( const CG_Vector2& rkVector ) const
    {
        return CG_Vector2(
            x - rkVector.x,
            y - rkVector.y
			);
    }

    inline CG_Vector2 operator * ( const double fScalar ) const
    {
        return CG_Vector2(
            x * fScalar,
            y * fScalar);
    }

    inline CG_Vector2 operator * ( const CG_Vector2& rhs) const
    {
        return CG_Vector2(
            x * rhs.x,
            y * rhs.y);
    }

    inline CG_Vector2 operator / ( const double fScalar ) const
    {
        assert( fScalar != 0.0 );

        double fInv = 1.0f / fScalar;

        return CG_Vector2(
            x * fInv,
            y * fInv
			);
    }

    inline CG_Vector2 operator / ( const CG_Vector2& rhs) const
    {
        return CG_Vector2(
            x / rhs.x,
            y / rhs.y);
    }

    inline const CG_Vector2& operator + () const
    {
        return *this;
    }

    inline CG_Vector2 operator - () const
    {
        return CG_Vector2(-x, -y );
    }

    inline friend CG_Vector2 operator * ( const double fScalar, const CG_Vector2& rkVector )
    {
        return CG_Vector2(
            fScalar * rkVector.x,
            fScalar * rkVector.y);
    }

    inline friend CG_Vector2 operator / ( const double fScalar, const CG_Vector2& rkVector )
    {
        return CG_Vector2(
            fScalar / rkVector.x,
            fScalar / rkVector.y);
    }

    inline friend CG_Vector2 operator + (const CG_Vector2& lhs, const double rhs)
    {
        return CG_Vector2(
            lhs.x + rhs,
            lhs.y + rhs );
    }

    inline friend CG_Vector2 operator + (const double lhs, const CG_Vector2& rhs)
    {
        return CG_Vector2(
            lhs + rhs.x,
            lhs + rhs.y);
    }

    inline friend CG_Vector2 operator - (const CG_Vector2& lhs, const double rhs)
    {
        return CG_Vector2(
            lhs.x - rhs,
            lhs.y - rhs);
    }

    inline friend CG_Vector2 operator - (const double lhs, const CG_Vector2& rhs)
    {
        return CG_Vector2(
            lhs - rhs.x,
            lhs - rhs.y);
    }

    inline CG_Vector2& operator += ( const CG_Vector2& rkVector )
    {
        x += rkVector.x;
        y += rkVector.y;

        return *this;
    }

    inline CG_Vector2& operator += ( const double fScalar )
    {
        x += fScalar;
        y += fScalar;
        return *this;
    }

    inline CG_Vector2& operator -= ( const CG_Vector2& rkVector )
    {
        x -= rkVector.x;
        y -= rkVector.y;

        return *this;
    }

    inline CG_Vector2& operator -= ( const double fScalar )
    {
        x -= fScalar;
        y -= fScalar;
        return *this;
    }

    inline CG_Vector2& operator *= ( const double fScalar )
    {
        x *= fScalar;
        y *= fScalar;
        return *this;
    }

    inline CG_Vector2& operator *= ( const CG_Vector2& rkVector )
    {
        x *= rkVector.x;
        y *= rkVector.y;

        return *this;
    }

    inline CG_Vector2& operator /= ( const double fScalar )
    {
        assert( fScalar != 0.0 );

        double fInv = 1.0f / fScalar;

        x *= fInv;
        y *= fInv;

        return *this;
    }

    inline CG_Vector2& operator /= ( const CG_Vector2& rkVector )
    {
        x /= rkVector.x;
        y /= rkVector.y;

        return *this;
    }

    inline double length () const
    {
        return sqrt( x * x + y * y );
    }

    inline double squaredLength () const
    {
        return x * x + y * y ;
    }

    /** 
    */
    inline double distance(const CG_Vector2& rhs) const
    {
        return (*this - rhs).length();
    }

    /** 返回到其他另一个向量的平方距离
    */
    inline double squaredDistance(const CG_Vector2& rhs) const
    {
        return (*this - rhs).squaredLength();
    }

    /**矢量点乘
    */
    inline double dotProduct(const CG_Vector2& vec) const
    {
        return x * vec.x + y * vec.y;
    }

    /** 标准化向量
    */
    inline double normalise()
    {
        double fLength = sqrt( x * x + y * y );

        if ( fLength > 0.0f )
        {
            double fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
        }

        return fLength;
    }


	//矢量叉积
    inline CG_Vector2 crossProduct( const CG_Vector2& rkVector ) const
    {
        return CG_Vector2(0,0
            /*y * rkVector.z - z * rkVector.y,
            z * rkVector.x - x * rkVector.z,
            x * rkVector.y - y * rkVector.x*/);
    }

	static double crossProduct( CG_Vector2& rkVector1,CG_Vector2& rkVector2 )
    {
        return rkVector1.x * rkVector2.y - rkVector2.x * rkVector1.y;
    }

   
    inline CG_Vector2 midPoint( const CG_Vector2& vec ) const
    {
        return CG_Vector2(
            ( x + vec.x ) * 0.5f,
            ( y + vec.y ) * 0.5f );
    }

  
    inline bool operator < ( const CG_Vector2& rhs ) const
    {
        if( x < rhs.x && y < rhs.y )
		{
            return true;
		}
        return false;
    }

   
    inline bool operator > ( const CG_Vector2& rhs ) const
    {
        if( x > rhs.x && y > rhs.y )
		{
            return true;
		}
        return false;
    }
   
	
	//计算两个矢量之间的角度
	inline double angleBetween(const CG_Vector2& dest) const
	{
		double lenProduct = length() * dest.length();

		// 检查除数是否为0
		if(lenProduct < 1e-9f)
			lenProduct = 1e-9f;

		double f = dotProduct(dest) / lenProduct;

		return acos(f);

	}

    
    inline bool isZeroLength(void) const
    {
        double sqlen = (x * x) + (y * y);
        return (sqlen < (1e-06 * 1e-06));

    }

    
    inline CG_Vector2 normalisedCopy(void) const
    {
        CG_Vector2 ret = *this;
        ret.normalise();
        return ret;
    }

    
    inline CG_Vector2 reflect(const CG_Vector2& normal) const
    {
        return CG_Vector2( *this - ( 2 * this->dotProduct(normal) * normal ) );
    }	



	
    static const CG_Vector2 ZERO;
    static const CG_Vector2 UNIT_X;
    static const CG_Vector2 UNIT_Y;
    static const CG_Vector2 NEGATIVE_UNIT_X;
    static const CG_Vector2 NEGATIVE_UNIT_Y;
    static const CG_Vector2 UNIT_SCALE;
};

#endif // end of __VECTOR2_H_2E808F26_70F1_4B31_99FD_D31AAB367DD7__
