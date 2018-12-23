//
//  AffineTransform.cpp
//  Math3D
//
//  Created by zhouxuguang on 16/10/9.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "AffineTransform.h"

NS_MATH3D_BEGIN

AffineTransform CreateAffineTransform(Real a, Real b, Real c, Real d, Real tx, Real ty)
{
    AffineTransform tran;
    tran.a = a;
    tran.b = b;
    tran.c = c;
    tran.d = d;
    tran.tx = tx;
    tran.ty = ty;
    
    return tran;
}

AffineTransform AffineTransformInvert(const AffineTransform& t)
{
    Real determinant = 1.0 / (t.a * t.d - t.b * t.c);
    
    return CreateAffineTransform(determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
                                   determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) );
}

bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2)
{
    return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}

NS_MATH3D_END
