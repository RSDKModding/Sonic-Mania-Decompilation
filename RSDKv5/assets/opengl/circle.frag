#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec3 maskColor;
uniform int inkEffect;

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
    if (inkEffect == 6 && out_color.xyz != maskColor)
        discard;
    else if (inkEffect == 7 && out_color.xyz == maskColor)
        discard;
    if (inkEffect == 1) out_color = vec4(out_color.xyz, 0.5);
}