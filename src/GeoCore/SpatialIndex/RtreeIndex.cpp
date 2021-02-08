
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "RtreeIndex.h"

#include "GeoEnvelope.h"

CRtreeIndex::CRtreeIndex(void)
{
	m_pRootNode = NULL;
}

CRtreeIndex::~CRtreeIndex(void)
{
	if (m_pRootNode != NULL)
	{
		delete m_pRootNode;
	}
}

//void CRtree::BuildRtTree(OGRLayer *poLayer)
//{
//	//ѭ�����빹��R��
//	GeoEnvelope rect;
//	OGREnvelope env;
//	long FID = -1;	//Ҫ��ID��
//	for (int i = 0; i < poLayer->GetFeatureCount(); i ++)
//	{
//		FID = poLayer->GetFeature(i)->GetFID();
//		poLayer->GetFeature(i)->GetGeometryRef()->getEnvelope(&env);
//		rect.maxX = env.MaxX;
//		rect.minX = env.MinX;
//		rect.maxY = env.MaxY;
//		rect.minY = env.MinY;
//		Insert(FID,rect);		//���Ҫ�ز���
//	}
//}

void CRtreeIndex::Insert(long key,const GeoEnvelope &itemRect)
{
	RTreeNode *newNode = NULL;		 //�洢���ʵĽڵ�
	RTreeNode *splitNode = NULL;	//���ѳ����Ľڵ�
	newNode = ChooseLeaf(m_pRootNode,(GeoEnvelope &)itemRect);	//Ѱ��һ�����ʵ�Ҷ�ڵ�
	if (newNode && newNode->nCount < M)	//����ڵ��л���ʣ��Ŀռ䣬�����
	{
		newNode->pBranch[newNode->nCount].pChild = (RTreeNode*)&key;
		newNode->pBranch[newNode->nCount].MBR = itemRect;
		newNode->nCount += 1;	  //������1
		AdjustRTree(newNode,splitNode);
	}

	else	//����ڵ�������M������������ڵ��������Ҫ�ڵ����
	{
		std::vector<IndexEntry> branchVec;
		for(int i = 0; i < M; i ++)
		{
			//newNode->branch[i].IsDelete = false;
			branchVec.push_back(newNode->pBranch[i]);
		}
		IndexEntry addBranch;
		addBranch.pChild = (RTreeNode*)&key;
		addBranch.count = 0;
		//addBranch.IsDelete = false;
		addBranch.MBR = itemRect;
		branchVec.push_back(addBranch);
		SplitNode(newNode,branchVec,&splitNode);//���ѽڵ�

		RTreeNode *aNode = RTreeNewNode();
		aNode->pBranch[0].pChild = newNode;
		aNode->pBranch[1].pChild = splitNode;
		aNode->nLevel = newNode->nLevel + 1;
		aNode->nCount = 2;

		//�����ڵ㸳ֵ
		newNode->pParent = aNode;
		splitNode->pParent = aNode;

		AdjustRTree(newNode,splitNode);
	}

	RTreeNode *pRootNode = m_pRootNode;

	//�����µĸ��ڵ�
	if (pRootNode->nCount >= M)
	{
		std::vector<IndexEntry> branchVec;

		for (int i = 0; i < pRootNode->nCount; i ++)
		{
			branchVec.push_back(pRootNode->pBranch[i]);
		}
		RTreeNode *secondChild = NULL;
		SplitNode(pRootNode,branchVec,&secondChild);
		RTreeNode *firstNode = pRootNode;

		pRootNode = RTreeNewNode();
		pRootNode->pBranch[0].pChild = firstNode;
		pRootNode->pBranch[1].pChild = secondChild;
		pRootNode->nCount = 2;
		pRootNode->nLevel = firstNode->nLevel+1;

		//�����ڵ㸳ֵ
		firstNode->pParent = pRootNode;
		secondChild->pParent = pRootNode;
	}  
}

void CRtreeIndex::Query(double cx, double cy, vector<long>& ItemSearched )
{
	static RTreeNode *pRootNode = NULL;
	if (NULL == m_pRootNode)
	{
		return;
	}

	if (NULL == pRootNode)
	{
		pRootNode = m_pRootNode;
	}

	if (pRootNode->nLevel == 0)	 //�����Ҷ�ӽڵ�
	{
		for (int i = 0; i < pRootNode->nCount; i ++)
		{
			if (pRootNode->pBranch[i].MBR.Contains(cx,cy))
			{
				ItemSearched.push_back((long)(pRootNode->pBranch[i].pChild));	//��Ԫ�ص�ID�ż��뵽������
			}
		}
	}

	else	//������м�ڵ�
	{
		for (int i = 0; i < pRootNode->nCount; i++)
		{
			if (pRootNode->pBranch[i].MBR.Contains(cx,cy))	//����ཻ����������ݹ����
			{
				pRootNode = pRootNode->pBranch[i].pChild;
				Query(cx,cy,ItemSearched);
			}
		}
	}

	pRootNode = m_pRootNode;
}


