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
    DepthTest()
    {
        mEnabled = true;
        mFunction = DepthTestFunction::Less;
    }

    //bool Enabled { get; set; }
    //DepthTestFunction Function { get; set; }

private:
    bool mEnabled;
    DepthTestFunction mFunction;
};


#endif //RENDERENGINE_DEPTHTEST_H
