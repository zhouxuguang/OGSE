#ifndef VBOTORUS_H
#define VBOTORUS_H

#include "GLPrecompile.h"


class VBOTorus
{
private:
    unsigned int vaoHandle;
    int faces, rings, sides;

    void generateVerts(float * , float * ,float *, unsigned int *,
                       float , float);

public:
    VBOTorus(float, float, int, int);

    void render() const;
};

#endif // VBOTORUS_H
