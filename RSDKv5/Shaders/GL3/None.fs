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
    out_color = vec4(ex_UV, 0, 1);
    //return;
    vec2 viewScale;
    viewScale.x = fract((1.0 / pixelSize.x) * viewSize.x) - 0.01;
    viewScale.y = fract((1.0 / pixelSize.y) * viewSize.y) - 0.01;

    // if viewSize is an integer scale of pixelSize (within a small margin of error)
    if (viewScale.x < 0 && viewScale.y < 0) {
        // just get the pixel at this fragment with no filtering
#if defined(RETRO_REV02) || true
        out_color = texture2D(texDiffuse, ex_UV) * screenDim;
#else
        out_color = texture2D(texDiffuse, ex_UV);
#endif
        return;
    }

    // otherwise, it's not pixel perfect... do a bit of pixel filtering
    // we have to do it manually here since the engine samples this shader using the "point" filter, rather than "linear"

    vec2 adjacent;
    adjacent.x = abs(dFdx(ex_UV.x));
    adjacent.y = abs(dFdy(ex_UV.y));

    vec4 texPos;
    texPos.zw = adjacent.yx * vec2(0.500501) + ex_UV.yx;
    texPos.xy = -adjacent.xy * vec2(0.500501) + ex_UV.xy;

    vec2 texSize  = vec2(1.0) / textureSize.yx;
    vec2 texCoord = clamp(texSize.xy * round(ex_UV.yx / texSize.xy), texPos.yx, texPos.zw);
    
    vec4 blendFactor;
    blendFactor.xy = -texPos.xy +  texCoord.yx;
    blendFactor.zw =  texPos.zw + -texCoord.xy;

    float strength = adjacent.x * adjacent.y * 0.500501 * 2.002;

    vec4 blend;
    blend.x = (blendFactor.x * blendFactor.y) / strength;
    blend.y = (blendFactor.z * blendFactor.w) / strength;
    blend.z = (blendFactor.z * blendFactor.x) / strength;
    blend.w = (blendFactor.w * blendFactor.y) / strength;

    out_color = 
        blend.x * texture2D(texDiffuse, texPos.xy) + 
        blend.y * texture2D(texDiffuse, texPos.wz) + 
        blend.z * texture2D(texDiffuse, texPos.xz) +
        blend.w * texture2D(texDiffuse, texPos.wy); 
    
#if defined(RETRO_REV02) 
    out_color.rgb *= screenDim;
#endif
}