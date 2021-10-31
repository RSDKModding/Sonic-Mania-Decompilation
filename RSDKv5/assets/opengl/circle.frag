#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec3 maskColor;
uniform int maskMode;

struct ScreenInfo {
    vec2 size;
    vec2 clipRectTL;
    vec2 clipRectBR;
};

uniform ScreenInfo screen;

uniform float innerRadius;
uniform float outerRadius;

void main() {
    float dist = sqrt(dot(ex_UV, ex_UV));
    //out_color = vec4(dist, dot(pos, pos), 1.0, 1.0); 
    if (dist >= outerRadius || dist <= innerRadius) discard;
    out_color = vec4(ex_color.b, ex_color.g, ex_color.r, ex_color.a);
    if (maskMode == 1 && out_color.xyz != maskColor)
        discard;
    else if (maskMode == 2 && out_color.xyz == maskColor)
        discard;
}