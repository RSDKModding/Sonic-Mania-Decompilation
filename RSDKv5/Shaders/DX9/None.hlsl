// =======================
// VARIABLES
// =======================

SamplerState texDiffuse: register(s0);  // screen display texture

float2 pixelSize: register(c0);         // internal game resolution (usually 424x240 or smth)
float2 textureSize: register(c1);       // size of the internal framebuffer texture
float2 viewSize: register(c2);          // window viewport size

#if defined(RETRO_REV02)                // if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
float2 screenDim: register(c3);         // screen dimming percent
#endif


// =======================
// STRUCTS
// =======================

struct VertexInput
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float4 tex      : TEXCOORD;
};

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float4 tex      : TEXCOORD;
};

struct PixelInput
{
    float2 tex : TEXCOORD;
};

// =======================
// ENTRY POINTS
// =======================

VertexOutput VSMain(VertexInput input)
{
	VertexOutput output;
	
	output.pos      = input.pos;
	output.color    = input.color;
	output.tex      = input.tex;
	
	return output;
}

float4 PSMain(PixelInput input) : SV_TARGET
{
    float2 viewScale = frac((1.0 / pixelSize) * viewSize) - 0.01;

    // if viewSize is an integer scale of pixelSize (within a small margin of error)
    if (viewScale.x < 0 && viewScale.y < 0) {
        // just get the pixel at this fragment with no filtering
#if defined(RETRO_REV02) 
        return tex2D(texDiffuse, input.tex) * screenDim.x;
#else
        return tex2D(texDiffuse, input.tex);
#endif
    }

    // otherwise, it's not pixel perfect... do a bit of pixel filtering
    // we have to do it manually here since the engine samples this shader using the "point" filter, rather than "linear"

    float2 adjacent;
    adjacent.x = abs(ddx(input.tex.x));
    adjacent.y = abs(ddy(input.tex.y));

    float4 texPos;
    texPos.zw = adjacent.yx * 0.500501 + input.tex.yx;
    texPos.xy = -adjacent.xy * 0.500501 + input.tex.xy;

    float2 texSize  = 1.0 / textureSize.yx;
    float2 texCoord = clamp(texSize.xy * round(input.tex.yx / texSize.xy), texPos.yx, texPos.zw);
    
    float4 blendFactor;
    blendFactor.xy = -texPos.xy +  texCoord.yx;
    blendFactor.zw =  texPos.zw + -texCoord.xy;

    float strength = adjacent.x * adjacent.y * 0.500501 * 2.002;

    float4 filteredColor = 
        ((blendFactor.x * blendFactor.y) / strength) * tex2D(texDiffuse, texPos.xy) + 
        ((blendFactor.z * blendFactor.w) / strength) * tex2D(texDiffuse, texPos.wz) + 
        ((blendFactor.z * blendFactor.x) / strength) * tex2D(texDiffuse, texPos.xz) +
        ((blendFactor.w * blendFactor.y) / strength) * tex2D(texDiffuse, texPos.wy); 
    
#if defined(RETRO_REV02) 
    filteredColor *= screenDim.x;
#endif
    return filteredColor;
}