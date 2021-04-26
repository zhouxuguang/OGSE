#version 310 es

precision mediump float;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; // 光源位置（眼睛坐标）
uniform vec3 Kd;            // 漫反射系数
uniform vec3 Ld;            // 漫反射光强度

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;		//直接传入MVP矩阵是为了减少每个顶点的计算量

void main()
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);		//法线转换到眼睛坐标
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition - eyeCoords));

    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );		//漫反射光计算

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
