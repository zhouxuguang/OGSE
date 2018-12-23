
#include "Light.h"

NS_MATH3D_BEGIN

#define CC_ASSERT(x) assert(x)

void BaseLight::setIntensity(float intensity)
{
    CC_ASSERT(intensity >= 0);
    mIntensity = intensity;
}

void BaseLight::onEnter()
{
//    auto scene = getScene();
//    if (scene)
//    {
//        auto &lights = scene->_lights;
//        auto iter = std::find(lights.begin(), lights.end(), this);
//        if (iter == lights.end())
//            lights.push_back(this);
//    }
//    Node::onEnter();
}
void BaseLight::onExit()
{
//    auto scene = getScene();
//    if (scene)
//    {
//        auto &lights = scene->_lights;
//        auto iter = std::find(lights.begin(), lights.end(), this);
//        if (iter != lights.end())
//            lights.erase(iter);
//    }
//    Node::onExit();
}

void BaseLight::setRotationFromDirection( const Vector3 &direction )
{
//    float projLen = sqrt(direction.x * direction.x + direction.z * direction.z);
//    float rotY = CC_RADIANS_TO_DEGREES(atan2f(-direction.x, -direction.z));
//    float rotX = -CC_RADIANS_TO_DEGREES(atan2f(-direction.y, projLen));
//    setRotation3D(Vec3(rotX, rotY, 0.0f));
}

BaseLight::BaseLight(): mIntensity(1.0f), mLightFlag(BaseLight::LIGHT0), mEnabled(true)
{
    
}

BaseLight::~BaseLight()
{
    
}

NS_MATH3D_END

