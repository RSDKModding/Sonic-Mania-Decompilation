#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec2 screenSize;
uniform vec2 viewportSize;

uniform sampler2D sprite;

uniform sampler2DRect palette;
uniform int paletteLines[0x100];
uniform bool useColor;

void main()
{
    vec2 screenPos = ((gl_FragCoord.xy) / viewportSize) * screenSize;
    screenPos.y = screenSize.y - screenPos.y;
    if (useColor) out_color = vec4(ex_color.bgr, 1.0);
    else {
        int index = int(texture(sprite, ex_UV).r * 255);
        if (index == 0)
            discard;

        int id1 = paletteLines[int(ceil(screenPos.y))];
        int id2 = paletteLines[int(floor(screenPos.y))];

        if (id1 == id2) {
            vec2 palvec;
            palvec.x = index;
            palvec.y = id1;

            out_color = vec4(texture(palette, palvec).rgb, 1.0);
        } 
        else {
            vec2 palvec;
            palvec.x = index;

            palvec.y = id1;
            vec3 c1 = texture(palette, palvec).rgb;
            palvec.y = id2;
            vec3 c2 = texture(palette, palvec).rgb;
            out_color = vec4(mix(c2, c1, fract(screenPos.y)), 1.0);
        }

    }
}