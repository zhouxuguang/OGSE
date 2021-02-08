#ifndef GeoTRANMERC_INCLUDE_H
#define GeoTRANMERC_INCLUDE_H



#ifdef __cplusplus
extern "C" {
#endif

	//���ݾ�γ�Ȼ�ô��ź��ϱ�����
    bool GetUTMZone(double lat, double lon, int& zone,bool& isNorth);
    
    //��׵�γ��
    double GetBFLat(double sm_a,double sm_b,double dbY);

	//�����߻��� phi�ǻ���
	double ArcLengthOfMeridian(double sm_a,double sm_b,double phi);

	//��������ת��Ϊƽ������ phi lambda,lambda0������
    void MapLatLonToXY (double sm_a,double sm_b,double phi, double lambda, double lambda0, double* xy);

	//�������תƽ������
	bool GeodeicalToMercator(double sm_a,double sm_b,
		double dbLat, double dbLon, double dbLon0,
		double& easting,
		double& northing);

	//Mercatorת�������� dbLon0����
	bool MercatorToGeodeical(double sm_a,double sm_b,double dbX, double dbY, 
		double dbLon0,
		double& dbLat,
		double& dbLon);

#ifdef __cplusplus
}
#endif

#endif 
