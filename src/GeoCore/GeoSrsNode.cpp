#include <stdlib.h>

#include "GeoSrsNode.h"

GeoSrsNode::GeoSrsNode(const char * pszValue)
{
	m_pszValue = strdup(pszValue);
	m_pParentNode = NULL;
	m_pChildNodes = NULL;
	m_nChildNum = 0;
}

GeoSrsNode::~GeoSrsNode(void)
{
	free(m_pszValue);
	m_pszValue = NULL;

	//�ͷź��ӽڵ�ռ�
	ClearChildren();
}

bool GeoSrsNode::IsLeafNode() const
{
	return 0 == m_nChildNum;
}

bool GeoSrsNode::NeedQuoting() const
{
	// ��Ҷ�ӽڵ㲻��Ҫ˫����
	if( GetChildCount() != 0 )
	{
		return false;
	}

	//��Ȩ������Ҫ����
	if( m_pParentNode != NULL && EQUAL(m_pParentNode->GetValue(),"AUTHORITY") )
	{
		return true;
	}

	//�������ǩ����Ҫ����
	if( m_pParentNode != NULL && EQUAL(m_pParentNode->GetValue(),"AXIS")
		&& this != m_pParentNode->GetChild(0) )
	{
		return false;
	}

	if( (m_pszValue[0] == 'e' || m_pszValue[0] == 'E') )
	{
		return false;
	}

	for( int i = 0; m_pszValue[i] != '\0'; i++ )
	{
		if( (m_pszValue[i] < '0' || m_pszValue[i] > '9')
			&& m_pszValue[i] != '.'
			&& m_pszValue[i] != '-' && m_pszValue[i] != '+'
			&& m_pszValue[i] != 'e' && m_pszValue[i] != 'E' )
		{
			return true;
		}
	}

	return false;
}

int GeoSrsNode::GetChildCount() const
{
	return m_nChildNum;
}

GeoSrsNode* GeoSrsNode::GetChild( int nIndex)
{
	return GetChild(nIndex);
}

const GeoSrsNode *GeoSrsNode::GetChild( int nIndex) const
{
	if (nIndex < 0 || nIndex > m_nChildNum-1)
	{
		return NULL;
	}

	return m_pChildNodes[nIndex];
}

GeoSrsNode* GeoSrsNode::GetNode( const char * pszName)
{
	if (NULL == this)
	{
		return NULL;
	}

	if (m_nChildNum > 0 && EQUAL(pszName,m_pszValue) == 0)
	{
		return this;
	}

	int i = 0;

	//�Ƚ�ÿ���ӽڵ�
	for (i = 0; i < m_nChildNum; i ++)
	{
		if (EQUAL(pszName,m_pChildNodes[i]->m_pszValue) == 1)
		{
			return m_pChildNodes[i];
		}
	}

	//�Ƚ�ÿ���ӽڵ�ĺ��ӽڵ�
	GeoSrsNode* pNode = NULL;
	for (i = 0; i < m_nChildNum; i ++)
	{
		pNode = m_pChildNodes[i]->GetNode(pszName);
		if (pNode != NULL)
		{
			return pNode;
		}
	}

	return NULL;
}

const GeoSrsNode *GeoSrsNode::GetNode( const char * pszNodeName) const
{
	return ((GeoSrsNode*)this)->GetNode(pszNodeName);
}

void GeoSrsNode::InsertChild( GeoSrsNode *pNode, int nIndex)
{
	if (nIndex < 0 || nIndex > m_nChildNum-1)
	{
		return;
	}
	m_pChildNodes = (GeoSrsNode**)realloc(m_pChildNodes,sizeof(void*)*(m_nChildNum+1));
	if (m_pChildNodes != NULL)
	{
		memmove(m_pChildNodes+nIndex+1,m_pChildNodes+nIndex,sizeof(void*)*(m_nChildNum-nIndex));
		m_pChildNodes[nIndex] = pNode;
		m_nChildNum++;
		pNode->m_pParentNode = this;
	}
}

void GeoSrsNode::AddChild( GeoSrsNode * pNode)
{
	m_pChildNodes = (GeoSrsNode**)realloc(m_pChildNodes,sizeof(void*)*(m_nChildNum+1));
	if (m_pChildNodes != NULL)
	{
		m_pChildNodes[m_nChildNum++] = pNode;
	}
}

int GeoSrsNode::FindChild( const char * pszName) const
{
	for (int i = 0; i < m_nChildNum; i ++)
	{
		if (EQUAL(pszName,m_pChildNodes[i]->m_pszValue) == 1)
		{
			return i;
		}
	}

	return -1;
}

void GeoSrsNode::DestroyChild( int nIndex)
{
	if (nIndex < 0 || nIndex > m_nChildNum-1)
	{
		return;
	}

	delete m_pChildNodes[nIndex];
	if (nIndex < m_nChildNum-1)
	{
		memmove(m_pChildNodes+nIndex,m_pChildNodes+nIndex+1,sizeof(void*)*(m_nChildNum-nIndex-1));
	}

	m_nChildNum--;
}

