/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: GeoSrsNode.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年11月23日
@ 内容摘要	: 空间参考系统节点数据结构
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOSRSNODE_H_A103473F_DC3C_48A6_A284_D69D7B23B561__
#define __GEOSRSNODE_H_A103473F_DC3C_48A6_A284_D69D7B23B561__

#include <string>
#include "GeoDefine.h"


class GEOMETRY_API GeoSrsNode
{
public:
	GeoSrsNode(const char * pszValue);
	~GeoSrsNode(void);

	bool IsLeafNode() const;

	bool NeedQuoting() const;

	int GetChildCount() const ;
	GeoSrsNode *GetChild( int );
	const GeoSrsNode *GetChild( int ) const;

	GeoSrsNode *GetNode( const char * );
	const GeoSrsNode *GetNode( const char * ) const;

	void InsertChild( GeoSrsNode *, int );
	void AddChild( GeoSrsNode * );
	int  FindChild( const char * ) const;
	void DestroyChild( int );
	void ClearChildren();

	const char *GetValue() const ;
	void  SetValue( const char * );

	GeoSrsNode *Clone() const;

	bool ImportFromWkt( char ** );
	bool ExportToWkt(char ** ppszResult) const;
	bool ExportToPrettyWkt(char ** ppszResult,int nDepth) const;

private:
	char* m_pszValue;					//节点的值
	GeoSrsNode* m_pParentNode;			//父节点
	GeoSrsNode** m_pChildNodes;			//子节点列表
	int m_nChildNum;					//子节点个数
};

#endif // end of __GEOSRSNODE_H_A103473F_DC3C_48A6_A284_D69D7B23B561__
