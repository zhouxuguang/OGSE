//
// Created by Zhou,Xuguang on 2018/12/18.
//

#ifndef RENDERENGINE_BLENDING_H
#define RENDERENGINE_BLENDING_H

//颜色混合因子
enum BlendingFactor
{
    Zero,
    One,
    SourceAlpha,
    OneMinusSourceAlpha,
    DestinationAlpha,
    OneMinusDestinationAlpha,
    DestinationColor,
    OneMinusDestinationColor,
    SourceAlphaSaturate,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha
};

//颜色混合的计算方式
enum BlendEquation
{
    Add,
    Minimum,
    Maximum,
    Subtract,
    ReverseSubtract
};

class Blending
{
public:
    Blending()
    {
        mEnabled = false;
        mSourceRGBFactor = BlendingFactor::One;
        mSourceAlphaFactor = BlendingFactor::One;
        mDestinationRGBFactor = BlendingFactor::Zero;
        mDestinationAlphaFactor = BlendingFactor::Zero;
        mRGBEquation = BlendEquation::Add;
        mAlphaEquation = BlendEquation::Add;
        //Color = Color.FromArgb(0, 0, 0, 0);
    }

    bool IsEnabled() const;
    void Enabled(bool bEnable);

    BlendingFactor GetSourceRGBFactor() const;
    void SetSourceRGBFactor(BlendingFactor srcFactor);

    BlendingFactor GetSourceAlphaFactor() const;
    void GetSourceAlphaFactor(BlendingFactor srcFactor) const;

    BlendingFactor GetDestinationRGBFactor() const ;
    void SetDestinationRGBFactor(BlendingFactor dstFactor);

    BlendingFactor GetDestinationAlphaFactor() const ;
    void SetDestinationAlphaFactor(BlendingFactor dstFactor);

    BlendEquation GetRGBEquation() const ;
    void SetRGBEquation(BlendEquation blendEquation);

    BlendEquation GetAlphaEquation() const ;
    void SetAlphaEquation(BlendEquation blendEquation);

    //Color Color { get; set; }

private:
    bool mEnabled;
    BlendingFactor mSourceRGBFactor;
    BlendingFactor mSourceAlphaFactor;
    BlendingFactor mDestinationRGBFactor;
    BlendingFactor mDestinationAlphaFactor;
    BlendEquation mRGBEquation;
    BlendEquation mAlphaEquation;
};


#endif //RENDERENGINE_BLENDING_H
