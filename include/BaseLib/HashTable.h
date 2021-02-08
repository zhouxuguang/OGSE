#ifndef BASELIB_HASHTABLE_INCLUDE_H_0X76GEFE
#define BASELIB_HASHTABLE_INCLUDE_H_0X76GEFE

#include "HashFunction.h"

NS_BASELIB_BEGIN

//哈希表的实现

template<typename KEY,typename VALUE>
class HashTable
{
public:
	typedef struct HashMapNode
	{
		eUInt32 nHashCode;		//哈希值
		HashMapNode *pNext;		//下一个节点
		KEY key;					//键
		VALUE value;				//值
	} HashMapNode;

public:
	explicit HashTable(int nSize = 32);

	~HashTable(void);

	//初始化哈希表
	void Init(int nSize);

	//插入元素
	bool Insert(const KEY& key,const VALUE& value);

	bool Remove(const KEY& key);

	//检查hash表中是否存在键为key的元素
	bool Contains(const KEY& key) const;

	bool GetValue(const KEY& key,VALUE& ret) const;

	eUInt32 GetUsedCount() const;

private:
	static void ReleaseNodeList(HashMapNode* pNode);

	//清空内存
	void Release();

	//重建Hash表
	void RecreateHash();

private:
	size_t m_nTableSize;		//表的大小
	//size_t m_nUsedCount;		//已经用了多少元素
	size_t m_nValueCount;		//存储了多少键值对
	HashMapNode** m_poChains;	//链表指针数组
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

	//如果在当前的元素中找到
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

	//如果在当前的元素中找到
	for (HashMapNode* pNode = m_poChains[nOffset]; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->key == key)
		{
			pNode->value = value;
			return true;
		}
	}

	//如果没有找到，则新创建要素
	HashMapNode *pNewNode = new HashMapNode();
	pNewNode->key = key;
	pNewNode->value = value;
	pNewNode->nHashCode = nHashCode;

	//这里使用头插法
	pNewNode->pNext = m_poChains[nOffset];
	m_poChains[nOffset] = pNewNode;
	m_nValueCount++;


	//如果必要，再次哈希
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
