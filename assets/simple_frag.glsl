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

layout(std140, binding = 1) uniform lights
{
    vec3 ambient;
    vec3 direction;
    vec3 direction_color;
    vec3 point;
    vec3 point_color;
};

vec3 calcAmbient()
{
    return ambient;
}

vec3 calcDirection()
{
    vec3 light_dir = normalize(-direction);
    float diff = max(dot(normal, light_dir), 0.0);
    return diff * direction;
}

vec3 calcPoint()
{
    vec3 lightDir = normalize(point - fragCoord.xyz);
    float diff = max(dot(normal, lightDir), 0.0) * 25.0;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normalize(eye - fragCoord.xyz), reflectDir), 0.0), 32) * (50.0 * texture(tex1, texCoord).r);

    float intensity = length(point - fragCoord.xyz);
    return ((diff + spec) / intensity) * direction_color;
}

void main()
{
    vec4 sourceColor = texture(tex0, texCoord);
    vec3 amb_color = calcAmbient();
    vec3 dir_color = calcDirection();
    vec3 point_color = calcPoint();

    frag_color = vec4((amb_color + dir_color + point_color) * sourceColor.rgb, 1.0);
}
