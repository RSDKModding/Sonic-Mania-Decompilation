#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform float innerRadius;
uniform float outerRadius;

void main() {
    float dist = sqrt(dot(ex_UV, ex_UV));
    //out_color = vec4(dist, dot(pos, pos), 1.0, 1.0); 
    if (dist >= outerRadius || dist <= innerRadius) discard;
    out_color = vec4(ex_color.bgr, 1.0);
}