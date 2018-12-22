#include "GeoCoordinate.h"
#include <math.h>

GeoCoordinate::GeoCoordinate(void)
{
	x = 0;
	y = 0;
}

GeoCoordinate::GeoCoordinate(double x, double y)
{
	this->x = x;
	this->y = y;
}

GeoCoordinate::GeoCoordinate(const GeoCoordinate& coord)
{
	this->x = coord.x;
	this->y = coord.y;
}

GeoCoordinate::~GeoCoordinate(void)
{
}

//计算到另一个坐标的距离
double GeoCoordinate::Distance(GeoCoordinate &coord) const
{
	double dx = x - coord.x;
	double dy = y - coord.y;
	return sqrt(dx*dx + dy*dy);
}

void GeoCoordinate::SetX(double x)
{
	this->x = x;
}

void GeoCoordinate::SetY(double y)
{
	this->y = y;
}
