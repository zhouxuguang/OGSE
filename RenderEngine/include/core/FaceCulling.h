//
// Created by Zhou,Xuguang on 2018/12/18.
//

#ifndef RENDERENGINE_FACECULLING_H
#define RENDERENGINE_FACECULLING_H

//剔除的面
enum CullFace
{
    Front,
    Back,
    FrontAndBack
};

class FaceCulling
{
public:
    FaceCulling()
    {
        //Enabled = true;
        //Face = CullFace.Back;
        //FrontFaceWindingOrder = WindingOrder.Counterclockwise;
    }

};


#endif //RENDERENGINE_FACECULLING_H
