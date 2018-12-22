//
//  FileResource.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/11/19.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "FileResource.h"

FileResource::FileResource(const char* pszFileName)
{
    m_fpFile = fopen(pszFileName, "rb");
    m_nFileSize = 0;
    if (m_fpFile != NULL)
    {
        fseek(m_fpFile,0,SEEK_END);
        m_nFileSize = ftell(m_fpFile);
        fseek(m_fpFile,0,SEEK_SET);
    }
}

FileResource::FileResource(const std::string& strFileName)
{
    m_fpFile = fopen(strFileName.c_str(), "rb");
    m_nFileSize = 0;
    if (m_fpFile != NULL)
    {
        fseek(m_fpFile,0,SEEK_END);
        m_nFileSize = ftell(m_fpFile);
        fseek(m_fpFile,0,SEEK_SET);
    }
}

void FileResource::Seek(const eUInt64 Position)
{
    if (NULL == m_fpFile)
    {
        return;
    }
    
    fseek(m_fpFile, Position, SEEK_SET);
}

eUInt64 FileResource::Read(void *Buf, const eUInt64 Size)
{
    if (NULL == m_fpFile)
    {
        return 0;
    }
    
    return fread(Buf, Size, 1, m_fpFile);
}

bool FileResource::Eof() const
{
    if (NULL == m_fpFile)
    {
        return false;
    }
    
    return feof(m_fpFile);
}

eUInt64 FileResource::GetSize() const
{
    //占位实现
    return m_nFileSize;
}

eUInt64 FileResource::GetPos() const
{
    return 0;
}
