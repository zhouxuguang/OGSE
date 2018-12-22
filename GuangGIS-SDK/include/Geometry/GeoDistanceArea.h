/**
* @file              DistanceArea.h
* @brief             椭球面上计算多边形面积的接口文件
* @details           
* @author            zxg 
* @date              2015年5月15日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __DISTANCEAREA_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__
#define __DISTANCEAREA_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__


class GeoDistanceArea
{
public:
	GeoDistanceArea();

    ~GeoDistanceArea();

	/**
	* @brief SetEllipsePara 设置计算面积的参数
	* @param[in] double a 长半轴
	* @param[in] double b 短半轴
	* @return void
	* @author zxg
	* @date 2015年5月15日
	* @note 
	*/
	void SetEllipsePara(double a,double b);

	/**
	* @brief ComputePolygonArea 计算地球椭球面上多边形的面积
	* @param[in]  const double *padX X坐标数组
	* @param[in] const double* padY Y坐标数组
	* @param[in] int nCount 点的个数
	* @return double 返回值是面积
	* @author zxg
	* @date 2015年5月15日
	* @note 
	*/
	double ComputePolygonArea( const double *padX,const double* padY,int nCount );

private:

    double mSemiMajor, mSemiMinor, mInvFlattening;

    double GetQ( double x );
    double GetQbar( double x );

	void ComputeAreaInit();

    // 面积计算临时变量

    double m_QA, m_QB, m_QC;
    double m_QbarA, m_QbarB, m_QbarC, m_QbarD;
    double m_AE;  /* a^2(1-e^2) */
    double m_Qp;  
    double m_E;  
    double m_TwoPI;

};


#endif // end of __DISTANCEAREA_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__
