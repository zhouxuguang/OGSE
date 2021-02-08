/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: Rtree.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年6月28日
@ 内容摘要	: R树空间索引的实现
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__
#define __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__


#include <vector>

#include "ISpatialIndex.h"

#include "GeoEnvelope.h"

using namespace std;

const int page_size = 512;		//页面大小

//#define M (int)((page_size-(2*sizeof(int))) / sizeof(IndexEntry))

#define M 10


struct IndexEntry	  //入口数据结构
{
	int count;					//指向的子节点的元素个数
	struct RTreeNode* pChild;	//孩子节点(叶子节点就是ID，非叶子节点就是孩子节点指针)
	GeoEnvelope MBR;			//空间目标的最小外接矩形

};

typedef struct RTreeNode		//R树节点数据结构 
{
	int nCount;						//节点元素的个数
	int nLevel;						//节点所处的层次 ,0表示叶子节点
	struct RTreeNode* pParent;		//父节点指针
	IndexEntry *pBranch;			//节点元素数组
}RTreeNode,*RTree;


class CRtreeIndex : public ISpatialIndex
{
private:
	RTreeNode *m_pRootNode;	//根节点指针

public:

	CRtreeIndex(void);
	~CRtreeIndex(void);

	//构建R树
	//void BuildRtTree(OGRLayer *poLayer);

	//将给定的索引项添加到索引中
	virtual void Insert(long key,const GeoEnvelope &itemRect);

	//点查询: 查询所有与给定点选中的索引记录,保存记录ID到数组中
	virtual void Query(double cx, double cy, vector<long>& ItemSearched );

	//区域查询：查询所有与给定范围searchBox相交的索引记录,保存记录ID到数组中
	//利用ID在图层中查询相关地理对象,此查询为相交查询，可以再设包含查询等
	virtual void Query(const GeoEnvelope& searchBox, vector<long>& ItemSearched );

	//删除 ： 删除指定索引记录,成功返回true，失败，返回false
	virtual bool Delete(long itemID,const GeoEnvelope& itemBox);

	//寻找一个合适的叶子节点
	RTreeNode *FindLeaf(RTreeNode *a_node,GeoEnvelope& delRect,int* nIndex);

	//缩减树结构
	void CondenseTree(RTreeNode *l_node);

	//更新：更新(itemID,oldItemMBR) to (itemID,newItemMBR)
	virtual void Update(long itemID, const GeoEnvelope& oldItemMBR, const GeoEnvelope& newItemMBR);

	//寻找一个合适的叶子节点
	RTreeNode* ChooseLeaf(RTreeNode *root,GeoEnvelope &itemRect);

	//调整R树的结构
	void AdjustRTree(RTreeNode *LNode,RTreeNode *RNode);

	//R树节点的分裂(平方分裂算法)
	void SplitNode(RTreeNode*& a_node, vector<IndexEntry>& a_branch,RTreeNode** a_newNode);

	//选取种子矩形
	void PickSeeds(vector<IndexEntry>,int* pos1,int* pos2);

	//选择进行分配的矩形
	int PickNext(RTreeNode* a_node, RTreeNode *otherNode,vector<IndexEntry> branch);

	//初始化节点的值
	void RTreeInitNode( RTreeNode *node);

	//初始化节点的分支
	void RTreeInitBranch( IndexEntry *br );

	//新建一个R树节点
	RTreeNode *RTreeNewNode(void);

	//计算R树节点的父节点
	RTreeNode* GetParentNode(RTreeNode *node);


	GeoEnvelope& GetMBR(RTreeNode *rnode); //获取索引空间的MBR
	//virtual UINT GetIndexType(); //获取该索引类型
};

#endif // end of __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__
