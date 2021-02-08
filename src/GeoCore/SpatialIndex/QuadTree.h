/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: QuadTree.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��10��8��
@ ����ժҪ	: �Ĳ������ܴ���
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/


#ifndef __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__
#define __QUADTREE_H_59CAE94A_E937_42AD_AA27_794E467715BB__

#include <vector>

#include "ogrsf_frmts.h"




/* һ��������������޻��֣�:

UL(1)   |    UR(0)
----------|-----------
LL(2)   |    LR(3)
���¶Ը��������͵�ö��
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
	double minX;	//��С������
	double maxX;	//��������
	double minY;	//��С������
	double maxY;	//���������

	//�����ཻ
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

	//���ΰ�����ϵ
	bool Contains(const MapRect& other)
	{
		return minX <= other.minX && minY <= other.minY &&
			maxX >= other.maxX && maxY >= other.maxY;
	}

	//���κϲ�
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
	@ ��������	: Split
	@ ��������	: ���ηָһ��Ϊ��
	@ �Ρ�����	: MapRect *rect0
	@ �Ρ�����	: MapRect *rect1
	@ �Ρ�����	: MapRect *rect2
	@ �Ρ�����	: MapRect *rect3
	@ ����ֵ	: void
	@ ��������	: guang
	@ �������	: 2013������
	@ �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	void Split(MapRect *rect0,MapRect *rect1,
		MapRect *rect2,MapRect *rect3)
	{		
		//����
		rect0->minX = (minX+maxX)/2;
		rect0->maxX = maxX;
		rect0->minY = (minY+maxY)/2;
		rect0->maxY = maxY;

		//����
		rect1->minX = minX;
		rect1->maxX = (minX+maxX)/2;
		rect1->minY = (minY+maxY)/2;
		rect1->maxY = maxY;

		//����
		rect2->minX = minX;
		rect2->maxX = (minX+maxX)/2;
		rect2->minY = minY;
		rect2->maxY = (minY+maxY)/2;

		//����
		rect3->minX = (minX+maxX)/2;
		rect3->maxX = maxX;
		rect3->minY = minY;
		rect3->maxY = (minY+maxY)/2;

	}

	double GetArea() const
	{
		double dArea = 0.0;	//���
		return fabs(maxX-minX)*fabs(maxY-minY);
	}

	//�жϵ��Ƿ��ھ�����
	bool IsPointInRect(double x,double y)
	{
		if (x >= minX && x <= maxX && 
			y >= minY && y <= maxY)
		{
			return true;
		}

		return false;
	}

	MapRect &operator = (MapRect other)	//���صȺ������
	{
		minX = other.minX;
		maxX = other.maxX;
		minY = other.minY;
		maxY = other.maxY;

		return *this;
	}

};

/*�ռ����MBR��Ϣ*/
typedef struct SHPMBRInfo
{
	int nID;		//�ռ����ID��
	MapRect Box;	//�ռ����MBR��Χ����
}SHPMBRInfo;

/* �Ĳ����ڵ����ͽṹ */
typedef struct QuadNode
{
	MapRect		Box;			//�ڵ�������ľ�������
	int			nShpCount;		//�ڵ������������пռ�������
	SHPMBRInfo* pShapeObj;		//�ռ����ָ������
	int		nChildCount;		//�ӽڵ����
	QuadNode  *children[4];		//ָ��ڵ���ĸ�����
}QuadNode;

/* �Ĳ������ͽṹ */
typedef struct quadtree_t
{
	QuadNode  *root;
	int         depth;           // �Ĳ��������
}QuadTree;


	//��ʼ���Ĳ����ڵ�
	QuadNode *InitQuadNode();

	//��δ����Ĳ������������Ĳ�����
	void CreateQuadTree(int depth,OGRLayer *poLayer,QuadTree* pQuadTree);

	//����������֧
	void CreateQuadBranch(int depth,MapRect &rect,QuadNode** node);

	//�����Ĳ����ռ�����
	void BuildQuadTree(OGRLayer *poLayer,QuadTree* pQuadTree);

	//�Ĳ���������ѯ(���β�ѯ)
	void SearchQuadTree(QuadNode* node,MapRect &queryRect,std::vector<int>& ItemSearched);

	//�Ĳ���������ѯ(���β�ѯ)���в�ѯ
	void SearchQuadTreePara(std::vector<QuadNode*> resNodes,MapRect &queryRect,std::vector<int>& ItemSearched);

	//�Ĳ����Ĳ�ѯ�����ѯ��
	void PtSearchQTree(QuadNode* node,double cx,double cy,std::vector<int>& ItemSearched);

	//��ָ���Ŀռ������뵽�Ĳ�����
	void Insert(long key,MapRect &itemRect,QuadNode* pNode);

	//��ָ���Ŀռ������뵽�Ĳ�����
	void InsertQuad(long key,MapRect &itemRect,QuadNode* pNode);

	//��ָ���Ŀռ������뵽�Ĳ�����
	void InsertQuad2(long key,MapRect &itemRect,QuadNode* pNode);

	//�ж�һ���ڵ��Ƿ���Ҷ�ӽڵ�
	bool IsQuadLeaf(QuadNode* node);

	//ɾ������Ľڵ�
	bool DelFalseNode(QuadNode* node);

	//�Ĳ�������(����Ҫ��)
	void TraversalQuadTree(QuadNode* quadTree,std::vector<int>& resVec);

	//�Ĳ������������нڵ㣩
	void TraversalQuadTree(QuadNode* quadTree,std::vector<QuadNode*>& arrNode);

	//�ͷ������ڴ�ռ�
	void ReleaseQuadTree(QuadNode** quadTree);

	//�����Ĳ�����ռ���ֽڵĴ�С
	long CalByteQuadTree(QuadNode* quadTree,long& nSize);


#endif

