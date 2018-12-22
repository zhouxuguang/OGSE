#ifndef OPENGL_UTIL_SKYBOX_H
#define OPENGL_UTIL_SKYBOX_H

//#include "drawable.h"

class SkyBox //: public Drawable
{
private:
    unsigned int vaoHandle;

public:
    SkyBox();

    void render() const;
};


#endif // OPENGL_UTIL_SKYBOX_H
