#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

uniform mat4 mvp;          // raylib fills this
uniform mat4 modelMatrix;  // our per-entity transform
uniform mat4 lightViewProj;

out vec3 fragWorldPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec4 fragLightSpacePos;

void main()
{
    vec4 worldPos = modelMatrix * vec4(vertexPosition, 1.0);
    fragWorldPos = worldPos.xyz;

    fragNormal = mat3(modelMatrix) * vertexNormal;
    fragTexCoord = vertexTexCoord;

    fragLightSpacePos = lightViewProj * worldPos;

    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
