#version 460

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in vec4 aColor;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    vertexColor = aColor;
}