void CRtreeIndex::Query(const GeoEnvelope& searchBox, vector<long>& ItemSearched )
{
	static RTreeNode *pRootNode = NULL;
	if (NULL == m_pRootNode)
	{
		return;
	}

	if (NULL == pRootNode)
	{
		pRootNode = m_pRootNode;
	}

	//����ڵ���Ҷ�ӽڵ�
	if ( pRootNode->nLevel == 0)
	{
		for (int i = 0; i < pRootNode->nCount; i ++)
		{
			if (pRootNode->pBranch[i].MBR.Intersects(searchBox))	//����ཻ�������ѡ��
			{
				ItemSearched.push_back((int)pRootNode->pBranch[i].pChild);	//��Ԫ�ص�ID�ż��뵽������
			}
		}
	}

	else if (pRootNode->nLevel > 0)	//������м�ڵ�
	{
		for (int i = 0; i < pRootNode->nCount; i++)
		{
			if (pRootNode->pBranch[i].MBR.Intersects(searchBox))	//����ཻ����������ݹ����
			{
				pRootNode = pRootNode->pBranch[i].pChild;
				Query(searchBox,ItemSearched);
			}
		}
	}

	pRootNode = m_pRootNode;
}

bool CRtreeIndex::Delete(long itemID,const GeoEnvelope& itemBox)
{
	int locIndex = -1;	//Ҫɾ��Ҫ���ڽڵ��е�λ��
	RTreeNode *Node = FindLeaf(m_pRootNode,(GeoEnvelope &)itemBox,&locIndex);	//Ѱ�Һ��ʵ�Ҷ�ӽڵ�
	if (Node == NULL)
	{
		return false;		//���û�ҵ���ֱ����ֹɾ������
	}

	//���ҵ��Ľڵ���ɾ��Ҫ��
	for (int i = locIndex;i<Node->nCount;i++)
	{
		Node->pBranch[i] = Node->pBranch[i+1];
	}
	Node->nCount--;	//Ԫ�ظ�����1

	//�������Ľṹ
	CondenseTree(Node);

	if (m_pRootNode->nCount == 1)	   //������ڵ�ֻ��һ���ӽڵ㣬�Ͱ�����ӽڵ���Ϊ���ڵ�
	{
		RTreeNode *p = m_pRootNode->pBranch[0].pChild;
		m_pRootNode->pBranch[0].pChild = NULL;
		m_pRootNode = p;
		m_pRootNode->nLevel += 1;
	}
	return true;
}

RTreeNode* CRtreeIndex::FindLeaf(RTreeNode *a_node,GeoEnvelope& delRect,int* nIndex)
{
	//   �����Ҷ�ӽڵ�
	if (a_node && a_node->nLevel == 0)
	{
		for (int i = 0; i < a_node->nCount; i ++)
		{
			if (a_node->pBranch[i].MBR.Contains(delRect))
			{
				nIndex = &i;  //������¼�ڽڵ��е�λ��
				return a_node;
			}
		}
	}

	//  ������м�ڵ�
	else if (a_node && a_node->nLevel > 0)
	{
		for (int i = 0; i < a_node->nCount; i ++)
		{
			if (a_node->pBranch[i].MBR.Intersects(delRect))
			{
				FindLeaf(a_node->pBranch[i].pChild,delRect,nIndex);
			}
		}
	}

	return NULL;
}

