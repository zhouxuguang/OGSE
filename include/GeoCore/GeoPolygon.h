/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: GeoPolygon.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��3��23��
@ ����ժҪ	: �����
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/

#ifndef __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__
#define __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__

class GeoPoint;
class GeoLineString;

class GEOMETRY_API GeoSurface : public GeoGeometry
{
public:
	virtual double GetArea() const = 0;

	virtual int PointInPolygon(const GeoPoint& poPoint) = 0;
};


class GEOMETRY_API GeoPolygon : public GeoSurface
{
public:
	//Ĭ�Ϲ��캯��
	GeoPolygon(void);
	virtual ~GeoPolygon(void);

	//�������캯��
	GeoPolygon(const GeoPolygon& other);
	GeoPolygon& operator= (const GeoPolygon& other);

	/*��׼�ӿ�*/
	virtual int GetDimension() const;   //�������ά��
	virtual bool Empty();	  //��ռ�����
	virtual GeoWkbGeometryType GetGeometryType() const;  //��ȡ�����������
	virtual const char * GetGeometryName() const;//��ȡ�����������
	virtual void GetEnvelope(GeoEnvelope *env) const; //��ȡ���������С�������
	virtual void Move(double dx,double dy);
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC��ʽ����
	virtual std::string AsText();
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//��¡
	virtual GeoGeometry *Clone() const;

	
	int GetNumRings() const;	  //���㻷�ĸ���
	int GetNumInteriorRings() const;	 //���㡰�����ĸ���

	GeoLineString* GetExteriorRing();
	const GeoLineString* GetExteriorRing() const;
	GeoLineString* GetInteriorRing(int iRing);
	const GeoLineString* GetInteriorRing(int iRing) const;
	void AddRing(const GeoLineString *poNewRing);
	GeoLineString** GetRings() const { return m_papoRings;}
	GeoLineString*  GetRing(int nIndex) const;

	//��������ĵ�
	virtual int Centroid( GeoPoint * poPoint ) const;

	//�жϵ��Ƿ��ڶ������
	int PointInPolygon(const GeoPoint& poPoint);
	//�ж϶�����Ƿ��ھ�����
	int PolygonInRect(GeoEnvelope &env);
	void CloseRings();

private:
	//���ݳ�Ա
	int m_nRingCount;				//���ĸ�����һ���⻷��0�������ڻ�������
	GeoLineString **m_papoRings;	//ָ�����ߵ�ָ��
};

#endif // end of __GEOPOLYGON_H_DEDB2644_0578_4CA9_9A8A_0EFAC37BDC13__
