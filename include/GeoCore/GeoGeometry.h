/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoGeometry.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��6��3��
* ����ժҪ	: �����������
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__
#define __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__


#include <string>
#include "GeoDefine.h"


class GeoPoint;
class GeoEnvelope;
class GeoSpatialReference;

class GEOMETRY_API GeoGeometry
{
public:
	GeoGeometry(void);
	virtual ~GeoGeometry(void);

	/*��׼�ӿ�*/
	virtual int GetDimension() const = 0;   //�������ά��
	virtual bool Empty() = 0;	  //��ռ�����
	virtual GeoWkbGeometryType GetGeometryType() const = 0;  //��ȡ�����������
	virtual const char *GetGeometryName() const = 0;//��ȡ�����������
	virtual void GetEnvelope(GeoEnvelope *env) const = 0; //��ȡ���������С�������
	virtual void Move(double dx,double dy) = 0;			//�ƶ�������
	virtual double GetArea() const = 0;							//�������
	virtual double GetLength() const = 0;							//���㳤��
	//virtual void ToKML(std::string &strKML) = 0;							//ת��ΪKML

	//��¡����
	virtual GeoGeometry *Clone() const = 0;

	//OGC��ʽ����
	virtual int WkbByteSize() const = 0;
	virtual std::string AsText() = 0;							//ת��ΪWKT
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const
	{
		return 1;
	}

	void SetSpatialReference( GeoSpatialReference *poSR );
	GeoSpatialReference *GetSpatialReference( ) const;

	// �ռ��ϵ
	virtual bool  Intersects( GeoGeometry * ) const;
	virtual bool  Equals( GeoGeometry * ) const;
	virtual bool  Disjoint( const GeoGeometry * ) const;
	virtual bool  Touches( const GeoGeometry * ) const;
	virtual bool  Crosses( const GeoGeometry * ) const;
	virtual bool  Within( const GeoGeometry * ) const;
	virtual bool  Contains( const GeoGeometry * ) const;
	virtual bool  Overlaps( const GeoGeometry * ) const;
	virtual bool  Relate( const GeoGeometry *, const char * ) const;

	//�ռ����
	virtual GeoGeometry *Boundary() const;
	virtual double  Distance( const GeoGeometry * ) const;
	virtual GeoGeometry *ConvexHull() const;
	virtual GeoGeometry *Buffer( double dfDist, int nQuadSegs = 30 ) const;
	virtual GeoGeometry *Intersection( const GeoGeometry *) const;
	virtual GeoGeometry *Union( const GeoGeometry * ) const;
	virtual GeoGeometry *UnionCascaded() const;
	virtual GeoGeometry *Difference( const GeoGeometry * ) const;
	virtual GeoGeometry *SymDifference( const GeoGeometry * ) const;
	virtual int       Centroid( GeoPoint * poPoint ) const;
	virtual GeoGeometry *Simplify(double dTolerance) const;
	GeoGeometry *SimplifyPreserveTopology(double dTolerance) const;

	virtual GeoGeometry *Polygonize() const;

	// backward compatibility to non-standard method names. 
	bool  Intersect( GeoGeometry * ) const;
	bool  Equal( GeoGeometry * ) const;
	virtual GeoGeometry *SymmetricDifference( const GeoGeometry * ) const;
	virtual GeoGeometry *getBoundary() const;   


private:
	GeoSpatialReference * m_poSRS;                // �ռ�ο�ϵͳ������Ϊ��

protected:
	int m_nCoordDimension;				//�����ά��
	
};


#endif // end of __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__
