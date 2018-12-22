//
//  GLSLProgramCache.hpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef GLSLProgramCache_hpp
#define GLSLProgramCache_hpp

#include <stdio.h>
#include <string>
#include "PreCompile.h"

class GLSLProgram;

class GLSLProgramCache
{
public:
    GLSLProgramCache();

    ~GLSLProgramCache();
    
    static GLSLProgramCache* GetInstance();
    
    /** loads the default shaders */
    void LoadDefaultGLSLPrograms();
    
    /** returns a GL program for a given key
     */
    GLSLProgram * GetGLProgram(const std::string &key);
    
    void AddGLProgram(GLSLProgram* program, const std::string &key);
    
private:
    bool init();
    void LoadDefaultGLSLProgram(GLSLProgram *program, int type);
    
    /**Predefined shaders.*/
    std::unordered_map<std::string, GLSLProgram*> _programs;
};


#endif /* GLSLProgramCache_hpp */
