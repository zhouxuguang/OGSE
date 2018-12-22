/**
* @file              SysGraphLayer.h
* @brief             标绘相关图元的图层
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年8月5日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSGRAPHLAYER_H_FCE6CCD4_0971_4F3C_AA5D_D4F87485AD63__
#define __SYSGRAPHLAYER_H_FCE6CCD4_0971_4F3C_AA5D_D4F87485AD63__

#include "SysMapLayer.h"

namespace SysDraw
{
	class SysGraphElement;

	class SYSDRAW_API SysGraphLayer : public SysMapLayer
	{
	public:
		explicit SysGraphLayer(std::string strLyrName);

		~SysGraphLayer(void);

		/**
		* @brief 添加图元
		* @param[in] SysGraphElement* pElement
		* @return void
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		void AddGraphicElement(SysGraphElement* pElement);

		/**
		* @brief 获得第nIndex个图元
		* @param[in] int nIndex
		* @return SysGraphElement*
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		SysGraphElement* GetGraphicElement(int nIndex) const;

		/**
		* @brief 获得图元个数
		* @return size_t
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		size_t GetGraphicElementCount() const;

		/**
		* @brief 删除第nIndex个图元
		* @param[in] int nIndex
		* @return bool
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		bool RemoveGraphicElement(int nIndex);

		/**
		* @brief 移除所有元素
		* @return void
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		void RemoveAllGraphicElement();

		/**
		* @brief 绘制所有图元
		* @param[in] SysRenderContext* rendererContext
		* @return bool
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		bool Draw( SysRenderContext* rendererContext ) ;

		/**
		* @brief 绘制第nIndex个图元
		* @param[in] int nIndex
		* @param[in] SysRenderContext* rendererContext
		* @return bool
		* @author 周旭光
		* @date 2015年8月11日
		* @note 
		*/
		bool DrawElement(int nIndex, SysRenderContext* rendererContext ) const;

	private:
		std::vector<SysGraphElement*> m_vecGraphicElement; 
	};

}

#endif // end of __SYSGRAPHLAYER_H_FCE6CCD4_0971_4F3C_AA5D_D4F87485AD63__
