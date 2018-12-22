#include "GeoMappingFile.h"

#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

static int nAllocSize = 0;

GeoMappingFile::GeoMappingFile()
{
	m_pszFileName = NULL;
	m_MapData = NULL;
	m_FileHandle = NULL;
	m_MapHandle = NULL;

	m_nCurOffset = 0;

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	nAllocSize = info.dwAllocationGranularity;

	m_pBuf = new unsigned char[65536];
}

GeoMappingFile::~GeoMappingFile(void)
{
	Close();
	if (m_pBuf != NULL)
	{
		delete []m_pBuf;
		m_pBuf = NULL;
	}
}

bool GeoMappingFile::Open(const char* pszFileName,const char* pszAccess)
{
	m_pszFileName = strdup(pszFileName);
	m_MapData = NULL;

	m_FileHandle = ::CreateFile(pszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	m_Access = 0;

	//创建映射文件对象.
	m_MapHandle = ::CreateFileMapping(m_FileHandle,NULL,PAGE_READWRITE ,0, 0,NULL);

	return true;
}

void GeoMappingFile::Close()
{
	if (m_pszFileName != NULL)
	{
		free(m_pszFileName);
		m_pszFileName = NULL;
	}

	if (m_MapData != NULL)
	{
		::UnmapViewOfFile(m_MapData);
		m_MapData = NULL;
	}

	if (m_FileHandle != NULL)
	{
		::CloseHandle(m_FileHandle);
		m_FileHandle = NULL;
	}

	if (m_FileHandle != NULL)
	{
		::CloseHandle(m_FileHandle);
		m_FileHandle = NULL;
	}
}

void* GeoMappingFile::GetMappingBuf(unsigned long long nOffset,unsigned long long nLength,ACCESS_TYPE eType)
{
	DWORD dwAccess = 0;
	if (eType == READ_ONLY)
	{
		dwAccess = FILE_MAP_READ;
	}
	else if (eType == WRITE_ONLY)
	{
		dwAccess = FILE_MAP_WRITE;
	}
	else if (eType == READ_WRITE)
	{
		dwAccess = FILE_MAP_COPY;
	}

	//确定映射的起始位置
	int bAign = nOffset%nAllocSize == 0;
	int nLeftSize = 0;
	if (!bAign)
	{
		nLeftSize = nOffset%nAllocSize;
		nOffset = nOffset - nLeftSize;
	}

	m_MapData = (unsigned char*)::MapViewOfFile(m_MapHandle, FILE_MAP_COPY, 0, 
				nOffset, nLength + nLeftSize );

	return (unsigned char*)m_MapData + nLeftSize;
}

void* GeoMappingFile::GetMappingBuf(unsigned long long nLength)
{
	m_MapData = (unsigned char*)::MapViewOfFile(m_MapHandle, FILE_MAP_COPY, 0, 
		0, 0  );

	return (unsigned char*)m_MapData ;
}

unsigned char* GeoMappingFile::GetBufData() const
{
	return (unsigned char*)m_MapData;
}

long long GeoMappingFile::GetOffset() const
{
	return m_nCurOffset;
}

void GeoMappingFile::SetOffset(long long nOffset)
{
	m_nCurOffset = nOffset;
}

//解除映射
void GeoMappingFile::UmMapData()
{
	if (m_MapData != NULL)
	{
		::UnmapViewOfFile(m_MapData);
		m_MapData = NULL;
	}
}

#elif defined linux
	//linux

#endif
