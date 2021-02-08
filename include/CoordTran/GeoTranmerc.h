#ifndef GeoTRANMERC_INCLUDE_H
#define GeoTRANMERC_INCLUDE_H



#ifdef __cplusplus
extern "C" {
#endif

	//根据经纬度获得带号和南北半球
    bool GetUTMZone(double lat, double lon, int& zone,bool& isNorth);
    
    //求底点纬度
    double GetBFLat(double sm_a,double sm_b,double dbY);

	//子午线弧长 phi是弧度
	double ArcLengthOfMeridian(double sm_a,double sm_b,double phi);

	//地理坐标转换为平面坐标 phi lambda,lambda0弧度制
    void MapLatLonToXY (double sm_a,double sm_b,double phi, double lambda, double lambda0, double* xy);

	//大地坐标转平面坐标
	bool GeodeicalToMercator(double sm_a,double sm_b,
		double dbLat, double dbLon, double dbLon0,
		double& easting,
		double& northing);

	//Mercator转地理坐标 dbLon0弧度
	bool MercatorToGeodeical(double sm_a,double sm_b,double dbX, double dbY, 
		double dbLon0,
		double& dbLat,
		double& dbLon);

#ifdef __cplusplus
}
#endif

#endif 
