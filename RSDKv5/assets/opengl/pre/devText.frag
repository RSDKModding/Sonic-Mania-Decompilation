#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2DRect text;

void main()
{
    int show = int(texture(text, ex_UV).r * 255);

    if (show == 0)
        discard;

    out_color = vec4(ex_color.bgr, 1.0);
}