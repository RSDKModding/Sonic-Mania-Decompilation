#version 330 core
in vec3 in_pos;
in vec2 in_UV;
out vec2 ex_UV;

// uniform mat4 model;
uniform mat4 view; // set to identity for screen based
uniform mat4 projection;

void main()
{
    gl_Position = view * projection * vec4(in_pos, 1.0);
    ex_UV       = in_UV;
}