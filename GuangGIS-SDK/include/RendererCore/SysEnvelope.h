/**
* @file              SysEnvelope.h
* @brief             地理空间矩形类，可以表达几何体的轴对齐最小外界矩形
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2012年1月8日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2012
* @par               History:
*/
#ifndef __SYSENVELOPE_H_33EFCD04_B733_480C_A506_AF7BB60CD0BB__
#define __SYSENVELOPE_H_33EFCD04_B733_480C_A506_AF7BB60CD0BB__

#include "SysDrawCommon.h"

namespace SysDraw
{

	class SysCoordinate;

	class SYSDRAW_API SysEnvelope
	{
	public:
		/**
		* @brief 默认构造函数
		* @return 
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		SysEnvelope();

		/**
		* @brief 带参数的构造函数
		* @param[in] double minX
		* @param[in] double maxX
		* @param[in] double minY
		* @param[in] double maxY
		* @return 
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		SysEnvelope(double minX,double maxX,double minY,double maxY);

		/**
		* @brief 拷贝构造函数
		* @param[in] const SysEnvelope& envelope
		* @return 
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		SysEnvelope(const SysEnvelope& envelope);

		/**
		* @brief 重载赋值运算符
		* @param[in] const SysEnvelope& other
		* @return SysEnvelope
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		SysEnvelope &operator=(const SysEnvelope& other);

		/**
		* @brief 用两个坐标点初始化
		* @param[in] SysCoordinate *coord1
		* @param[in] SysCoordinate *coord2
		* @return 
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		SysEnvelope(SysCoordinate *coord1,SysCoordinate *coord2);

		~SysEnvelope(void);

		/**
		* @brief 初始化矩形
		* @param[in] double x1
		* @param[in] double x2
		* @param[in] double y1
		* @param[in] double y2
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void Init(double x1,double x2,double y1,double y2);


		/**
		* @brief 判断矩形是否为空
		* @param[in] void
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool IsNull(void) const;

		/**
		* @brief 设置矩形为空
		* @param[in] void
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void SetNull(void);

		/**
		* @brief 获取最小外包矩形的宽度
		* @param[in] void
		* @return double
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		double GetWidth(void) const;

		/**
		* @brief 获取最小外界矩形的高度
		* @param[in] void
		* @return double
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		double GetHeight(void) const;

		/**
		* @brief 获得矩形的中心点坐标
		* @param[out] SysCoordinate &coord
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Center(SysCoordinate &coord) const;

		/**
		* @brief 获得矩形的中心点坐标
		* @param[out] double &x
		* @param[out] double &y
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Center(double &x,double &y) const;

		/**
		* @brief 测试是否包含另一个MBR
		* @param[in] const SysEnvelope &env
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Contains(const SysEnvelope &env) const;

		/**
		* @brief 判断一个点是否在该矩形中
		* @param[in] const SysCoordinate &pt
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Contains(const SysCoordinate &pt) const;

		/**
		* @brief 是否包含这个点
		* @param[in] double x
		* @param[in] double y
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Contains(double x, double y) const;

		/**
		* @brief 判断一个点是否在矩形内
		* @param[in] double x
		* @param[in] double y
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool IsPointInRect(double x,double y) const;

		/**
		* @brief 计算两个矩形相交的部分
		* @param[in] const SysEnvelope& env
		* @param[out] SysEnvelope &envResult
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Intersection(const SysEnvelope& env,SysEnvelope &envResult) const;

		/**
		* @brief 计算两个矩形是否相交
		* @param[in] const SysEnvelope *pOther
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Intersects(const SysEnvelope *pOther) const;

		/**
		* @brief 计算两个矩形是否相交
		* @param[in] const SysEnvelope &env
		* @return bool
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		bool Intersects(const SysEnvelope &env) const;

		/**
		* @brief 矩形增长
		* @param[in] double x
		* @param[in] double y
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void ExpandToInclude(double x, double y);

		/**
		* @brief 矩形增长
		* @param[in] const SysCoordinate &pt
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void ExpandToInclude(const SysCoordinate &pt);

		/**
		* @brief 矩形增长
		* @param[in] const SysEnvelope &other
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void ExpandToInclude(const SysEnvelope &other);

		/**
		* @brief 矩形增长
		* @param[in] const SysEnvelope *other
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void ExpandToInclude(const SysEnvelope *other);

		/**
		* @brief 计算到另一个MBR的距离
		* @param[in] const SysEnvelope &env
		* @return double
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		double DistanceTo(const SysEnvelope &env) const;

		/**
		* @brief 计算面积
		* @return double
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		double GetArea() const;

		/**
		* @brief 计算周长
		* @return double
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		double Perimeter() const;

		/**
		* @brief 变换，平移
		* @param[in] double transX
		* @param[in] double transY
		* @return void
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		void Translate(double transX, double transY);

		/**
		* @brief 判断p1,p2构成的矩形和q1,q2构成的矩形是否相交
		* @param[in] const SysCoordinate &p1
		* @param[in] const SysCoordinate &p2
		* @param[in] const SysCoordinate &q1
		* @param[in] const SysCoordinate &q2
		* @return staticbool 
		* @author 周旭光
		* @date 2012年1月8日
		* @note 
		*/
		static bool Intersects(const SysCoordinate &p1, const SysCoordinate &p2, const SysCoordinate &q1, const SysCoordinate &q2);

	public:
		double minX;	//最小外包矩形的最小x值
		double maxX;	//最小外包矩形的最大x值
		double minY;	//最小外包矩形的最小y值
		double maxY;	//最小外包矩形的最大y值
	};

}

#endif // end of 
