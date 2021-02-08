/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: QuadTree.cpp
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��10��8��
@ ����ժҪ	:
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/

/**************************** ��������ѡ���ͷ�ļ� ****************************/

/*********************************** ��ʵ�� ***********************************/

#include "QuadTree.h"
#include <assert.h>
#include <omp.h>
#include <deque>
#include <algorithm>

using namespace std;


QuadNode *InitQuadNode()
{
	QuadNode *node = new QuadNode;
	node->Box.maxX = 0;
	node->Box.maxY = 0;
	node->Box.minX = 0;
	node->Box.minY = 0;

	for (int i = 0; i < 4; i ++)
	{
		node->children[i] = NULL;
	}
	node->nChildCount = 0;
	node->nShpCount = 0;
	node->pShapeObj = NULL;

	return node;
}

void CreateQuadTree(int depth,OGRLayer *poLayer,QuadTree* pQuadTree)
{
	pQuadTree->depth = depth;

	OGREnvelope env;	//����ͼ���MBR
	poLayer->GetExtent(&env);
	
	MapRect rect;
	rect.minX = env.MinX;
	rect.minY = env.MinY;
	rect.maxX = env.MaxX;
	rect.maxY = env.MaxY;
	
	//����������֧
	CreateQuadBranch(depth,rect,&(pQuadTree->root));

	int nCount = poLayer->GetFeatureCount();
	OGRFeature **pFeatureClass = new OGRFeature*[nCount];
	for (int i = 0; i < poLayer->GetFeatureCount(); i ++)
	{
		pFeatureClass[i] = poLayer->GetFeature(i); 
	}

	//�������Ҫ��
	OGREnvelope envObj;	//�ռ�����MBR
	//#pragma omp parallel for
	for (int i = 0; i < nCount; i ++)
	{
		pFeatureClass[i]->GetGeometryRef()->getEnvelope(&envObj);
		rect.minX = envObj.MinX;
		rect.minY = envObj.MinY;
		rect.maxX = envObj.MaxX;
		rect.maxY = envObj.MaxY;
		InsertQuad(i,rect,pQuadTree->root);
	}

	//DelFalseNode(pQuadTree->root);
}

void CreateQuadBranch(int depth,MapRect &rect,QuadNode** node)
{
	if (depth != 0)
	{
		*node = InitQuadNode();	//��������
		QuadNode *pNode = *node;
		pNode->Box = rect;
		pNode->nChildCount = 4;

		MapRect boxs[4];
		pNode->Box.Split(boxs,boxs+1,boxs+2,boxs+3);
		for (int i = 0; i < 4; i ++)
		{
			//�����ĸ��ڵ㲢������Ӧ��MBR
			pNode->children[i] = InitQuadNode();
			pNode->children[i]->Box = boxs[i];

			CreateQuadBranch(depth-1,boxs[i],&(pNode->children[i]));
		}
	}
}

void BuildQuadTree(OGRLayer *poLayer,QuadTree* pQuadTree)
{
	assert(poLayer);
	OGREnvelope env;	//����ͼ���MBR
	poLayer->GetExtent(&env);
	pQuadTree->root = InitQuadNode();

	QuadNode* rootNode = pQuadTree->root;

	rootNode->Box.minX = env.MinX;
	rootNode->Box.minY = env.MinY;
	rootNode->Box.maxX = env.MaxX;
	rootNode->Box.maxY = env.MaxY;

	//�����������(	���ݵȱ����е���͹�ʽ)
	//pQuadTree->depth = log(poLayer->GetFeatureCount()*3/8.0+1)/log(4.0);
	int nCount = poLayer->GetFeatureCount();

	MapRect rect;
	OGREnvelope envObj;	//�ռ�����MBR
	for (int i = 0; i < nCount; i ++)
	{
		poLayer->GetFeature(i)->GetGeometryRef()->getEnvelope(&envObj);
		rect.minX = envObj.MinX;
		rect.minY = envObj.MinY;
		rect.maxX = envObj.MaxX;
		rect.maxY = envObj.MaxY;
		InsertQuad2(i,rect,rootNode);
	}

	DelFalseNode(pQuadTree->root);
}

