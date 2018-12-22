/**
* @file              SysMapDataLayer.h
* @brief             数据图层类
* @details           
* @author            周旭光 (zhouxuguang@aeroimginfo.com)
* @date              2015年6月26日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SYSMAPDATALAYER_H_FB417015_743C_4A9B_BC55_2D45657BC406__
#define __SYSMAPDATALAYER_H_FB417015_743C_4A9B_BC55_2D45657BC406__

#include "SysMapLayer.h"

namespace SysDraw
{


	class SYSDRAW_API SysMapDataLayer : public SysMapLayer
	{
	public:
		SysMapDataLayer(SysMapLayer::LayerType type, std::string strLyrName);
		virtual ~SysMapDataLayer(void);
	};


}


#endif // end of __SYSMAPDATALAYER_H_FB417015_743C_4A9B_BC55_2D45657BC406__
