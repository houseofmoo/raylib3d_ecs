#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec4 vertexColor;

out vec2 fragTexCoord;
out vec4 fragColor;

uniform mat4 mvp;
uniform float outlineSize; // how much to expand along the normal

void main()
{
    // Push the vertex outward along its normal
    vec3 inflatedPos = vertexPosition + vertexNormal * outlineSize;

    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    gl_Position = mvp * vec4(inflatedPos, 1.0);
}
