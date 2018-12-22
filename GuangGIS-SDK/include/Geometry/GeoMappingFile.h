#ifndef GEO_MAPPING_FILE_INCLUDE
#define GEO_MAPPING_FILE_INCLUDE

//内存映射文件的封装

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

	//打开文件
	bool Open(const char* pszFileName,const char* pszAccess);

	void Close();

	//映射文件的某一部分区域
	void* GetMappingBuf(unsigned long long nOffset,unsigned long long nLength,ACCESS_TYPE eType = READ_WRITE);

	void* GetMappingBuf(unsigned long long nLength);

	unsigned char* GetBufData() const;

	long long GetOffset() const;

	void SetOffset(long long nOffset);

	//解除映射
	void UmMapData();

private:
	char *m_pszFileName;
	void *m_FileHandle;		//文件句柄
	void *m_MapHandle;
	void *m_MapData;	//映射数据

	unsigned char* m_pBuf;		//内部缓冲数据//规定是64k
	unsigned long m_Access;
	long long m_nCurOffset;
};

#endif
