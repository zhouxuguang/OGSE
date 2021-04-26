#ifndef VBOMESH_H
#define VBOMESH_H

#include "Matrix4x4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "GLPrecompile.h"

USING_NS_MATH3D

class VBOMesh
{
private:
    unsigned int faces;
    unsigned int vaoHandle;

    void trimString( std::string & str );
    
    void storeVBO( const std::vector<Vector3> & points,
                            const std::vector<Vector3> & normals,
                            const std::vector<Vector2> &texCoords,
                            const std::vector<Vector4> &tangents,
                            const std::vector<int> &elements );
    
    void generateAveragedNormals(
            const std::vector<Vector3> & points,
            std::vector<Vector3> & normals,
            const std::vector<int> & faces );
    
    void generateTangents(
            const std::vector<Vector3> & points,
            const std::vector<Vector3> & normals,
            const std::vector<int> & faces,
            const std::vector<Vector2> & texCoords,
            std::vector<Vector4> & tangents);
    
    void center(std::vector<Vector3> &);

public:
    VBOMesh( const char * fileName, bool reCenterMesh = false );

    void render() const;

    void loadOBJ( const char * fileName, bool );
};

#endif // VBOMESH_H
