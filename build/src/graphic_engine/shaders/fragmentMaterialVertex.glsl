#version 330 core

struct DirectinalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 color;
in vec3 normal;
in vec3 fragmentPosition;

uniform vec3 viewPos;

uniform DirectinalLight directionalLight;

out vec4 FragColor;

float getBlinnPhongSpecular(vec3 lightDir, vec3 viewDir, vec3 normal, float shininess)
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    return pow(max(dot(normal, halfwayDir), 0.0), shininess);
}

vec3 CalcDirectionalLight(DirectinalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    float spec = getBlinnPhongSpecular(lightDir, viewDir, normal, 32.0);

    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular) * color;

}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragmentPosition);

    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}
