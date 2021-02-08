#ifndef GEO_MAPPING_FILE_INCLUDE
#define GEO_MAPPING_FILE_INCLUDE

//�ڴ�ӳ���ļ��ķ�װ

enum ACCESS_TYPE
{
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE,
};


class GeoMappingFile
{
public:
	GeoMappingFile();
	~GeoMappingFile(void);

	//���ļ�
	bool Open(const char* pszFileName,const char* pszAccess);

	void Close();

	//ӳ���ļ���ĳһ��������
	void* GetMappingBuf(unsigned long long nOffset,unsigned long long nLength,ACCESS_TYPE eType = READ_WRITE);

	void* GetMappingBuf(unsigned long long nLength);

	unsigned char* GetBufData() const;

	long long GetOffset() const;

	void SetOffset(long long nOffset);

	//���ӳ��
	void UmMapData();

private:
	char *m_pszFileName;
	void *m_FileHandle;		//�ļ����
	void *m_MapHandle;
	void *m_MapData;	//ӳ������

	unsigned char* m_pBuf;		//�ڲ���������//�涨��64k
	unsigned long m_Access;
	long long m_nCurOffset;
};

#endif
