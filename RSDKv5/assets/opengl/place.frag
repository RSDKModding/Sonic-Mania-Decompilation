#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec3 maskColor;
uniform int maskMode;
uniform float transparency;
uniform sampler2DRect sprite;

uniform sampler2DRect palette;
uniform int paletteLines[0x100];
uniform bool useColor;

struct ScreenInfo {
    vec2 size;
    vec2 clipRectTL;
    vec2 clipRectBR;
};

uniform ScreenInfo screen;

void main()
{
    //out_color = vec4(gl_FragCoord.xy / 480.0, 1.0, 1.0);
    if (useColor) out_color = vec4(ex_color.b, ex_color.g, ex_color.r, ex_color.a);
    else {
        vec2 screenPos = gl_FragCoord.xy;

        //int id = paletteLines[int(screenPos.y)];
        int id = 0;
        int index = int(texture(sprite, ex_UV).r * 255);

        if (index == 0)
            discard;

        vec2 palvec;
        palvec.x = index;
        palvec.y = id;

        out_color = vec4(texture(palette, palvec).xyz, transparency);
    }
    if (maskMode != 0) out_color = vec4(out_color.xyz - maskColor, 1.0);//*/
    //if (maskMode == 1 && out_color.xyz != maskColor) discard;
    //else if (maskMode == 2 && out_color.xyz == maskColor) discard;
}