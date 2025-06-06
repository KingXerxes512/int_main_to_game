#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

out vec3 normal;
out vec2 texCoord;
out vec4 fragCoord;

uniform mat4 model;

layout(std140, binding = 0) uniform camera
{
    mat4 view;
    mat4 projection;
    vec3 eye;
};

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    vec4 norm4 = model * vec4(in_normal, 0.0);
    normal = norm4.xyz;
    texCoord = in_uv;

    fragCoord = model * vec4(in_position, 1.0);
}
