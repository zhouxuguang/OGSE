/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoLineString.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��26��
* ����ժҪ	: �����࣬OGC��׼�����LINESTRING
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__
#define __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__

/**********************************  ͷ�ļ� ************************************/

class GeoPoint;
class GeoEnvelope;

class GEOMETRY_API GeoCurve : public GeoGeometry
{
public:
	GeoCurve(){}

	virtual ~GeoCurve(){}

	virtual double GetLength() const = 0;

	virtual void GetStartPoint(GeoPoint* poPoint) const = 0;

	virtual void GetEndPoint(GeoPoint* poPoint) const = 0;

	virtual bool IsClosed() const{return false;}

};

class GEOMETRY_API GeoLineString : public GeoCurve
{
public:
	GeoLineString(void);
	GeoLineString( const GeoLineString * poSrcRing );
	virtual ~GeoLineString(void);

	/*��׼�ӿ�*/
	virtual int GetDimension() const;   //�������ά��
	virtual bool Empty();	  //��ռ�����
	virtual GeoWkbGeometryType GetGeometryType() const;  //��ȡ�����������
	virtual const char *GetGeometryName() const;//��ȡ�����������
	virtual void GetEnvelope(GeoEnvelope *env) const; //��ȡ���������С�������

	//�ƶ��ߴ�
	virtual void Move(double dx,double dy);
	virtual double GetArea() const;
	virtual double GetLength() const;

	//OGC��ʽ����
	virtual std::string AsText();			//ת��ΪWKT��ʽ
	virtual int WkbByteSize() const;
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const;

	//clone
	virtual GeoGeometry *Clone() const;

	//LINESTRING�ķ���
	//��ø���
	int GetNumPoint() const;
	//����
	virtual double Length() const;
	//��һ��������һ����
	virtual void GetStartPoint(GeoPoint* poPoint) const;
	virtual void GetEndPoint(GeoPoint* poPoint) const;
	//�õ���i�������xֵ
	double GetX(int i) const;
	//�õ���i�������yֵ
	double GetY(int i) const;
	//����X����
	void SetX(int i,double xIn);
	//����Y����
	void SetY(int i,double yIn);

	//�õ���i�������
	void GetPoint(int i,GeoPoint *poPoint) const;
	void GetPoint(int i,double &x,double &y) const;
	void GetPoint(int i,GeoCoordinate& coord) const;

	//�����������
	void GetPoints( GeoCoordinate *poCoords, double *paZ = NULL ) const;
	void GetPoints( double* padX, double* padY, double* padZ = NULL ) const;
	void GetPoints( GeoCoordinate *poCoords, double* padZ ,int nStride = 0) const;
	void GetPoints( double* padX, double* padY, double* padZ ,int nStride = 0) const;
	void GetPointArray(double *padVertexs) const;

	GeoCoordinate* GetPointsBuf() const;

	//����ָ���������
	void SetPoint(int i,GeoPoint* poPoint);
	void SetPoint(int i,double x,double y);
	void SetPoint(int i,GeoCoordinate *poPoint);

	void SetPoints(int nPoints, GeoCoordinate *paoPointsIn);
	void SetPoints(int nPoints,double* padX,double* padY,double* padZ = NULL);

	//������С�������
	void SetEnvelope(GeoEnvelope &env);

	//���㵽��һ����ľ���
	double DistanceToPoint(GeoPoint &poPoint);
	double DistanceToPoint(double x,double y);

	//�ж�һ���ߴ��Ƿ���һ��������
	int IsLineInRect(GeoEnvelope &env);


	//��ӵ㵽�ߴ���
	void AddPoint(GeoPoint& poPoint);
	void AddPoint(double x,double y);
	void AddPoint(GeoCoordinate& poPoint);

	//ָ����ĸ���
	void SetPointsNumber(int nNewPointCount);

	//�Ƿ�պ�
	bool IsClosed() const;

	//�Ƿ��Ǽ򵥵��ߴ�
	bool IsSimple() const;

	//�Ƿ�Ϊ��
	bool IsEmpty() const;

	//���Ƿ���ʱ������
	bool IsClockWise() const;

	//�����߱պ�
	void CloseRings();

	//�������˳��,��ʼ��ͽ����㽻����
	void ReversePoints( void );


private:

	int         m_nPointCount;			//�����ĸ���
	int			m_nCurMaxPointCount;	//��ǰ����ĸ�������
	GeoCoordinate *m_paoPoints;			//�����������
};

#endif // end of __GEOLINESTRING_H_BD4CA47E_DA05_4800_8CEC_90035368CDE7__
