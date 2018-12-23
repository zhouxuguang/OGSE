#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <algorithm>

#include "triangulate.h"

static const double EPSILON=0.0000000000000001;

float Triangulate::Area(const Vector2dVector &contour)
{
    
    int n = contour.size();
    
    float A=0.0f;
    
    for(int p=n-1,q=0; q<n; p=q++)
    {
        A+= contour[p].GetX()*contour[q].GetY() - contour[q].GetX()*contour[p].GetY();
    }
    return A*0.5f;
}

/*
 InsideTriangle decides if a point P is Inside of the triangle
 defined by A, B, C.
 */
bool Triangulate::InsideTriangle(float Ax, float Ay,
                                 float Bx, float By,
                                 float Cx, float Cy,
                                 float Px, float Py)

{
    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp;
    
    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx= Px - Ax;  apy= Py - Ay;
    bpx= Px - Bx;  bpy= Py - By;
    cpx= Px - Cx;  cpy= Py - Cy;
    
    aCROSSbp = ax*bpy - ay*bpx;
    cCROSSap = cx*apy - cy*apx;
    bCROSScp = bx*cpy - by*cpx;
    
    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
}

bool Triangulate::Snip(const Vector2dVector &contour,int u,int v,int w,int n,int *V)
{
    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py;
    
    Ax = contour[V[u]].GetX();
    Ay = contour[V[u]].GetY();
    
    Bx = contour[V[v]].GetX();
    By = contour[V[v]].GetY();
    
    Cx = contour[V[w]].GetX();
    Cy = contour[V[w]].GetY();
    
    float fCrossProdoct = (( (Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax)));
    
    if ( 0 > fCrossProdoct )
    {
        return false;
    }
    
    for (p=0;p<n;p++)
    {
        if( (p == u) || (p == v) || (p == w) ) continue;
        Px = contour[V[p]].GetX();
        Py = contour[V[p]].GetY();
        if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) return false;
    }
    
    return true;
}

/* Add BEGIN by zhouxuguang for<多边形剖分> on 2016/5/20 */

static bool PointEqual(const Vector2d& a,const Vector2d& b)
{
    return a.mX == b.mX && a.mY == b.mY;
}

/* Add END by zhouxuguang for<多边形剖分> on 2016/5/20 */

bool Triangulate::Process( const Vector2dVector &contour,Vector2dVector &result)
{
    int n = contour.size();
    if ( n < 3 ) return false;
    
    //如果是顺时针 那就返回剖分错误
    if(Area(contour)<=0.0f)
    {
        return false;
    }
    
    int *V = new int[n];
    /* we want a counter-clockwise polygon in V */
    //    if ( 0.0f < Area(contour) )
    //        for (int v=0; v<n; v++) V[v] = v;
    //    else
    //        for(int v=0; v<n; v++) V[v] = (n-1)-v;
    for (int v=0; v<n; v++) V[v] = v;
    
    int nv = n;
    
    /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2*nv;   /* error detection */

    for(int m=0, v=nv-1; nv>2; )
    {
        /* if we loop, it is probably a non-simple polygon */
        if (0 >= (count--))
        {
            delete [] V;
            //** Triangulate: ERROR - probable bad polygon!
            return false;
        }
        
        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v  ; if (nv <= u) u = 0;     /* previous */
        v = u+1; if (nv <= v) v = 0;     /* new v    */
        int w = v+1; if (nv <= w) w = 0;     /* next     */
        
        if ( Snip(contour,u,v,w,nv,V) )
        {
            int a,b,c,s,t;
            
            /* true names of the vertices */
            a = V[u]; b = V[v]; c = V[w];
            
            /* output Triangle */
            result.push_back( contour[a] );
            result.push_back( contour[b] );
            result.push_back( contour[c] );
            
            m++;
            
            /* remove v from remaining polygon */
            for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;
            
            /* resest error detection counter */
            count = 2*nv;
        }
    }
    
    
    
    delete [] V;
    
    return true;
}

/* Add BEGIN by zhouxuguang for<多边形剖分> on 2016/5/20 */

bool Triangulate::ProcessExt(Vector2dVector &contour,Vector2dVector &result)
{
    //对点集去重
    Vector2dVector::iterator iterNewEnd = std::unique(contour.begin(), contour.end(),PointEqual);
    contour.erase(iterNewEnd, contour.end());
    
    size_t n = contour.size();
    if (n < 3) {
        return false;
    }
    
    if (PointEqual(contour.front(),contour.back())) {
        contour.pop_back();
    }
    
    n = contour.size();
    if ( n < 3 )
        return false;
    
    /* allocate and initialize list of Vertices in polygon */
    if(Area(contour) <= 0.0f)
    {
        std::reverse(contour.begin(), contour.end());
    }
    
    int *V = new int[n];
    for (int v=0; v<n; v++)
        V[v] = v;
    
    int nv = n;
    int v = nv-1;
    
    /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2*nv;   /* error detection */
    int m = 0;
    
    while (nv > 2)
    {
        if (0 >= (count--))
        {
            delete []V;
            return false;
        }
        /* three consecutive vertices in current polygon, <u,v,w> */
        int u = v  ; if (nv <= u) u = 0;     /* previous */
        v = u+1; if (nv <= v) v = 0;     /* new v    */
        int w = v+1; if (nv <= w) w = 0;     /* next     */
        
        if ( Snip(contour,u,v,w,nv,V) )
        {
            int a,b,c,s,t;
            
            a = V[u]; b = V[v]; c = V[w];
            
            /* output Triangle */
            result.push_back( contour[a] );
            result.push_back( contour[b] );
            result.push_back( contour[c] );
            
            m++;
            
            /* remove v from remaining polygon */
            for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t];
            
            nv--;
            /* resest error detection counter */
            count = 2*nv;
        }
    }
    
    delete [] V;
    
    return true;
}

/* Add END by zhouxuguang for<多边形剖分> on 2016/5/20 */