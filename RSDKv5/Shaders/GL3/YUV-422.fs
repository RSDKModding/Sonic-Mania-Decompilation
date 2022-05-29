#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D texDiffuse;

uniform float screenDim;
uniform vec2 pixelSize;
uniform vec2 textureSize;
uniform vec2 viewSize;

void main()
{
    vec3 yuv;
    yuv.r  = texture(texDiffuse, ex_UV).r;
    yuv.gb = texture(texDiffuse, vec2(clamp(ex_UV.x / 2.0, 0, 0.499), ex_UV.y)).gb;
    yuv -= vec3(16.0 / 256, .5, .5);

    out_color.r = 1.164 * yuv.r + 1.596 * yuv.b;
    out_color.g = 1.164 * yuv.r - 0.392 * yuv.g - 0.813 * yuv.b;
    out_color.b = 1.164 * yuv.r + 2.017 * yuv.g;
#if defined(RETRO_REV02) 
    out_color.rgb *= screenDim;
#endif
}