void SearchQuadTree(QuadNode* node,MapRect &queryRect,std::vector<int>& ItemSearched)
{
	assert(node);

	//int coreNum = omp_get_num_procs();
	//vector<int> * pResArr = new vector<int>[coreNum];

	if (NULL != node)
	{
		for (int i = 0; i < node->nShpCount; i ++)
		{
			if (queryRect.Contains(node->pShapeObj[i].Box)
				|| queryRect.Intersects(node->pShapeObj[i].Box))
			{
				ItemSearched.push_back(node->pShapeObj[i].nID);
			}
		}

		//���������ĸ����ӽڵ�
		/*#pragma omp parallel sections
		{
			#pragma omp section
			if ((node->children[0] != NULL) && 
				(node->children[0]->Box.Contains(queryRect)
				|| node->children[0]->Box.Intersects(queryRect)))
			{
				int tid = omp_get_thread_num();
				SearchQuadTree(node->children[0],queryRect,pResArr[tid]);
			}

			#pragma omp section
			if ((node->children[1] != NULL) && 
				(node->children[1]->Box.Contains(queryRect)
				|| node->children[1]->Box.Intersects(queryRect)))
			{
				int tid = omp_get_thread_num();
				SearchQuadTree(node->children[1],queryRect,pResArr[tid]);
			}

			#pragma omp section
			if ((node->children[2] != NULL) && 
				(node->children[2]->Box.Contains(queryRect)
				|| node->children[2]->Box.Intersects(queryRect)))
			{
				int tid = omp_get_thread_num();
				SearchQuadTree(node->children[2],queryRect,pResArr[tid]);
			}

			#pragma omp section
			if ((node->children[3] != NULL) && 
				(node->children[3]->Box.Contains(queryRect)
				|| node->children[3]->Box.Intersects(queryRect)))
			{
				int tid = omp_get_thread_num();
				SearchQuadTree(node->children[3],queryRect,pResArr[tid]);
			}
		}*/
		for (int i = 0; i < 4; i ++)
		{
			if ((node->children[i] != NULL) && 
				(node->children[i]->Box.Contains(queryRect)
				|| node->children[i]->Box.Intersects(queryRect)))
			{
				SearchQuadTree(node->children[i],queryRect,ItemSearched);
				//node = node->children[i];	//�ǵݹ�
			}
		}
	}

	/*for (int i = 0 ; i < coreNum; i ++)
	{
		ItemSearched.insert(ItemSearched.end(),pResArr[i].begin(),pResArr[i].end());
	}*/

}

void SearchQuadTreePara(vector<QuadNode*> resNodes,MapRect &queryRect,vector<int>& ItemSearched)
{
	int coreNum = omp_get_num_procs();
	omp_set_num_threads(coreNum);
	vector<int>* searchArrs = new vector<int>[coreNum];
	for (int i = 0; i < coreNum; i ++)
	{
		searchArrs[i].clear();
	}

	#pragma omp parallel for
	for (int i = 0; i < resNodes.size(); i ++)
	{
		int tid = omp_get_thread_num();
		for (int j = 0; j < resNodes[i]->nShpCount; j ++)
		{
			if (queryRect.Contains(resNodes[i]->pShapeObj[j].Box)
				|| queryRect.Intersects(resNodes[i]->pShapeObj[j].Box))
			{
				searchArrs[tid].push_back(resNodes[i]->pShapeObj[j].nID);
			}
		}
	}

	for (int i = 0; i < coreNum; i ++)
	{
		ItemSearched.insert(ItemSearched.end(),
			searchArrs[i].begin(),searchArrs[i].end());
	}

	delete [] searchArrs;
	searchArrs = NULL;
}

