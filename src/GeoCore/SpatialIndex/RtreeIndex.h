/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: Rtree.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��6��28��
@ ����ժҪ	: R���ռ�������ʵ��
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__
#define __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__


#include <vector>

#include "ISpatialIndex.h"

#include "GeoEnvelope.h"

using namespace std;

const int page_size = 512;		//ҳ���С

//#define M (int)((page_size-(2*sizeof(int))) / sizeof(IndexEntry))

#define M 10


struct IndexEntry	  //������ݽṹ
{
	int count;					//ָ����ӽڵ��Ԫ�ظ���
	struct RTreeNode* pChild;	//���ӽڵ�(Ҷ�ӽڵ����ID����Ҷ�ӽڵ���Ǻ��ӽڵ�ָ��)
	GeoEnvelope MBR;			//�ռ�Ŀ�����С��Ӿ���

};

typedef struct RTreeNode		//R���ڵ����ݽṹ 
{
	int nCount;						//�ڵ�Ԫ�صĸ���
	int nLevel;						//�ڵ������Ĳ�� ,0��ʾҶ�ӽڵ�
	struct RTreeNode* pParent;		//���ڵ�ָ��
	IndexEntry *pBranch;			//�ڵ�Ԫ������
}RTreeNode,*RTree;


class CRtreeIndex : public ISpatialIndex
{
private:
	RTreeNode *m_pRootNode;	//���ڵ�ָ��

public:

	CRtreeIndex(void);
	~CRtreeIndex(void);

	//����R��
	//void BuildRtTree(OGRLayer *poLayer);

	//����������������ӵ�������
	virtual void Insert(long key,const GeoEnvelope &itemRect);

	//���ѯ: ��ѯ�����������ѡ�е�������¼,�����¼ID��������
	virtual void Query(double cx, double cy, vector<long>& ItemSearched );

	//�����ѯ����ѯ�����������ΧsearchBox�ཻ��������¼,�����¼ID��������
	//����ID��ͼ���в�ѯ��ص������,�˲�ѯΪ�ཻ��ѯ���������������ѯ��
	virtual void Query(const GeoEnvelope& searchBox, vector<long>& ItemSearched );

	//ɾ�� �� ɾ��ָ��������¼,�ɹ�����true��ʧ�ܣ�����false
	virtual bool Delete(long itemID,const GeoEnvelope& itemBox);

	//Ѱ��һ�����ʵ�Ҷ�ӽڵ�
	RTreeNode *FindLeaf(RTreeNode *a_node,GeoEnvelope& delRect,int* nIndex);

	//�������ṹ
	void CondenseTree(RTreeNode *l_node);

	//���£�����(itemID,oldItemMBR) to (itemID,newItemMBR)
	virtual void Update(long itemID, const GeoEnvelope& oldItemMBR, const GeoEnvelope& newItemMBR);

	//Ѱ��һ�����ʵ�Ҷ�ӽڵ�
	RTreeNode* ChooseLeaf(RTreeNode *root,GeoEnvelope &itemRect);

	//����R���Ľṹ
	void AdjustRTree(RTreeNode *LNode,RTreeNode *RNode);

	//R���ڵ�ķ���(ƽ�������㷨)
	void SplitNode(RTreeNode*& a_node, vector<IndexEntry>& a_branch,RTreeNode** a_newNode);

	//ѡȡ���Ӿ���
	void PickSeeds(vector<IndexEntry>,int* pos1,int* pos2);

	//ѡ����з���ľ���
	int PickNext(RTreeNode* a_node, RTreeNode *otherNode,vector<IndexEntry> branch);

	//��ʼ���ڵ��ֵ
	void RTreeInitNode( RTreeNode *node);

	//��ʼ���ڵ�ķ�֧
	void RTreeInitBranch( IndexEntry *br );

	//�½�һ��R���ڵ�
	RTreeNode *RTreeNewNode(void);

	//����R���ڵ�ĸ��ڵ�
	RTreeNode* GetParentNode(RTreeNode *node);


	GeoEnvelope& GetMBR(RTreeNode *rnode); //��ȡ�����ռ��MBR
	//virtual UINT GetIndexType(); //��ȡ����������
};

#endif // end of __RTREE_H_9BE6A67C_162B_4D8A_A193_7793D8974DBC__
