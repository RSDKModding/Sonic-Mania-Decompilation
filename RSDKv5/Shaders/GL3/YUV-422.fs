// =======================
// VARIABLES
// =======================
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D texDiffuse; // screen display texture

uniform vec2 pixelSize;   // internal game resolution (usually 424x240 or smth)
uniform vec2 textureSize; // size of the internal framebuffer texture
uniform vec2 viewSize;    // window viewport size
#if defined(RETRO_REV02)  // if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
uniform float screenDim; // screen dimming percent
#endif


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