void PtSearchQTree(QuadNode* node,double cx,double cy,vector<int>& ItemSearched)
{
	assert(node);
	if (node->nShpCount >0)		//�ڵ�
	{
		for (int i = 0; i < node->nShpCount; i ++)
		{
			if (node->pShapeObj[i].Box.IsPointInRect(cx,cy))
			{
				ItemSearched.push_back(node->pShapeObj[i].nID);
			}
		}
	}

	else if (node->nChildCount >0)				//�ڵ�
	{
		for (int i = 0; i < 4; i ++)
		{
			if (node->children[i]->Box.IsPointInRect(cx,cy))
			{
				PtSearchQTree(node->children[i],cx,cy,ItemSearched);
			}
		}
	}

	//�ҳ��ظ�Ԫ�ص�λ��
	std::sort(ItemSearched.begin(),ItemSearched.end());	//������,Ĭ������
	vector<int>::iterator unique_iter = 
		std::unique(ItemSearched.begin(),ItemSearched.end());
	ItemSearched.erase(unique_iter,ItemSearched.end());
}

void Insert(long key, MapRect &itemRect,QuadNode* pNode)
{
	QuadNode *node = pNode;		//�������ڵ㸱��
	SHPMBRInfo pShpInfo;
	
	//�ڵ��к���
	if (0 < node->nChildCount)
	{
		for (int i = 0; i < 4; i ++)
		{  
			//����������ཻ���򽫽ڵ���뵽�˽ڵ�
			if (node->children[i]->Box.Contains(itemRect)
				|| node->children[i]->Box.Intersects(itemRect))
			{
				//node = node->children[i];
				Insert(key,itemRect,node->children[i]);
			}
		}
	}

	//�����ǰ�ڵ����һ���ӽڵ�ʱ
	else if (1 == node->nShpCount)
	{
		MapRect boxs[4];
		node->Box.Split(boxs,boxs+1,boxs+2,boxs+3);

		//�����ĸ��ڵ㲢������Ӧ��MBR
		node->children[UR] = InitQuadNode();
		node->children[UL] = InitQuadNode();
		node->children[LL] = InitQuadNode();
		node->children[LR] = InitQuadNode();

		node->children[UR]->Box = boxs[0];
		node->children[UL]->Box = boxs[1];
		node->children[LL]->Box = boxs[2];
		node->children[LR]->Box = boxs[3];
		node->nChildCount = 4;

		for (int i = 0; i < 4; i ++)
		{  
			//����ǰ�ڵ��е�Ҫ���ƶ�����Ӧ���ӽڵ���
			for (int j = 0; j < node->nShpCount; j ++)
			{
				if (node->children[i]->Box.Contains(node->pShapeObj[j].Box)
					|| node->children[i]->Box.Intersects(node->pShapeObj[j].Box))
				{
					node->children[i]->nShpCount += 1;
					node->children[i]->pShapeObj = 
						(SHPMBRInfo*)malloc(node->children[i]->nShpCount*sizeof(SHPMBRInfo));
					
					memcpy(node->children[i]->pShapeObj,&(node->pShapeObj[j]),sizeof(SHPMBRInfo));

					free(node->pShapeObj);
					node->pShapeObj = NULL;
					node->nShpCount = 0;
				}
			}
		}

		for (int i = 0; i < 4; i ++)
		{  
			//����������ཻ���򽫽ڵ���뵽�˽ڵ�
			if (node->children[i]->Box.Contains(itemRect)
				|| node->children[i]->Box.Intersects(itemRect))
			{
				if (node->children[i]->nShpCount == 0)	 //���֮ǰû�нڵ�
				{
					node->children[i]->nShpCount += 1;
					node->pShapeObj = 
						(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*node->children[i]->nShpCount);
				}
				else if	(node->children[i]->nShpCount > 0)
				{
					node->children[i]->nShpCount += 1;
					node->children[i]->pShapeObj = 
						(SHPMBRInfo *)realloc(node->children[i]->pShapeObj,
						sizeof(SHPMBRInfo)*node->children[i]->nShpCount);
				}

				pShpInfo.Box = itemRect;
				pShpInfo.nID = key;
				memcpy(node->children[i]->pShapeObj,
					&pShpInfo,sizeof(SHPMBRInfo));
			}
		}
	}

	//��ǰ�ڵ�û�пռ����
	else if (0 == node->nShpCount)
	{
		node->nShpCount += 1;
		node->pShapeObj = 
			(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*node->nShpCount);

		pShpInfo.Box = itemRect;
		pShpInfo.nID = key;
		memcpy(node->pShapeObj,&pShpInfo,sizeof(SHPMBRInfo));
	}
}

