#include "GeoDefine.h"

//交换字节的顺序
extern void GeoSwapBytes(void* pBuf,int nLen);

void GeoSwapBytes(void* pBuf,int nLen)
{
	if (NULL == pBuf)
	{
		return;
	}
	GByte* poBytes = (GByte*)pBuf;
	GByte uTemp = 0;
	for (int i = 0; i < nLen/2; i ++)
	{
		uTemp = poBytes[i];
		poBytes[i] = poBytes[nLen-i-1];
		poBytes[nLen-i-1] = uTemp;
	}
}

//获得机器的字节序,小伟端返回1
extern GeoByteOrder GetHostOrder();

extern GeoByteOrder GetHostOrder()
{
	int a = 2;
	GByte *poByte = (GByte*)&a;
	if (2 == *poByte)
	{
		return gWkbNDR;
	}

	return gWkbXDR;
}

#if defined linux

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

static void _split_whole_name(const char *whole_name, char *fname, char *ext);

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	char *p_whole_name;

	drive[0] = '\0';
	if (NULL == path)
	{
		dir[0] = '\0';
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}

	if ('/' == path[strlen(path)])
	{
		strcpy(dir, path);
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}

	p_whole_name = rindex((char*)path, '/');
	if (NULL != p_whole_name)
	{
		p_whole_name++;
		_split_whole_name(p_whole_name, fname, ext);

		snprintf(dir, p_whole_name - path, "%s", path);
	}
	else
	{
		_split_whole_name(path, fname, ext);
		dir[0] = '\0';
	}
}

static void _split_whole_name(const char *whole_name, char *fname, char *ext)
{
	char *p_ext;

	p_ext = rindex((char*)whole_name, '.');
	if (NULL != p_ext)
	{
		strcpy(ext, p_ext);
		snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
	}
	else
	{
		ext[0] = '\0';
		strcpy(fname, whole_name);
	}
}

#endif

