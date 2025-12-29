#version 330

in vec3 vertexPosition;

uniform mat4 modelMatrix;
uniform mat4 lightViewProj;

void main()
{
    vec4 worldPos = modelMatrix * vec4(vertexPosition, 1.0);
    gl_Position = lightViewProj * worldPos;
}
