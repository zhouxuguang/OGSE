//
//  AffineTransform.h  仿射变换实用函数
//  Math3D
//
//  Created by zhouxuguang on 16/10/9.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef MATH3D_AFFINETRANSFORM_INCLUDE_HDFG809GDFDSFD
#define MATH3D_AFFINETRANSFORM_INCLUDE_HDFG809GDFDSFD

#include "Math3DCommon.h"

NS_MATH3D_BEGIN

/*Affine transform
a   b    0
c   d    0
tx  ty   1

Identity
1   0    0
0   1    0
0   0    1
*/
struct MATH3D_API AffineTransform
{
    Real a, b, c, d;
    Real tx, ty;
    
    static const AffineTransform IDENTITY;
};

MATH3D_API AffineTransform CreateAffineTransform(Real a, Real b, Real c, Real d, Real tx, Real ty);

MATH3D_API bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2);

MATH3D_API AffineTransform AffineTransformInvert(const AffineTransform& t);

NS_MATH3D_END

#endif /* MATH3D_AFFINETRANSFORM_INCLUDE_HDFG809GDFDSFD */
