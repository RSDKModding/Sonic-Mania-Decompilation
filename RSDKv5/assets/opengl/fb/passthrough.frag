#version 330 core
in vec2 ex_UV;
out vec4 out_color;

uniform int inkEffect;
uniform int maskColor;
uniform sampler2DRect lookupTable;
uniform int alpha;

uniform sampler2D source;
uniform sampler2D dest;

const int INK_NONE   = 0;
const int INK_BLEND  = 1;
const int INK_ALPHA  = 2;
const int INK_ADD    = 3;
const int INK_SUB    = 4;
const int INK_LOOKUP = 5;
const int INK_MASKED = 6;
const int INK_UNMASK = 7;

void main()
{
    out_color  = texture(source, ex_UV);
    float alph = alpha / 255.0;
    if (out_color.a == 0 || (alph == 0 && inkEffect < INK_LOOKUP && inkEffect != INK_NONE))
        discard;
    ivec3 placed  = ivec3(texture(dest, ex_UV).rgb * 255);
    int truncated = (placed.b >> 3) | ((placed.g >> 2) << 5) | ((placed.r >> 3) << 11);

    switch (inkEffect) {
        case INK_NONE: out_color = vec4(out_color.rgb, 1.0); break;
        case INK_BLEND: alph = .5;
        case INK_ALPHA:
        case INK_ADD: out_color = vec4(out_color.rgb, alph); break;
        case INK_SUB: out_color = vec4((out_color - texture(dest, ex_UV) * (1 - alph)).rgb, 1.0); break;
        case INK_LOOKUP: {
            ivec2 lookupUV = ivec2(truncated % 256, truncated / 256);
            out_color      = vec4(texture(lookupTable, lookupUV).rgb, 1.0);
            break;
        }
        case INK_MASKED:
            if (truncated != maskColor)
                discard;
            out_color = vec4(out_color.rgb, 1.0);
            break;
        case INK_UNMASK:
            if (truncated == maskColor)
                discard;
            out_color = vec4(out_color.rgb, 1.0);
            break;
    }
}