void GeoSrsNode::ClearChildren()
{
	for (int i = 0; i < m_nChildNum; i ++)
	{
		delete m_pChildNodes[i];
		m_pChildNodes[i] = NULL;
	}

	delete m_pChildNodes;
	m_pChildNodes = NULL;
	m_nChildNum = 0;
}

const char* GeoSrsNode::GetValue() const
{
	return m_pszValue;
}

void GeoSrsNode::SetValue( const char * pszName)
{
	if (NULL != m_pszValue)
	{
		free(m_pszValue);
		pszName = strdup(pszName);
	}
}

GeoSrsNode* GeoSrsNode::Clone() const
{
	GeoSrsNode* pNewNode = new GeoSrsNode(m_pszValue);
	for (int i = 0; i < m_nChildNum; i ++)
	{
		pNewNode->AddChild(m_pChildNodes[i]->Clone());
	}

	return pNewNode;
}

bool GeoSrsNode::ImportFromWkt( char ** pszWkt)
{
	return true;
}

bool GeoSrsNode::ExportToWkt(char ** ppszResult) const
{
	char **papszChildrenWkt = NULL;
	int nLength = strlen(m_pszValue)+4;
	int i = 0;

	/* -------------------------------------------------------------------- */
	/*      �����������ӽڵ���ַ�������                   .                */
	/* -------------------------------------------------------------------- */
	papszChildrenWkt = (char **) calloc(sizeof(char*),(m_nChildNum+1));

	for( i = 0; i < m_nChildNum; i++ )
	{
		m_pChildNodes[i]->ExportToWkt( papszChildrenWkt + i );
		nLength += strlen(papszChildrenWkt[i]) + 1;
	}

	/* -------------------------------------------------------------------- */
	/*      ������ַ��������ڴ�.                                           */
	/* -------------------------------------------------------------------- */
	*ppszResult = (char *) malloc(nLength);
	*ppszResult[0] = '\0';

	/* �������Ҫ˫���ţ������˫����                          */
	if( NeedQuoting() )
	{
		strcat( *ppszResult, "\"" );
		strcat( *ppszResult, m_pszValue ); /* should we do quoting? */
		strcat( *ppszResult, "\"" );
	}
	else
	{
		strcat( *ppszResult, m_pszValue );
	}

	/* -------------------------------------------------------------------- */
	/*      ���������Լ����ŷָ��������ӽڵ�                                */
	/* -------------------------------------------------------------------- */
	if( m_nChildNum > 0 )
	{
		strcat( *ppszResult, "[" );
	}

	for( i = 0; i < m_nChildNum; i++ )
	{
		strcat( *ppszResult, papszChildrenWkt[i] );
		if( i == m_nChildNum-1 )
			strcat( *ppszResult, "]" );
		else
			strcat( *ppszResult, "," );
	}

	free( papszChildrenWkt );

	return true;

}

bool GeoSrsNode::ExportToPrettyWkt(char ** ppszResult,int nDepth) const
{
	char **papszChildrenWkt = NULL;
	int nLength = strlen(m_pszValue)+4;
	int i = 0;

	/* -------------------------------------------------------------------- */
	/*      �����������ӽڵ���ַ�������                   .                */
	/* -------------------------------------------------------------------- */
	papszChildrenWkt = (char **) calloc(sizeof(char*),(m_nChildNum+1));

	for( i = 0; i < m_nChildNum; i++ )
	{
		m_pChildNodes[i]->ExportToPrettyWkt( papszChildrenWkt + i,
			nDepth + 1);
		nLength += strlen(papszChildrenWkt[i]) + 2 + nDepth*4;
	}

	/* -------------------------------------------------------------------- */
	/*      ������ַ��������ڴ�.                                           */
	/* -------------------------------------------------------------------- */
	*ppszResult = (char *) malloc(nLength);
	*ppszResult[0] = '\0';

	/* �������Ҫ˫���ţ������˫����                          */
	if( NeedQuoting() )
	{
		strcat( *ppszResult, "\"" );
		strcat( *ppszResult, m_pszValue ); 
		strcat( *ppszResult, "\"" );
	}
	else
		strcat( *ppszResult, m_pszValue );

	/* -------------------------------------------------------------------- */
	/*      ���������Լ����ŷָ��������ӽڵ�                                */
	/* -------------------------------------------------------------------- */
	if( m_nChildNum > 0 )
	{
		strcat( *ppszResult, "[" );
	}

	for( i = 0; i < m_nChildNum; i++ )
	{
		if( m_pChildNodes[i]->GetChildCount() > 0 )
		{
			int  j;
			strcat( *ppszResult, "\n" );
			for( j = 0; j < 4*nDepth; j++ )
			{
				strcat( *ppszResult, " " );
			}
		}
		strcat( *ppszResult, papszChildrenWkt[i] );
		if( i < m_nChildNum-1 )
		{
			strcat( *ppszResult, "," );
		}
	}

	if( m_nChildNum > 0 )
	{
		if( (*ppszResult)[strlen(*ppszResult)-1] == ',' )
			(*ppszResult)[strlen(*ppszResult)-1] = '\0';

		strcat( *ppszResult, "]" );
	}

	free( papszChildrenWkt );

	return true;
}
