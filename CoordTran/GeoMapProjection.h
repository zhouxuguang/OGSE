#ifndef GEOMAPPROJECTION_INCLUDE_0214
#define GEOMAPPROJECTION_INCLUDE_0214

#include "GeoEllipsoid.h"


class COORDTRAN_API GeoMapProjection
{
public:
	GeoMapProjection(const GeoEllipsoid& ellipsoid ,
		double dbOriginLat = 0,double dbOriginLon = 0);

	GeoMapProjection(const GeoMapProjection& src);

	virtual ~GeoMapProjection(void);

	virtual void GetOrigin(double &dbOriginLat,double &dbOriginLon)const;

	virtual bool ForwardTransform(double dbLon,double dbLat,double &dbEast,double &dbNorth) const = 0;

	virtual bool InverseTransform(double dbEast,double dbNorth,double &dbLon,double &dbLat) const = 0;

	virtual double GetFalseEasting() const;

	virtual double GetFalseNorthing() const;

	virtual double GetStandardParallel1() const;

	virtual double GetStandardParallel2() const;

	virtual void Update();

	virtual std::string GetProjectionName() const;

	virtual double GetA() const;
	virtual double GetB() const;
	virtual double GetF() const;

	const GeoEllipsoid& GetEllipsoid() const ;
	virtual bool IsGeographic()const;

	virtual void SetEllipsoid(const GeoEllipsoid& ellipsoid);
	virtual void SetAB(double a, double b);

protected:
	double m_dbOriginLon;		//起始点经度
	double m_dbOriginLat;
	//double m_dbFalseEasting;		//假东偏移
	//double m_dbFalseNorthing;		//假北偏移

	GeoEllipsoid m_Ellipsoid;

	double m_dbA;
	double m_dbB;
	double m_dbF;

};

#endif
