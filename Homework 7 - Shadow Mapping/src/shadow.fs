#version 330 core
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;   
        }
    }
    shadow /= 25.0;
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.8);

    // ambient
    vec3 ambient = 0.8 * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * lightColor;

    // calculate shadow
    float bias = 0.005;
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
}