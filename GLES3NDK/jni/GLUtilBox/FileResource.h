//
//  FileResource.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/11/19.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef FileResource_hpp
#define FileResource_hpp

#include "Precompile.h"

class FileResource
{
public:
    FileResource(const char* pszFileName);
    FileResource(const std::string& strFileName);
    
    void  Seek( const eUInt64 Position );
    
    eUInt64 Read( void* Buf, const eUInt64 Size );
    
    bool    Eof() const;
    
    eUInt64  GetSize() const;
    
    eUInt64  GetPos() const;
    
private:
    FILE* m_fpFile;
    eUInt64 m_nFileSize;
};


#endif /* FileResource_hpp */