void CRtreeIndex::CondenseTree(RTreeNode *l_node)
{
	RTreeNode *Node = l_node;		//��NodeΪ��ɾ����Ԫ��Ҷ�ӽڵ�
	vector<RTreeNode *> QnodeVector(0);		//�ڵ�ļ��ϣ����ڴ�ű�ɾ���Ľڵ�
	QnodeVector.clear();

	int pIndex = -1;	//�ڸ��ڵ��е�������

	RTreeNode *parentNode = NULL;	//���ڵ�
	while (parentNode = Node->pParent)
	{
		for (int i = 0; i < parentNode->nCount; i ++)
		{
			if (parentNode->pBranch[i].pChild == Node)	//Ѱ���ڸ��ڵ��еĵ�Ԫ
			{
				pIndex = i;
			}
		}
		if (Node->nCount < M/2)   //����ڵ�����
		{
			//�ڸ��ڵ���ɾ��Node
			parentNode->pBranch[pIndex].pChild = NULL;	//�Ͽ����ӽڵ����ϵ
			for (int i = pIndex;i<parentNode->nCount;i++)
			{
				parentNode->pBranch[i] = parentNode->pBranch[i+1];
			}
			parentNode->nCount--;

			QnodeVector.push_back(Node);	//���ڵ���뵽QnodeVector��
		}
		//�����ڵ�Node���ڸ��ڵ㵥Ԫ��MBR��ʹ�����Node�����е�Ԫ
		parentNode->pBranch[pIndex].MBR = GetMBR(Node);

		Node = parentNode;
	}

	//���²���
	for (int i = 0; i < (int)QnodeVector.size(); i ++)
	{
		for (int j = 0; j < QnodeVector.at(i)->nCount; j ++)
		{
			Insert((long)QnodeVector.at(i)->pBranch[j].pChild,
				QnodeVector.at(i)->pBranch[j].MBR);
		}
	}
}

void CRtreeIndex::Update(long itemID, const GeoEnvelope& oldItemMBR, const GeoEnvelope& newItemMBR)
{
	//
}

RTreeNode *CRtreeIndex::ChooseLeaf(RTreeNode *root,GeoEnvelope &itemRect)
{
	//�Ӹ��ڵ㿪ʼ����
	if (root != NULL)
	{
		RTreeNode *Node = root;		//��ʼ��Ϊ���ڵ�
		if (Node->nLevel == 0)		//�����Ҷ�ӽڵ㣬ֱ�ӷ���
		{
			return Node;
		}

		double *deltaArea = new double[Node->nCount];	//�������
		while (Node->nLevel != 0 && Node != NULL)		//�������Ҷ�ӽڵ�
		{
			//���������Ԫ�����������
			GeoEnvelope newRect;
			for (int i = 0; i < Node->nCount; i ++)	//�ҳ������������С�Ľڵ�
			{
				newRect.minX = min(Node->pBranch[i].MBR.minX,itemRect.minX);
				newRect.maxX = max(Node->pBranch[i].MBR.maxX,itemRect.maxX);
				newRect.minY = min(Node->pBranch[i].MBR.minY,itemRect.minY);
				newRect.maxY = max(Node->pBranch[i].MBR.maxY,itemRect.maxY);
				deltaArea[i] = newRect.GetArea() - Node->pBranch[i].MBR.GetArea();	//�������
			}

			//�ҳ������С�Ľڵ�
			double minDelArea = DBL_MAX;
			int nIndex = -1;
			for (int i = 0; i < Node->nCount; i ++)
			{
				if (deltaArea[i] < minDelArea)
				{
					minDelArea = deltaArea[i];
					nIndex = i;		//������СԪ�����ڵ��±�
				}
			}

			Node = Node->pBranch[nIndex].pChild;

			ChooseLeaf(Node,itemRect);	//��������С�ĵ�Ԫ�еݹ����
		}

		delete [] deltaArea;

		return Node;
	}

	return NULL;
	
}

void CRtreeIndex::AdjustRTree(RTreeNode *LNode,RTreeNode *RNode)
{
	//
	RTreeNode *N = LNode;
	RTreeNode *NN = RNode;	//����Ѷ��������½ڵ㣨����û�У�

	int pIndex = -1;	//�ýڵ��ڸ��ڵ��е�λ��
	RTreeNode *parentNode = NULL;	//���ڵ�
	while (parentNode = N->pParent)
	{
		for (int i = 0; i < parentNode->nCount; i ++)
		{
			if (parentNode->pBranch[i].pChild == N)	//Ѱ���ڸ��ڵ��еĵ�Ԫ
			{
				pIndex = i;
			}
		}
		parentNode->pBranch[pIndex].MBR = GetMBR(N);	//����MBR

		RTreeNode *newNode = NULL;
		if (NULL != NN)	 //��������з��ѵĽڵ�
		{
			parentNode->pBranch[parentNode->nCount].pChild = NN;
			parentNode->pBranch[parentNode->nCount].count = NN->nCount;
			parentNode->pBranch[parentNode->nCount].MBR = GetMBR(NN);
			parentNode->nCount += 1;

			if (parentNode->nCount > M)	//�ڵ�����������ڵ���
			{
				vector<IndexEntry> branchVec;
				branchVec.clear();
				for(int i = 0; i < M; i ++)
				{
					//parentNode->branch[i].IsDelete = false;
					branchVec.push_back(parentNode->pBranch[i]);
				}
				IndexEntry addBranch;
				addBranch.pChild = NN;
				addBranch.count = NN->nCount;
				//addBranch.IsDelete = false;
				addBranch.MBR = GetMBR(NN);

				branchVec.push_back(addBranch);
				SplitNode(parentNode,branchVec,&newNode);//���ѽڵ�
			}
		}

		N = parentNode;
		NN = newNode;
	}

}

