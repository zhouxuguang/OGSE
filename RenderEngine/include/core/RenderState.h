//
// Created by Zhou,Xuguang on 2018/12/18.
//

#ifndef RENDERENGINE_RENDERSTATE_H
#define RENDERENGINE_RENDERSTATE_H

#include "RenderDefine.h"

enum ProgramPointSize
{
    Enabled,
    Disabled
};

enum RasterizationMode
{
    Point,
    Line,
    Fill
};

class RenderState
{
//    public RenderState()
//    {
//        PrimitiveRestart = new PrimitiveRestart();
//        FacetCulling = new FacetCulling();
//        ProgramPointSize = ProgramPointSize.Disabled;
//        RasterizationMode = RasterizationMode.Fill;
//        ScissorTest = new ScissorTest();
//        StencilTest = new StencilTest();
//        DepthTest = new DepthTest();
//        DepthRange = new DepthRange();
//        Blending = new Blending();
//        ColorMask = new ColorMask(true, true, true, true);
//        DepthMask = true;
//    }
//
//    public PrimitiveRestart PrimitiveRestart { get; set; }
//    public FacetCulling FacetCulling { get; set; }
//    public ProgramPointSize ProgramPointSize { get; set; }
//    public RasterizationMode RasterizationMode { get; set; }
//    public ScissorTest ScissorTest { get; set; }
//    public StencilTest StencilTest { get; set; }
//    public DepthTest DepthTest { get; set; }
//    public DepthRange DepthRange { get; set; }
//    public Blending Blending { get; set; }
//    public ColorMask ColorMask { get; set; }
//    public bool DepthMask { get; set; }
};




#endif //RENDERENGINE_RENDERSTATE_H
