/**
* @file              DistanceArea.h
* @brief             �������ϼ�����������Ľӿ��ļ�
* @details           
* @author            zxg 
* @date              2015��5��15��
* @version           1.0.0.1
* @par               Copyright (c):����� 2015
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
	* @brief SetEllipsePara ���ü�������Ĳ���
	* @param[in] double a ������
	* @param[in] double b �̰���
	* @return void
	* @author zxg
	* @date 2015��5��15��
	* @note 
	*/
	void SetEllipsePara(double a,double b);

	/**
	* @brief ComputePolygonArea ��������������϶���ε����
	* @param[in]  const double *padX X��������
	* @param[in] const double* padY Y��������
	* @param[in] int nCount ��ĸ���
	* @return double ����ֵ�����
	* @author zxg
	* @date 2015��5��15��
	* @note 
	*/
	double ComputePolygonArea( const double *padX,const double* padY,int nCount );

private:

    double mSemiMajor, mSemiMinor, mInvFlattening;

    double GetQ( double x );
    double GetQbar( double x );

	void ComputeAreaInit();

    // ���������ʱ����

    double m_QA, m_QB, m_QC;
    double m_QbarA, m_QbarB, m_QbarC, m_QbarD;
    double m_AE;  /* a^2(1-e^2) */
    double m_Qp;  
    double m_E;  
    double m_TwoPI;

};


#endif // end of __DISTANCEAREA_H_C0C6C4C8_B1C4_49C3_87E4_EEF1ABC862F0__
