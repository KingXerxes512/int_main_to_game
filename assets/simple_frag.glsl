#version 460 core

in vec3 normal;
in vec2 texCoord;
in vec4 fragCoord;
out vec4 frag_color;

uniform sampler2D tex0;
uniform sampler2D tex1;

layout(std140, binding = 0) uniform camera
{
    mat4 view;
    mat4 projection;
    vec3 eye;
};

struct PointLight
{
    vec3 point;
    vec3 pointColor;
    vec3 attenuation;
};

layout(std430, binding = 1) readonly buffer lights
{
    vec3 ambient;
    vec3 direction;
    vec3 direction_color;
    int numPoints;
    PointLight points[];
};

vec3 calcAmbient()
{
    return ambient;
}

vec3 calcDirection()
{
    vec3 light_dir = normalize(-direction);
    float diff = max(dot(normal, light_dir), 0.0);
    return diff * direction_color;
}

vec3 calcPoint(int index)
{
    vec3 attenuation = points[index].attenuation;
    vec3 point = points[index].point;
    vec3 pointColor = points[index].pointColor;

    float distance = length(point - fragCoord.xyz);
    float atten = 1.0 / (attenuation.x + (attenuation.y * distance) + (attenuation.z * (distance * distance)));

    vec3 lightDir = normalize(point - fragCoord.xyz);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normalize(eye - fragCoord.xyz), reflectDir), 0.0), 32) * texture(tex1, texCoord).r;

    return ((diff + spec) * atten) * pointColor;
}

void main()
{
    vec4 sourceColor = texture(tex0, texCoord);
    vec3 color = calcAmbient();
    color += calcDirection();

    for (int i = 0; i < numPoints; ++i)
    {
        color += calcPoint(i);        
    }

    frag_color = vec4(color * sourceColor.rgb, 1.0);
}
