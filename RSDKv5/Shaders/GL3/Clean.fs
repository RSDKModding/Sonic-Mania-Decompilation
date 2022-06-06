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
    vec2 texel = ex_UV * vec4(textureSize, 1.0 / textureSize).xy;
    vec2 scale = vec2(2);

    vec2 texel_floored = floor(texel);
    vec2 s             = fract(texel);
    vec2 region_range  = 0.5 - 0.5 / scale;

    // Figure out where in the texel to sample to get correct pre-scaled bilinear.
    // Uses the hardware bilinear interpolator to avoid having to sample 4 times manually.

    vec2 center_dist = s - 0.5;
    vec2 f           = (center_dist - clamp(center_dist, -region_range, region_range)) * scale + 0.5;

    vec2 mod_texel = texel_floored + f;

    out_color = texture2D(texDiffuse, (mod_texel / textureSize.xy));
#if defined(RETRO_REV02)
    out_color.rgb *= screenDim;
#endif
}