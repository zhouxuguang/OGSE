/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoGeometryCollection.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��6��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__
#define __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__

/**********************************  ͷ�ļ� ************************************/

class GeoGeometry;
class GeoEnvelope;
class GeoPoint;
class GeoLineString;
class GeoPolygon;


class GEOMETRY_API GeoGeometryCollection : public GeoGeometry
{

public:
	GeoGeometryCollection(void);
	virtual ~GeoGeometryCollection(void);

public:
	//��׼�ӿ�
	virtual int GetDimension() const;   //�������ά��
	virtual bool Empty();	  //��ռ�����
	virtual GeoWkbGeometryType GetGeometryType() const;  //��ȡ�����������
	virtual const char *GetGeometryName() const;		//��ȡ�����������
	virtual void GetEnvelope(GeoEnvelope *env) const;	//��ȡ���������С�������
	virtual void Move(double dx,double dy);				//�ƶ�������
	virtual double GetArea() const;							//�������
	virtual double GetLength() const;							//���㳤��

	virtual std::string AsText();			//ת��ΪWKT��ʽ
	virtual int WkbByteSize() const;

	//�Ǳ�׼�ӿ�
	int GetNumGeoms() const;						//��ȡ������ĸ���
	GeoGeometry* GetGeometryRefs(int nIndex);		//���ݱ�Ż�ȡ������
	const GeoGeometry* GetGeometryRefs(int nIndex) const;

	bool AddGeometry(GeoGeometry *poGeom);
	bool RemoveGeometry(int iGeom,bool bDelete);

private:
	int nGeomCount;				//������ĸ���
	GeoGeometry **papoGeoms;	//�����弯��
};

#endif // end of __GEOGEOMETRYCOLLECTION_H_34DCA488_27DB_47FB_B148_EF38DAB57A14__