void CRtreeIndex::SplitNode(RTreeNode*& a_node, vector<IndexEntry>& a_branch,RTreeNode** a_newNode)
{
	//��ѡ�����Ӿ������������ڵ���
	int l1,l2;
	PickSeeds(a_branch,&l1,&l2);	//ѡȡ��ʼ�����Ӿ���
	vector<IndexEntry> firstNode;	 //��һ�����μ���
	for (int i = 0; i < a_node->nCount; i ++)
	{
		firstNode.push_back(a_node->pBranch[i]);
	}

	vector<IndexEntry> secondNode;					 //�ڶ������μ���
	*a_newNode = RTreeNewNode();
	for (int i = 0; i < (*a_newNode)->nCount; i ++)
	{
		secondNode.push_back((*a_newNode)->pBranch[i]);
	}
	//a_node = RTreeNewNode();

	//��ѡȡ�ľ��ηֱ𸳸�����
	a_node->pBranch[0].MBR = a_branch[l1].MBR;
	a_node->pBranch[0].pChild = a_branch[l1].pChild;
	a_node->pBranch[0].count = 0;
	a_node->nCount = 1;

	(*a_newNode)->pBranch[0].MBR = a_branch[l2].MBR;
	(*a_newNode)->pBranch[0].pChild = a_branch[l2].pChild;
	(*a_newNode)->pBranch[0].count = 0;
	(*a_newNode)->nCount += 1;
	(*a_newNode)->nLevel = a_node->nLevel;


	//��ɾ�������Ϊtrue
	//a_branch.at(l1).IsDelete = true;
	//a_branch.at(l2).IsDelete = true;

	for (int i = 0; i < (int)a_branch.size(); i ++)
	{
		//����Ǿ��μ����Ѿ�������ϣ�����ֹ
		int cnt = 0;	//ͳ�Ʊ��δɾ����Ԫ��
		for (int i = 0; i < (int)a_branch.size(); i ++)
		{
			/*if (!a_branch.at(i).IsDelete)
			{
				cnt += 1;
			}*/
		}

		//(2)
		if (0 == cnt)
		{
			return;		//ֱ�ӷ���
		}

		//δ�����Ԫ��+ĳһ���е�Ԫ��=m������ֹ����
		if (cnt+a_node->nCount == M/2 || cnt+(*a_newNode)->nCount == M/2)
		{
			//�����о��η��������ڵ�
			for (int i = 0; i < (int)a_branch.size(); i ++)
			{
				//if (!a_branch.at(i).IsDelete)	//û�з����
				//{
				//	a_node->branch[a_node->nCount].MBR = a_branch[i].MBR;
				//	a_node->branch[a_node->nCount].pChild = a_branch[i].pChild;
				//	a_node->branch[a_node->nCount].count = 0;
				//	a_node->nCount += 1;

				//	a_branch.at(i).IsDelete = true;
				//}
			}
			return;
		}

		//(3)
		int l3 = -1;		//��һ��Ҫ����ľ������
		l3 = PickNext(a_node,*a_newNode,a_branch);	
		//a_branch.at(l3).IsDelete = true;

		GeoEnvelope rect1,rect2;
		rect1 = GetMBR(a_node);
		rect2 = GetMBR(*a_newNode);

		//ѡ�����������С����Ϊ����Ľڵ�
		/*double d1 = rect1.Merge(a_branch.at(l3).MBR).GetArea()-a_branch.at(l3).MBR.GetArea();
		double d2 = rect2.Merge(a_branch.at(l3).MBR).GetArea()-a_branch.at(l3).MBR.GetArea();*/
		/*if (d1 < d2)
		{
			a_node->branch[a_node->nCount].MBR = a_branch[l3].MBR;
			a_node->branch[a_node->nCount].pChild = NULL;
			a_node->branch[a_node->nCount].pChild = a_branch[l3].pChild;
			a_node->branch[a_node->nCount].count = 0;
			a_node->nCount += 1;
		}
		if (d1 > d2)
		{
			(*a_newNode)->branch[a_node->nCount].MBR = a_branch[l3].MBR;
			(*a_newNode)->branch[a_node->nCount].pChild = NULL;
			(*a_newNode)->branch[a_node->nCount].pChild = a_branch[l3].pChild;
			(*a_newNode)->branch[a_node->nCount].count = 0;
			(*a_newNode)->nCount += 1;
		}*/

		/*if (d1 == d2 && rect1.GetArea() < rect2.GetArea())
		{
			a_node->branch[a_node->count].MBR = a_branch[l3].MBR;
			a_node->branch[a_node->count].child = NULL;
			a_node->branch[a_node->count].OID = a_branch[l3].OID;
			a_node->branch[a_node->count].count = 0;
			a_node->count += 1;
		}*/
	}
}

