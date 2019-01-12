//
// Created by Zhou,Xuguang on 2018/12/18.
//

#ifndef RENDERENGINE_DEPTHTEST_H
#define RENDERENGINE_DEPTHTEST_H


//深度测试的比较函数
enum DepthTestFunction
{
    Never,
    Less,
    Equal,
    LessThanOrEqual,
    Greater,
    NotEqual,
    GreaterThanOrEqual,
    Always
};

class DepthTest
{
public:
    DepthTest();
    
    ~DepthTest();
    
    void Enable(bool bEnable);
    
    bool IsEnable() const;
    
    void SetDepthTestFunction(DepthTestFunction function);
    
    DepthTestFunction GetDepthTestFunction() const;

private:
    bool mEnabled;
    DepthTestFunction mFunction;
};


#endif //RENDERENGINE_DEPTHTEST_H
