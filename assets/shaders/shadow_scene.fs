#version 330

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 fragLightSpacePos;

uniform sampler2D texture0;      // base albedo
uniform sampler2D shadowMap;     // our depth texture

uniform vec3 lightDir;           // normalized direction (from light towards scene)
uniform vec4 lightColor;

out vec4 finalColor;

// Simple shadow lookup with small bias + PCF-ish blur
float ComputeShadow(vec4 lightSpacePos)
{
    // Perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    // Transform from [-1,1] to [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // If outside the shadow map: treat as lit
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 1.0;
    }

    float currentDepth = projCoords.z;

    // Bias to reduce shadow acne
    float bias = 0.0015;

    // PCF - sample a 3x3 kernel
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vec2 offset = vec2(x, y) * texelSize;
            float closestDepth = texture(shadowMap, projCoords.xy + offset).r;
            if (currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
    }

    shadow /= 9.0;

    // 0 = fully shadowed, 1 = fully lit
    return 1.0 - shadow;
}

void main()
{
    vec3 baseColor = texture(texture0, fragTexCoord).rgb;

    // Simple diffuse lighting
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-lightDir);     // lightDir goes FROM light, so invert

    float NdotL = max(dot(N, L), 0.0);

    float shadowFactor = ComputeShadow(fragLightSpacePos);

    vec3 litColor = baseColor * (0.15 + NdotL * shadowFactor) * lightColor.rgb;

    finalColor = vec4(litColor, 1.0);
}
