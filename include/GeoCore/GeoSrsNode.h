/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: GeoSrsNode.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��11��23��
@ ����ժҪ	: �ռ�ο�ϵͳ�ڵ����ݽṹ
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

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
	char* m_pszValue;					//�ڵ��ֵ
	GeoSrsNode* m_pParentNode;			//���ڵ�
	GeoSrsNode** m_pChildNodes;			//�ӽڵ��б�
	int m_nChildNum;					//�ӽڵ����
};

#endif // end of __GEOSRSNODE_H_A103473F_DC3C_48A6_A284_D69D7B23B561__
