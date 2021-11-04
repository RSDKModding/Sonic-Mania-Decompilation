#version 330 core
in vec2 ex_UV;
out vec4 out_color;

uniform vec2 position[0x10];
uniform vec2 deform[0x10];
uniform int starting;
uniform int type;

uniform vec2 screenSize;
uniform vec2 viewportSize;

uniform sampler2D source;
uniform sampler2D dest;

void main()
{
    out_color = texture(source, ex_UV);
    if (out_color.a == 0)
        discard;

    vec2 screenPos = screenSize - ((gl_FragCoord.xy) / viewportSize) * screenSize;
    vec2 pos[0x10] = position;
    for (int i = 0; i < 0x10; ++i) {
        pos[i] /= 1 << 16;
        pos[i] /= viewportSize;
    }

    //if (type == 0)
    //    out_color = texture(source, ex_UV + pos[int(screenPos.y - starting)]);

    if (out_color.a == 0)
        discard;
    out_color = vec4(out_color.rgb, 1.0);
}