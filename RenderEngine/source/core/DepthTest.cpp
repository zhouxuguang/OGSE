//
// Created by Zhou,Xuguang on 2018/12/18.
//

#include "DepthTest.h"

DepthTest::DepthTest()
{
    mEnabled = false;
    mFunction = DepthTestFunction::Less;
}

DepthTest::~DepthTest()
{
}

void DepthTest::Enable(bool bEnable)
{
    mEnabled = bEnable;
}

bool DepthTest::IsEnable() const
{
    return mEnabled;
}

void DepthTest::SetDepthTestFunction(DepthTestFunction function)
{
    mFunction = function;
}

DepthTestFunction DepthTest::GetDepthTestFunction() const
{
    return mFunction;
}