void CRtreeIndex::PickSeeds(vector<IndexEntry> a_branch,int* pos1,int* pos2)
{
	/*int seed0,seed1;*/
	int count = a_branch.size();
	double worst = 0;
	double *area = new double[count];		//�������ε�Ԫ�����
	GeoEnvelope rect1,rect2;

	for (int i = 0; i < count; i ++)
	{
		area[i] = a_branch.at(i).MBR.GetArea();
	}

	for (int i = 0; i < count-1; i ++)
	{
		//for (int j = i+1; j < count; j ++)
		//{
		//	rect1 = a_branch.at(i).MBR;
		//	rect2 = a_branch.at(j).MBR;
		//	//�����µľ��Σ��ǰ�����i����j��Ԫ��
		//	GeoEnvelope newRect = rect1.Merge(rect2);
		//	if (i != j && worst <= newRect.GetArea()-area[i]-area[j])		//Ѱ�Ҿ�����������
		//	{
		//		worst = newRect.GetArea()-area[i]-area[j];
		//		*pos1 = i;
		//		*pos2 = j;
		//	}
		//}
	}

	delete []area;
}

int CRtreeIndex::PickNext(RTreeNode* a_node, RTreeNode *otherNode,vector<IndexEntry> branch)
{
	//
	//double deltaArea = 0;
	//int nSelected = -1;		//ѡ��ľ�������
	//int count = branch.size();
	//for (int i = 0; i < count; i ++)
	//{
	//	if (branch.at(i).IsDelete == false)
	//	{
	//		double d1,d2;	//������������ֵ
	//		GeoEnvelope oneRect = GetMBR(a_node);
	//		GeoEnvelope otherRect = GetMBR(otherNode);

	//		d1 = oneRect.Merge(branch.at(i).MBR).GetArea() - 
	//			branch.at(i).MBR.GetArea();
	//		d2 = otherRect.Merge(branch.at(i).MBR).GetArea() - 
	//			branch.at(i).MBR.GetArea();
	//		if (deltaArea < fabs(d1-d2))
	//		{
	//			deltaArea = fabs(d1-d2);
	//			nSelected = i;
	//		}
	//	}
	//}

	int nSelected = 0;
	return nSelected;
}

void CRtreeIndex::RTreeInitNode(RTreeNode *node)
{
	int i = 0;
	node->nCount = 0;
	node->nLevel = 0;
	for (i = 0; i < M; i++)
	{
		//��ʼ��ÿһ����֧
		RTreeInitBranch(&(node->pBranch[i]));
	}

}

void CRtreeIndex::RTreeInitBranch( IndexEntry *br )
{
	br->MBR.minX = 0;
	br->MBR.maxX = 0;
	br->MBR.minY = 0;
	br->MBR.maxY = 0;
	br->pChild = NULL;
}

RTreeNode * CRtreeIndex::RTreeNewNode()
{
	RTreeNode *node = (RTreeNode *)malloc(sizeof(RTreeNode));
	assert(node);
	RTreeInitNode(node);

	return node;
}

RTreeNode* CRtreeIndex::GetParentNode(RTreeNode *node)
{
	// �Ӹ��ڵ㿪ʼ����
	if (m_pRootNode)
	{
		for (int i = 0; i < m_pRootNode->nCount; i ++)
		{
			if (m_pRootNode->pBranch[i].pChild == node)
			{
				return m_pRootNode;
			}
			else
			{
				GetParentNode(m_pRootNode->pBranch[i].pChild);
			}
		}
	}

	return NULL;
}

GeoEnvelope& CRtreeIndex::GetMBR(RTreeNode *rnode)
{
	return *(new GeoEnvelope(NULL,NULL) );
}

