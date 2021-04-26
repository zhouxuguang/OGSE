#version 310 es

precision mediump float;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; // ��Դλ�ã��۾����꣩
uniform vec3 Kd;            // ������ϵ��
uniform vec3 Ld;            // �������ǿ��

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;		//ֱ�Ӵ���MVP������Ϊ�˼���ÿ������ļ�����

void main()
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);		//����ת�����۾�����
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition - eyeCoords));

    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );		//����������

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