void InsertQuad(long key,MapRect &itemRect,QuadNode* pNode)
{
	assert(pNode != NULL);

	if (!IsQuadLeaf(pNode))	   //��Ҷ�ӽڵ�
	{
		int nCorver = 0;		//��Խ���ӽڵ����
		int iIndex = -1;		//���ĸ��ӽڵ���ȫ������������
		for (int i = 0; i < 4; i ++)
		{
			if (pNode->children[i]->Box.Contains(itemRect)
				&& pNode->Box.Contains(itemRect))
			{
				nCorver += 1;
				iIndex = i;
			}
		}

		//�����ĳһ���ӽڵ�������������ӽڵ�
		if (/*pNode->Box.Contains(itemRect) || 
			pNode->Box.Intersects(itemRect)*/1 <= nCorver)
		{ 
			InsertQuad(key,itemRect,pNode->children[iIndex]);
		}

		//�����Խ�˶���ӽڵ㣬ֱ�ӷ�������ڵ���
		else if (nCorver == 0)
		{
			if (pNode->nShpCount == 0)	 //���֮ǰû�нڵ�
			{
				pNode->nShpCount += 1;
				pNode->pShapeObj = 
					(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*pNode->nShpCount);
			}
			else
			{
				pNode->nShpCount += 1;
				pNode->pShapeObj = 
					(SHPMBRInfo *)realloc(pNode->pShapeObj,sizeof(SHPMBRInfo)*pNode->nShpCount);
			}

			SHPMBRInfo pShpInfo;
			pShpInfo.Box = itemRect;
			pShpInfo.nID = key;
			memcpy(pNode->pShapeObj+pNode->nShpCount-1,&pShpInfo,sizeof(SHPMBRInfo));
		}
	}

	//�����Ҷ�ӽڵ㣬ֱ�ӷŽ�ȥ
	else if (IsQuadLeaf(pNode))
	{
		if (pNode->nShpCount == 0)	 //���֮ǰû�нڵ�
		{
			pNode->nShpCount += 1;
			pNode->pShapeObj = 
				(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*pNode->nShpCount);
		}
		else
		{
			pNode->nShpCount += 1;
			pNode->pShapeObj = 
				(SHPMBRInfo *)realloc(pNode->pShapeObj,sizeof(SHPMBRInfo)*pNode->nShpCount);
		}

		SHPMBRInfo pShpInfo;
		pShpInfo.Box = itemRect;
		pShpInfo.nID = key;
		memcpy(pNode->pShapeObj+pNode->nShpCount-1,&pShpInfo,sizeof(SHPMBRInfo));
	}
}

void InsertQuad2(long key,MapRect &itemRect,QuadNode* pNode)
{
	QuadNode *node = pNode;		//�������ڵ㸱��
	SHPMBRInfo pShpInfo;

	//�ڵ��к���
	if (0 < node->nChildCount)
	{
		for (int i = 0; i < 4; i ++)
		{  
			//����������ཻ���򽫽ڵ���뵽�˽ڵ�
			if (node->children[i]->Box.Contains(itemRect)
				|| node->children[i]->Box.Intersects(itemRect))
			{
				//node = node->children[i];
				Insert(key,itemRect,node->children[i]);
			}
		}
	}

	//�����ǰ�ڵ����һ���ӽڵ�ʱ
	else if (0 == node->nChildCount)
	{
		MapRect boxs[4];
		node->Box.Split(boxs,boxs+1,boxs+2,boxs+3);

		int cnt = -1;
		for (int i = 0; i < 4; i ++)
		{  
			//����������ཻ���򽫽ڵ���뵽�˽ڵ�
			if (boxs[i].Contains(itemRect))
			{
				cnt = i;
			}
		}

		//�����һ�����ΰ����˶����򴴽��ĸ����ӽڵ�
		if (cnt > -1)
		{
			for (int i = 0; i < 4; i ++)
			{
				//�����ĸ��ڵ㲢������Ӧ��MBR
				node->children[i] = InitQuadNode();
				node->children[i]->Box = boxs[i];
			}
			node->nChildCount = 4;
			InsertQuad2(key,itemRect,node->children[cnt]);	//�ݹ�
		}

		//���������������ֱ�ӽ��������˽ڵ�
		if (cnt == -1)
		{
			if (node->nShpCount == 0)	 //���֮ǰû�нڵ�
			{
				node->nShpCount += 1;
				node->pShapeObj = 
					(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*node->nShpCount);
			}
			else if	(node->nShpCount > 0)
			{
				node->nShpCount += 1;
				node->pShapeObj = 
					(SHPMBRInfo *)realloc(node->pShapeObj,
					sizeof(SHPMBRInfo)*node->nShpCount);
			}

			pShpInfo.Box = itemRect;
			pShpInfo.nID = key;
			memcpy(node->pShapeObj,
				&pShpInfo,sizeof(SHPMBRInfo));
		}
	}

	//��ǰ�ڵ�û�пռ����
	/*else if (0 == node->nShpCount)
	{
		node->nShpCount += 1;
		node->pShapeObj = 
			(SHPMBRInfo*)malloc(sizeof(SHPMBRInfo)*node->nShpCount);

		pShpInfo.Box = itemRect;
		pShpInfo.nID = key;
		memcpy(node->pShapeObj,&pShpInfo,sizeof(SHPMBRInfo));
	}*/
}

