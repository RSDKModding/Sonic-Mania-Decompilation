#version 330 core
in vec2 ex_UV;
out vec4 out_color;

uniform sampler2D tex;

void main() {
    out_color = vec4(texture(tex, ex_UV).rgb, 1.0);
}