#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec2 screenSize;
uniform vec2 viewportSize;

uniform sampler2DRect tiles;

uniform sampler2DRect palette;
uniform int paletteLines[0x100];

void main()
{
    ivec3 placed  = ivec3(texture(tiles, ex_UV).rgb * 255);
    int index = (placed.b >> 3) | ((placed.g >> 2) << 5) | ((placed.r >> 3) << 11);
    if (index == 0)
        discard;
    vec2 screenPos = ((gl_FragCoord.xy) / viewportSize) * screenSize;
    screenPos.y    = screenSize.y - screenPos.y;

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

        palvec.y  = id1;
        vec3 c1   = texture(palette, palvec).rgb;
        palvec.y  = id2;
        vec3 c2   = texture(palette, palvec).rgb;
        out_color = vec4(mix(c2, c1, fract(screenPos.y)), 1.0);
    }
}