bool IsQuadLeaf(QuadNode* node)
{
	if (NULL == node)
	{
		return 1;
	}
	for (int i = 0; i < 4; i ++)
	{
		if (node->children[i] != NULL)
		{
			return 0;
		}
	}

	return 1;
}

bool DelFalseNode(QuadNode* node)
{
	//���û���ӽڵ���û��Ҫ��
	if (node->nChildCount ==0 && node->nShpCount == 0)
	{
		ReleaseQuadTree(&node);
	}

	//������ӽڵ�
	else if (node->nChildCount > 0)
	{
		for (int i = 0; i < 4; i ++)
		{
			DelFalseNode(node->children[i]);
		}
	}

	return 1;
}

void TraversalQuadTree(QuadNode* quadTree,vector<int>& resVec)
{
	QuadNode *node = quadTree;
	int i = 0; 
	if (NULL != node)
	{
		//�����ڵ��еĿռ����洢������
		for (i = 0; i < node->nShpCount; i ++)
		{
			resVec.push_back((node->pShapeObj+i)->nID);
		}

		//�������ӽڵ�
		for (i = 0; i < node->nChildCount; i ++)
		{
			if (node->children[i] != NULL)
			{
				TraversalQuadTree(node->children[i],resVec);
			}
		}
	}

}

void TraversalQuadTree(QuadNode* quadTree,std::vector<QuadNode*>& arrNode)
{
	std::deque<QuadNode*> nodeQueue;
	if (quadTree != NULL)
	{
		nodeQueue.push_back(quadTree);
		while (!nodeQueue.empty())
		{
			QuadNode* queueHead = nodeQueue.at(0);	//ȡ����ͷ���
			arrNode.push_back(queueHead);
			nodeQueue.pop_front();
			for (int i = 0; i < 4; i ++)
			{
				if (queueHead->children[i] != NULL)
				{
					nodeQueue.push_back(queueHead->children[i]);
				}
			}
		}
	}
}

void ReleaseQuadTree(QuadNode** quadTree)
{
	int i = 0;
	QuadNode* node = *quadTree;
	if (NULL == node)
	{
		return;
	}

	else
	{
		for (i = 0; i < 4; i ++)
		{ 
			ReleaseQuadTree(&node->children[i]);
		}
		free(node);
		node = NULL;
	}

	node = NULL;
}

long CalByteQuadTree(QuadNode* quadTree,long& nSize)
{
	if (quadTree != NULL)
	{
		nSize += sizeof(QuadNode)+quadTree->nChildCount*sizeof(SHPMBRInfo);
		for (int i = 0; i < 4; i ++)
		{
			if (quadTree->children[i] != NULL)
			{
				nSize += CalByteQuadTree(quadTree->children[i],nSize);
			}
		}
	}

	return 1;
}
