/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2012
@ 文件名称	: QuadTree.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年10月8日
@ 内容摘要	: 四叉树功能代码
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/


#ifndef __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__
#define __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__

#include <vector>

#include "ogrsf_frmts.h"




/* 一个矩形区域的象限划分：:

UL(1)   |    UR(0)
----------|-----------
LL(2)   |    LR(3)
以下对该象限类型的枚举
*/
typedef enum
{
	UR = 0,
	UL = 1,
	LL = 2,
	LR = 3
}QuadrantEnum;

struct MapRect		 //MBR
{
	double minX;	//最小横坐标
	double maxX;	//最大横坐标
	double minY;	//最小纵坐标
	double maxY;	//最大纵坐标

	//矩形相交
	int Intersects(MapRect const& other) const
	{
		/*if (minX > other.maxX || maxX < other.minX || 
			minY > other.maxY || maxY < other.minY)
		{
			return 0;
		}
		return 1;*/
		return minX <= other.maxX && maxX >= other.minX && 
			minY <= other.maxY && maxY >= other.minY;
	}

	//矩形包含关系
	bool Contains(const MapRect& other)
	{
		return minX <= other.minX && minY <= other.minY &&
			maxX >= other.maxX && maxY >= other.maxY;
	}

	//矩形合并
	MapRect Merge(const MapRect& other)
	{
		MapRect rect;
		rect.minX = std::min(minX,other.minX);
		rect.maxX = std::max(maxX,other.maxX);
		rect.minY = std::min(minY,other.minY);
		rect.maxY = std::max(maxY,other.maxY);
		return rect;
	}

	/*******************************************************************************
	@ 函数名称	: Split
	@ 功能描述	: 矩形分割，一分为四
	@ 参　　数	: MapRect *rect0
	@ 参　　数	: MapRect *rect1
	@ 参　　数	: MapRect *rect2
	@ 参　　数	: MapRect *rect3
	@ 返回值	: void
	@ 作　　者	: guang
	@ 设计日期	: 2013年月日
	@ 修改日期		   修改人		  修改内容
	*******************************************************************************/
	void Split(MapRect *rect0,MapRect *rect1,
		MapRect *rect2,MapRect *rect3)
	{		
		//右上
		rect0->minX = (minX+maxX)/2;
		rect0->maxX = maxX;
		rect0->minY = (minY+maxY)/2;
		rect0->maxY = maxY;

		//左上
		rect1->minX = minX;
		rect1->maxX = (minX+maxX)/2;
		rect1->minY = (minY+maxY)/2;
		rect1->maxY = maxY;

		//左下
		rect2->minX = minX;
		rect2->maxX = (minX+maxX)/2;
		rect2->minY = minY;
		rect2->maxY = (minY+maxY)/2;

		//右下
		rect3->minX = (minX+maxX)/2;
		rect3->maxX = maxX;
		rect3->minY = minY;
		rect3->maxY = (minY+maxY)/2;

	}

	double GetArea() const
	{
		double dArea = 0.0;	//面积
		return fabs(maxX-minX)*fabs(maxY-minY);
	}

	//判断点是否在矩形内
	bool IsPointInRect(double x,double y)
	{
		if (x >= minX && x <= maxX && 
			y >= minY && y <= maxY)
		{
			return true;
		}

		return false;
	}

	MapRect &operator = (MapRect other)	//重载等号运算符
	{
		minX = other.minX;
		maxX = other.maxX;
		minY = other.minY;
		maxY = other.maxY;

		return *this;
	}

};

/*空间对象MBR信息*/
typedef struct SHPMBRInfo
{
	int nID;		//空间对象ID号
	MapRect Box;	//空间对象MBR范围坐标
}SHPMBRInfo;

/* 四叉树节点类型结构 */
typedef struct QuadNode
{
	MapRect		Box;			//节点所代表的矩形区域
	int			nShpCount;		//节点所包含的所有空间对象个数
	SHPMBRInfo* pShapeObj;		//空间对象指针数组
	int		nChildCount;		//子节点个数
	QuadNode  *children[4];		//指向节点的四个孩子
}QuadNode;

/* 四叉树类型结构 */
typedef struct quadtree_t
{
	QuadNode  *root;
	int         depth;           // 四叉树的深度
}QuadTree;


	//初始化四叉树节点
	QuadNode *InitQuadNode();

	//层次创建四叉树方法（满四叉树）
	void CreateQuadTree(int depth,OGRLayer *poLayer,QuadTree* pQuadTree);

	//创建各个分支
	void CreateQuadBranch(int depth,MapRect &rect,QuadNode** node);

	//构建四叉树空间索引
	void BuildQuadTree(OGRLayer *poLayer,QuadTree* pQuadTree);

	//四叉树索引查询(矩形查询)
	void SearchQuadTree(QuadNode* node,MapRect &queryRect,std::vector<int>& ItemSearched);

	//四叉树索引查询(矩形查询)并行查询
	void SearchQuadTreePara(std::vector<QuadNode*> resNodes,MapRect &queryRect,std::vector<int>& ItemSearched);

	//四叉树的查询（点查询）
	void PtSearchQTree(QuadNode* node,double cx,double cy,std::vector<int>& ItemSearched);

	//将指定的空间对象插入到四叉树中
	void Insert(long key,MapRect &itemRect,QuadNode* pNode);

	//将指定的空间对象插入到四叉树中
	void InsertQuad(long key,MapRect &itemRect,QuadNode* pNode);

	//将指定的空间对象插入到四叉树中
	void InsertQuad2(long key,MapRect &itemRect,QuadNode* pNode);

	//判断一个节点是否是叶子节点
	bool IsQuadLeaf(QuadNode* node);

	//删除多余的节点
	bool DelFalseNode(QuadNode* node);

	//四叉树遍历(所有要素)
	void TraversalQuadTree(QuadNode* quadTree,std::vector<int>& resVec);

	//四叉树遍历（所有节点）
	void TraversalQuadTree(QuadNode* quadTree,std::vector<QuadNode*>& arrNode);

	//释放树的内存空间
	void ReleaseQuadTree(QuadNode** quadTree);

	//计算四叉树所占的字节的大小
	long CalByteQuadTree(QuadNode* quadTree,long& nSize);


#endif

