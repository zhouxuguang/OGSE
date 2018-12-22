/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: CG_Angle.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年12月22日
@ 内容摘要	: 角度计算函数（弧度和角度转换等）
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_ANGLE_H_E3D8A46D_8AC8_4787_AB4E_DF66EC40138B__
#define __CG_ANGLE_H_E3D8A46D_8AC8_4787_AB4E_DF66EC40138B__

#include "CG_Common.h"


class COMPUTEGEOMETRY_API CG_Angle
{
public:
	static double toDegrees(double radians);

	
	static double toRadians(double angleDegrees);

	static double angle(const SPOINT& p0,
	                    const SPOINT& p1);

	static double angle(const SPOINT& p);

	/// Tests whether the angle between p0-p1-p2 is acute.
	//
	/// An angle is acute if it is less than 90 degrees.
	/// 
	/// Note: this implementation is not precise (determistic) for
	///       angles very close to 90 degrees.
	///
	/// @param p0 an endpoint of the angle
	/// @param p1 the base of the angle
	/// @param p2 the other endpoint of the angle
	///
	static bool isAcute(const SPOINT& p0,
	                    const SPOINT& p1,
	                    const SPOINT& p2);

	/// Tests whether the angle between p0-p1-p2 is obtuse.
	//
	/// An angle is obtuse if it is greater than 90 degrees.
	/// 
	/// Note: this implementation is not precise (determistic) for
	///       angles very close to 90 degrees.
	///
	/// @param p0 an endpoint of the angle
	/// @param p1 the base of the angle
	/// @param p2 the other endpoint of the angle
	///
	static bool isObtuse(const SPOINT& p0,
	                     const SPOINT& p1,
	                     const SPOINT& p2);

	/// Returns the unoriented smallest angle between two vectors.
	//
	/// The computed angle will be in the range [0, Pi).
	///
	/// @param tip1 the tip of one vector
	/// @param tail the tail of each vector
	/// @param tip2 the tip of the other vector
	/// @return the angle between tail-tip1 and tail-tip2
	///
	static double angleBetween(const SPOINT& tip1,
	                           const SPOINT& tail,
	                           const SPOINT& tip2);

	/// Returns the oriented smallest angle between two vectors.
	//
	/// The computed angle will be in the range (-Pi, Pi].
	/// A positive result corresponds to a counterclockwise rotation
	/// from v1 to v2;
	/// a negative result corresponds to a clockwise rotation.
	///
	/// @param tip1 the tip of v1
	/// @param tail the tail of each vector
	/// @param tip2 the tip of v2
	/// @return the angle between v1 and v2, relative to v1
	///
	static double angleBetweenOriented(const SPOINT& tip1,
	                                   const SPOINT& tail,
	                                   const SPOINT& tip2);

	/// Computes the interior angle between two segments of a ring.
	//
	/// The ring is assumed to be oriented in a clockwise direction.
	/// The computed angle will be in the range [0, 2Pi]
	///
	/// @param p0
	///          a point of the ring
	/// @param p1
	///          the next point of the ring
	/// @param p2
	///          the next point of the ring
	/// @return the interior angle based at <code>p1</code>
	///
	static double interiorAngle(const SPOINT& p0,
	                            const SPOINT& p1,
	                            const SPOINT& p2);

	/// \brief
	/// Returns whether an angle must turn clockwise or counterclockwise
	/// to overlap another angle.
	///
	/// @param ang1 an angle (in radians)
	/// @param ang2 an angle (in radians)
	/// @return whether a1 must turn CLOCKWISE, COUNTERCLOCKWISE or
	///         NONE to overlap a2.
	///
	static int getTurn(double ang1, double ang2);

	/// \brief
	/// Computes the normalized value of an angle, which is the
	/// equivalent angle in the range ( -Pi, Pi ].
	///
	/// @param angle the angle to normalize
	/// @return an equivalent angle in the range (-Pi, Pi]
	///
	static double normalize(double angle);

	/// \brief
	/// Computes the normalized positive value of an angle,
	/// which is the equivalent angle in the range [ 0, 2*Pi ).
	///
	/// E.g.:
	/// - normalizePositive(0.0) = 0.0
	/// - normalizePositive(-PI) = PI
	/// - normalizePositive(-2PI) = 0.0
	/// - normalizePositive(-3PI) = PI
	/// - normalizePositive(-4PI) = 0
	/// - normalizePositive(PI) = PI
	/// - normalizePositive(2PI) = 0.0
	/// - normalizePositive(3PI) = PI
	/// - normalizePositive(4PI) = 0.0
	///
	/// @param angle the angle to normalize, in radians
	/// @return an equivalent positive angle
	///
	static double normalizePositive(double angle);


	/// Computes the unoriented smallest difference between two angles.
	//
	/// The angles are assumed to be normalized to the range [-Pi, Pi].
	/// The result will be in the range [0, Pi].
	///
	/// @param ang1 the angle of one vector (in [-Pi, Pi] )
	/// @param ang2 the angle of the other vector (in range [-Pi, Pi] )
	/// @return the angle (in radians) between the two vectors
	///         (in range [0, Pi] )
	///
	static double diff(double ang1, double ang2);
};


#endif //
