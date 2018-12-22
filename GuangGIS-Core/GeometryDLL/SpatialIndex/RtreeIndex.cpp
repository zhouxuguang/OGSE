
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
//	//循环插入构建R树
//	GeoEnvelope rect;
//	OGREnvelope env;
//	long FID = -1;	//要素ID号
//	for (int i = 0; i < poLayer->GetFeatureCount(); i ++)
//	{
//		FID = poLayer->GetFeature(i)->GetFID();
//		poLayer->GetFeature(i)->GetGeometryRef()->getEnvelope(&env);
//		rect.maxX = env.MaxX;
//		rect.minX = env.MinX;
//		rect.maxY = env.MaxY;
//		rect.minY = env.MinY;
//		Insert(FID,rect);		//逐个要素插入
//	}
//}

void CRtreeIndex::Insert(long key,const GeoEnvelope &itemRect)
{
	RTreeNode *newNode = NULL;		 //存储合适的节点
	RTreeNode *splitNode = NULL;	//分裂出来的节点
	newNode = ChooseLeaf(m_pRootNode,(GeoEnvelope &)itemRect);	//寻找一个合适的叶节点
	if (newNode && newNode->nCount < M)	//如果节点中还有剩余的空间，则插入
	{
		newNode->pBranch[newNode->nCount].pChild = (RTreeNode*)&key;
		newNode->pBranch[newNode->nCount].MBR = itemRect;
		newNode->nCount += 1;	  //个数加1
		AdjustRTree(newNode,splitNode);
	}

	else	//如果节点数等于M，则插入会产生节点溢出，需要节点分裂
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
		SplitNode(newNode,branchVec,&splitNode);//分裂节点

		RTreeNode *aNode = RTreeNewNode();
		aNode->pBranch[0].pChild = newNode;
		aNode->pBranch[1].pChild = splitNode;
		aNode->nLevel = newNode->nLevel + 1;
		aNode->nCount = 2;

		//给父节点赋值
		newNode->pParent = aNode;
		splitNode->pParent = aNode;

		AdjustRTree(newNode,splitNode);
	}

	RTreeNode *pRootNode = m_pRootNode;

	//生成新的根节点
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

		//给父节点赋值
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

	if (pRootNode->nLevel == 0)	 //如果是叶子节点
	{
		for (int i = 0; i < pRootNode->nCount; i ++)
		{
			if (pRootNode->pBranch[i].MBR.Contains(cx,cy))
			{
				ItemSearched.push_back((long)(pRootNode->pBranch[i].pChild));	//将元素的ID号加入到数组中
			}
		}
	}

	else	//如果是中间节点
	{
		for (int i = 0; i < pRootNode->nCount; i++)
		{
			if (pRootNode->pBranch[i].MBR.Contains(cx,cy))	//如果相交，则对子树递归调用
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

	//如果节点是叶子节点
	if ( pRootNode->nLevel == 0)
	{
		for (int i = 0; i < pRootNode->nCount; i ++)
		{
			if (pRootNode->pBranch[i].MBR.Intersects(searchBox))	//如果相交，则进入选择集
			{
				ItemSearched.push_back((int)pRootNode->pBranch[i].pChild);	//将元素的ID号加入到数组中
			}
		}
	}

	else if (pRootNode->nLevel > 0)	//如果是中间节点
	{
		for (int i = 0; i < pRootNode->nCount; i++)
		{
			if (pRootNode->pBranch[i].MBR.Intersects(searchBox))	//如果相交，则对子树递归调用
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
	int locIndex = -1;	//要删除要素在节点中的位置
	RTreeNode *Node = FindLeaf(m_pRootNode,(GeoEnvelope &)itemBox,&locIndex);	//寻找合适的叶子节点
	if (Node == NULL)
	{
		return false;		//如果没找到，直接终止删除操作
	}

	//从找到的节点中删除要素
	for (int i = locIndex;i<Node->nCount;i++)
	{
		Node->pBranch[i] = Node->pBranch[i+1];
	}
	Node->nCount--;	//元素个数减1

	//调整树的结构
	CondenseTree(Node);

	if (m_pRootNode->nCount == 1)	   //如果根节点只有一个子节点，就把这个子节点作为根节点
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
	//   如果是叶子节点
	if (a_node && a_node->nLevel == 0)
	{
		for (int i = 0; i < a_node->nCount; i ++)
		{
			if (a_node->pBranch[i].MBR.Contains(delRect))
			{
				nIndex = &i;  //索引记录在节点中的位置
				return a_node;
			}
		}
	}

	//  如果是中间节点
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
	RTreeNode *Node = l_node;		//令Node为被删除单元的叶子节点
	vector<RTreeNode *> QnodeVector(0);		//节点的集合，用于存放被删除的节点
	QnodeVector.clear();

	int pIndex = -1;	//在父节点中的索引号

	RTreeNode *parentNode = NULL;	//父节点
	while (parentNode = Node->pParent)
	{
		for (int i = 0; i < parentNode->nCount; i ++)
		{
			if (parentNode->pBranch[i].pChild == Node)	//寻找在父节点中的单元
			{
				pIndex = i;
			}
		}
		if (Node->nCount < M/2)   //如果节点下溢
		{
			//在父节点中删除Node
			parentNode->pBranch[pIndex].pChild = NULL;	//断开和子节点的联系
			for (int i = pIndex;i<parentNode->nCount;i++)
			{
				parentNode->pBranch[i] = parentNode->pBranch[i+1];
			}
			parentNode->nCount--;

			QnodeVector.push_back(Node);	//将节点加入到QnodeVector中
		}
		//调整节点Node所在父节点单元的MBR，使其包括Node的所有单元
		parentNode->pBranch[pIndex].MBR = GetMBR(Node);

		Node = parentNode;
	}

	//重新插入
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
	//从根节点开始查找
	if (root != NULL)
	{
		RTreeNode *Node = root;		//初始化为根节点
		if (Node->nLevel == 0)		//如果是叶子节点，直接返回
		{
			return Node;
		}

		double *deltaArea = new double[Node->nCount];	//面积增量
		while (Node->nLevel != 0 && Node != NULL)		//如果不是叶子节点
		{
			//定义各个单元的面积增加量
			GeoEnvelope newRect;
			for (int i = 0; i < Node->nCount; i ++)	//找出面积增加量最小的节点
			{
				newRect.minX = min(Node->pBranch[i].MBR.minX,itemRect.minX);
				newRect.maxX = max(Node->pBranch[i].MBR.maxX,itemRect.maxX);
				newRect.minY = min(Node->pBranch[i].MBR.minY,itemRect.minY);
				newRect.maxY = max(Node->pBranch[i].MBR.maxY,itemRect.maxY);
				deltaArea[i] = newRect.GetArea() - Node->pBranch[i].MBR.GetArea();	//面积增量
			}

			//找出面积最小的节点
			double minDelArea = DBL_MAX;
			int nIndex = -1;
			for (int i = 0; i < Node->nCount; i ++)
			{
				if (deltaArea[i] < minDelArea)
				{
					minDelArea = deltaArea[i];
					nIndex = i;		//计算最小元素所在的下标
				}
			}

			Node = Node->pBranch[nIndex].pChild;

			ChooseLeaf(Node,itemRect);	//在增量最小的单元中递归查找
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
	RTreeNode *NN = RNode;	//因分裂而产生的新节点（可能没有）

	int pIndex = -1;	//该节点在父节点中的位置
	RTreeNode *parentNode = NULL;	//父节点
	while (parentNode = N->pParent)
	{
		for (int i = 0; i < parentNode->nCount; i ++)
		{
			if (parentNode->pBranch[i].pChild == N)	//寻找在父节点中的单元
			{
				pIndex = i;
			}
		}
		parentNode->pBranch[pIndex].MBR = GetMBR(N);	//调整MBR

		RTreeNode *newNode = NULL;
		if (NULL != NN)	 //如果存在有分裂的节点
		{
			parentNode->pBranch[parentNode->nCount].pChild = NN;
			parentNode->pBranch[parentNode->nCount].count = NN->nCount;
			parentNode->pBranch[parentNode->nCount].MBR = GetMBR(NN);
			parentNode->nCount += 1;

			if (parentNode->nCount > M)	//节点个数超过最大节点数
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
				SplitNode(parentNode,branchVec,&newNode);//分裂节点
			}
		}

		N = parentNode;
		NN = newNode;
	}

}

void CRtreeIndex::SplitNode(RTreeNode*& a_node, vector<IndexEntry>& a_branch,RTreeNode** a_newNode)
{
	//先选择种子矩形填入两个节点中
	int l1,l2;
	PickSeeds(a_branch,&l1,&l2);	//选取初始的种子矩形
	vector<IndexEntry> firstNode;	 //第一个矩形集合
	for (int i = 0; i < a_node->nCount; i ++)
	{
		firstNode.push_back(a_node->pBranch[i]);
	}

	vector<IndexEntry> secondNode;					 //第二个矩形集合
	*a_newNode = RTreeNewNode();
	for (int i = 0; i < (*a_newNode)->nCount; i ++)
	{
		secondNode.push_back((*a_newNode)->pBranch[i]);
	}
	//a_node = RTreeNewNode();

	//把选取的矩形分别赋给两类
	a_node->pBranch[0].MBR = a_branch[l1].MBR;
	a_node->pBranch[0].pChild = a_branch[l1].pChild;
	a_node->pBranch[0].count = 0;
	a_node->nCount = 1;

	(*a_newNode)->pBranch[0].MBR = a_branch[l2].MBR;
	(*a_newNode)->pBranch[0].pChild = a_branch[l2].pChild;
	(*a_newNode)->pBranch[0].count = 0;
	(*a_newNode)->nCount += 1;
	(*a_newNode)->nLevel = a_node->nLevel;


	//把删除标记设为true
	//a_branch.at(l1).IsDelete = true;
	//a_branch.at(l2).IsDelete = true;

	for (int i = 0; i < (int)a_branch.size(); i ++)
	{
		//如果是矩形集中已经分配完毕，则终止
		int cnt = 0;	//统计标记未删除的元素
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
			return;		//直接返回
		}

		//未分配的元素+某一类中的元素=m，则终止分配
		if (cnt+a_node->nCount == M/2 || cnt+(*a_newNode)->nCount == M/2)
		{
			//则将所有矩形分配给这个节点
			for (int i = 0; i < (int)a_branch.size(); i ++)
			{
				//if (!a_branch.at(i).IsDelete)	//没有分配的
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
		int l3 = -1;		//下一个要分配的矩形序号
		l3 = PickNext(a_node,*a_newNode,a_branch);	
		//a_branch.at(l3).IsDelete = true;

		GeoEnvelope rect1,rect2;
		rect1 = GetMBR(a_node);
		rect2 = GetMBR(*a_newNode);

		//选择面积增加最小的作为插入的节点
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
	double *area = new double[count];		//各个矩形单元的面积
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
		//	//生成新的矩形，是包括第i个和j个元素
		//	GeoEnvelope newRect = rect1.Merge(rect2);
		//	if (i != j && worst <= newRect.GetArea()-area[i]-area[j])		//寻找矩形增量最大的
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
	//int nSelected = -1;		//选择的矩形索引
	//int count = branch.size();
	//for (int i = 0; i < count; i ++)
	//{
	//	if (branch.at(i).IsDelete == false)
	//	{
	//		double d1,d2;	//两类的面积增加值
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
		//初始化每一个分支
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
	// 从根节点开始查找
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

