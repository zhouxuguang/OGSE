/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
@ �ļ�����	: ISpatialIndex.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2012��11��28��
@ ����ժҪ	: �ռ���������
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__
#define __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__

#include <vector>

#include "GeoDefine.h"

class GeoEnvelope;

//�ռ������Ļ���
class GEOMETRY_API ISpatialIndex
{
public:
	//����������������ӵ�������
	virtual void Insert(long key,const GeoEnvelope &itemRect)=0;

	//���ѯ: ��ѯ�����������ѡ�е�������¼,�����¼ID��������
	virtual void Query(double cx, double cy, vector<long>& ItemSearched )=0;

	//�����ѯ����ѯ�����������ΧsearchBox�ཻ��������¼,�����¼ID��������
	//����ID��ͼ���в�ѯ��ص������,�˲�ѯΪ�ཻ��ѯ���������������ѯ��
	virtual void Query(const GeoEnvelope& searchBox, vector<long>& ItemSearched )=0;

	//ɾ�� �� ɾ��ָ��������¼,�ɹ�����true��ʧ�ܣ�����false
	virtual bool Delete(long itemID,const GeoEnvelope& itemBox)=0;

	//���£�����(itemID,oldItemMBR) to (itemID,newItemMBR)
	virtual void Update(long itemID, const GeoEnvelope& oldItemMBR, const GeoEnvelope& newItemMBR)=0;

	//virtual GeoEnvelope GetMBR(RTreeNode *rnode); //��ȡ�����ռ��MBR
	virtual int GetIndexType()=0; //��ȡ����������

	ISpatialIndex(void);
	virtual ~ISpatialIndex(void);

protected:
private:
};

#endif // end of __ISPATIALINDEX_H_EAF91DAE_8BF1_4C90_BCE7_3D2EA8275D52__
