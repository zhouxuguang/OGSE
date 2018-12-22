
#include "CG_Angle.h"
#include <math.h>


double CG_Angle::toDegrees(double radians)
{
      return (radians * 180) / (M_PI);
}


double CG_Angle::toRadians(double angleDegrees)
{
	return (angleDegrees * M_PI) / 180.0;
}

double CG_Angle::angle(const SPOINT& p0,const SPOINT& p1)
{
      double dx = p1.x - p0.x;
      double dy = p1.y - p0.y;

	  double dbAngle = 0;

	  if (dx == 0 && dy > 0)
	  {
		  dbAngle = M_PI_2;
	  }

	  else if (dx == 0 && dy < 0)
	  {
		  dbAngle = M_PI_2 + M_PI;
	  }

	  else if (dx > 0 && dy >= 0)
	  {
		  dbAngle = atan2(dy,dx);
	  }

	  else if (dx > 0 && dy < 0)
	  {
		  dbAngle = atan2(dy,dx) + M_PI * 2;
	  }

	  else if (dx < 0)
	  {
		  dbAngle = atan2(dy,dx) + M_PI;
	  }
      return atan2(dy,dx);
}

double CG_Angle::angle(const SPOINT& p)
{
	return atan2(p.y, p.x);
}


bool CG_Angle::isAcute(const SPOINT& p0,
               const SPOINT& p1,
               const SPOINT& p2)
{
	// 根据向量的数量积来判断
	double dx0 = p0.x - p1.x;
	double dy0 = p0.y - p1.y;
	double dx1 = p2.x - p1.x;
	double dy1 = p2.y - p1.y;
	double dotprod = dx0 * dx1 + dy0 * dy1;
	return dotprod > 0;
}


bool CG_Angle::isObtuse(const SPOINT& p0,
                const SPOINT& p1,
                const SPOINT& p2)
{
	// 根据向量的数量积来判断
	double dx0 = p0.x - p1.x;
	double dy0 = p0.y - p1.y;
	double dx1 = p2.x - p1.x;
	double dy1 = p2.y - p1.y;
	double dotprod = dx0 * dx1 + dy0 * dy1;
	return dotprod < 0;
}


double CG_Angle::angleBetween(const SPOINT& tip1,
                    const SPOINT& tail,
                    const SPOINT& tip2)
{
	double a1 = angle(tail, tip1);
	double a2 = angle(tail, tip2);

	return diff(a1, a2);
}


double CG_Angle::angleBetweenOriented(const SPOINT& tip1,
                            const SPOINT& tail,
                            const SPOINT& tip2)
{
	double a1 = angle(tail, tip1);
	double a2 = angle(tail, tip2);
	double angDel = a2 - a1;

	// normalize, maintaining orientation
	if (angDel <= -M_PI)
		return angDel + 2 * M_PI;
	if (angDel > M_PI)
		return angDel - 2 * M_PI;
	return angDel;
}


double CG_Angle::interiorAngle(const SPOINT& p0, const SPOINT& p1,
                     const SPOINT& p2)
{
	double anglePrev = angle(p1, p0);
	double angleNext = angle(p1, p2);
	return fabs(angleNext - anglePrev);
}


int CG_Angle::getTurn(double ang1, double ang2)
{
	double crossproduct = sin(ang2 - ang1);

	if (crossproduct > 0) //逆时针
	{
		return 1;
	}
	if (crossproduct < 0) //顺时针
	{
		return 2;
	}
	return 0;
}


double CG_Angle::normalize(double angle)
{
	while (angle > M_PI)
		angle -= 2 * M_PI;
	while (angle <= -M_PI)
		angle += 2 * M_PI;
	return angle;
}


double CG_Angle::normalizePositive(double angle)
{
	if (angle < 0.0) 
	{
		while (angle < 0.0)
			angle += 2 * M_PI;
		// in case round-off error bumps the value over
		if (angle >= 2 * M_PI)
			angle = 0.0;
	}
	else 
	{
		while (angle >= 2 * M_PI)
			angle -= 2 * M_PI;
		// in case round-off error bumps the value under
		if (angle < 0.0)
			angle = 0.0;
	}
	return angle;
}


double CG_Angle::diff(double ang1, double ang2)
{
	double delAngle;

	if (ang1 < ang2) 
	{
		delAngle = ang2 - ang1;
	} 
	else 
	{
		delAngle = ang1 - ang2;
	}

	if (delAngle > M_PI) 
	{
		delAngle = (2 * M_PI) - delAngle;
	}

	return delAngle;
}


