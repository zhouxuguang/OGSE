#ifndef BASELIB_HASHTABLE_INCLUDE_H_0X76GEFE
#define BASELIB_HASHTABLE_INCLUDE_H_0X76GEFE

#include "HashFunction.h"

NS_BASELIB_BEGIN

//��ϣ���ʵ��

template<typename KEY,typename VALUE>
class HashTable
{
public:
	typedef struct HashMapNode
	{
		eUInt32 nHashCode;		//��ϣֵ
		HashMapNode *pNext;		//��һ���ڵ�
		KEY key;					//��
		VALUE value;				//ֵ
	} HashMapNode;

public:
	explicit HashTable(int nSize = 32);

	~HashTable(void);

	//��ʼ����ϣ��
	void Init(int nSize);

	//����Ԫ��
	bool Insert(const KEY& key,const VALUE& value);

	bool Remove(const KEY& key);

	//���hash�����Ƿ���ڼ�Ϊkey��Ԫ��
	bool Contains(const KEY& key) const;

	bool GetValue(const KEY& key,VALUE& ret) const;

	eUInt32 GetUsedCount() const;

private:
	static void ReleaseNodeList(HashMapNode* pNode);

	//����ڴ�
	void Release();

	//�ؽ�Hash��
	void RecreateHash();

private:
	size_t m_nTableSize;		//��Ĵ�С
	//size_t m_nUsedCount;		//�Ѿ����˶���Ԫ��
	size_t m_nValueCount;		//�洢�˶��ټ�ֵ��
	HashMapNode** m_poChains;	//����ָ������
};

template<typename KEY,typename VALUE>
bool HashTable<KEY, VALUE>::Remove(const KEY& key)
{
	eUInt32 nHashCode = GetHashCode(key);
	eUInt32 nOffset = nHashCode % (m_nTableSize);
	
	HashMapNode* pCurNode = m_poChains[nOffset];
	HashMapNode* pPreNode = NULL;
	while (pCurNode != NULL)
	{
		if (pCurNode->key == key)
		{
			if (NULL == pPreNode)
			{
				m_poChains[nOffset] = pCurNode->pNext;
			}
			else
			{
				pPreNode->pNext = pCurNode->pNext;
			}
			
			m_nValueCount--;
			delete pCurNode;
			return true;
		}

		pPreNode = pCurNode;
		pCurNode = pCurNode->pNext;
	}

	return true;
}

template<typename KEY,typename VALUE>
void HashTable<KEY, VALUE>::RecreateHash()
{
	//
	eUInt32 nNewTableSize = m_nTableSize * 2;
	HashMapNode **poNewNodes = new HashMapNode *[nNewTableSize];
	memset(poNewNodes,0,sizeof(HashMapNode *)*nNewTableSize);

	eUInt32 nOffset = 0;
	for (eUInt32 i=0; i < m_nTableSize; i ++)
	{
		HashMapNode* pNode = m_poChains[i];
		if (pNode != NULL)
		{
			HashMapNode* pNextNode = m_poChains[i];
			while (pNextNode != NULL)
			{
				pNode = pNextNode;
				pNextNode = pNextNode->pNext;
				nOffset = pNode->nHashCode % (nNewTableSize);
				pNode->pNext = poNewNodes[nOffset];
				poNewNodes[nOffset] = pNode;
			}
			
		}
	}

	delete []m_poChains;
	m_poChains = poNewNodes;
	m_nTableSize = nNewTableSize;
}

template<typename KEY,typename VALUE>
eUInt32 HashTable<KEY, VALUE>::GetUsedCount() const
{
	int nResult = 0;
	for(eUInt32 i = 0; i < m_nTableSize; i ++)
	{
		if (m_poChains[i] != NULL)
		{
			nResult++;
		}
	}

	return nResult;
}

template<typename KEY,typename VALUE>
void HashTable<KEY, VALUE>::ReleaseNodeList(HashMapNode* pNode)
{
	HashMapNode* pCur = pNode;
	while (pCur != NULL)
	{
		pNode = pNode->pNext;
		delete pCur;
		pCur = pNode;
	}
}

template<typename KEY,typename VALUE>
bool HashTable<KEY, VALUE>::GetValue(const KEY& key,VALUE& ret) const
{
	eUInt32 nHashCode = GetHashCode(key);
	eUInt32 nOffset = nHashCode % (m_nTableSize);

	for (HashMapNode* pNode = m_poChains[nOffset]; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->key == key)
		{
			ret = pNode->value;
			return true;
		}
	}

	return false;
}

template<typename KEY,typename VALUE>
HashTable<KEY, VALUE>::~HashTable(void)
{
	Release();
}

template<typename KEY,typename VALUE>
void HashTable<KEY, VALUE>::Release()
{
	if (m_poChains != NULL)
	{
		for (int i = 0; i < m_nTableSize; i ++)
		{
			HashTable<KEY, VALUE>::ReleaseNodeList(m_poChains[i]);
		}

		delete []m_poChains;
		m_poChains = NULL;
	}

	m_nTableSize = 0;
	m_nValueCount = 0;
}

template<typename KEY,typename VALUE>
bool HashTable<KEY, VALUE>::Contains(const KEY& key) const
{
	eUInt32 nHashCode = GetHashCode(key);
	eUInt32 nOffset = nHashCode % (m_nTableSize);

	//����ڵ�ǰ��Ԫ�����ҵ�
	for (HashMapNode* pNode = m_poChains[nOffset]; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->key == key)
		{
			return true;
		}
	}

	return false;
}

template<typename KEY,typename VALUE>
bool HashTable<KEY, VALUE>::Insert(const KEY& key,const VALUE& value)
{
	eUInt32 nHashCode = GetHashCode(key);
	eUInt32 nOffset = nHashCode % (m_nTableSize);

	//����ڵ�ǰ��Ԫ�����ҵ�
	for (HashMapNode* pNode = m_poChains[nOffset]; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->key == key)
		{
			pNode->value = value;
			return true;
		}
	}

	//���û���ҵ������´���Ҫ��
	HashMapNode *pNewNode = new HashMapNode();
	pNewNode->key = key;
	pNewNode->value = value;
	pNewNode->nHashCode = nHashCode;

	//����ʹ��ͷ�巨
	pNewNode->pNext = m_poChains[nOffset];
	m_poChains[nOffset] = pNewNode;
	m_nValueCount++;


	//�����Ҫ���ٴι�ϣ
	if (m_nValueCount > m_nTableSize*2)
	{
		RecreateHash();
	}
	return true;
}

template<typename KEY,typename VALUE>
HashTable<KEY, VALUE>::HashTable(int nSize)
{
	Init(nSize);
}

template<typename KEY,typename VALUE>
void HashTable<KEY, VALUE>::Init(int nSize)
{
	m_nValueCount = 0;
	m_nTableSize = nSize;
	m_poChains = new HashMapNode*[m_nTableSize];
	memset(m_poChains,0,sizeof(HashMapNode *)*m_nTableSize);
}

NS_BASELIB_END

#endif
