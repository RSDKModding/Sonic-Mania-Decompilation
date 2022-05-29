// =======================
// VARIABLES
// =======================

cbuffer RSDKBuffer : register(b0)
{
	float2 pixelSize;		// internal game resolution (usually 424x240 or smth)
	float2 textureSize;		// size of the internal framebuffer texture
	float2 viewSize;		// window viewport size

#if defined(RETRO_REV02)	// if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
	float2 screenDim;		// screen dimming percent
#endif
};

Texture2D texDiffuse : register(t0);    	// screen display texture
SamplerState sampDiffuse : register(s0); 	// screen display sampler


// =======================
// STRUCTS
// =======================

struct VertexInput
{
    float3 pos      : SV_POSITION;
    float2 tex      : TEXCOORD;
};

struct VertexOutput
{
    float4 pos      : SV_POSITION;
    float4 tex      : TEXCOORD;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

// =======================
// ENTRY POINTS
// =======================

VertexOutput VSMain(VertexInput input)
{
	VertexOutput output;
	
	output.pos      = float4(input.pos.xyz, 1.0);
	output.tex      = float4(input.tex.xy, 0.0, 0.0);
	
	return output;
}

float4 PSMain(PixelInput input) : SV_TARGET
{
	// Adapted from https://github.com/rsn8887/Sharp-Bilinear-Shaders/releases, used in RetroArch 
	
    float2 texel = input.tex.xy * float4(textureSize, 1.0 / textureSize).xy;

    float2 texelFloored = floor(texel);
    float2 s            = frac(texel);
    float2 regionRange  = 0.5 - 0.5 / 2.0;

    float2 centerDist   = s - 0.5;
    float2 f            = (centerDist - clamp(centerDist, -regionRange, regionRange)) * 2.0 + 0.5;

    float2 modTexel = texelFloored + f;
	
    float4 outColor = texDiffuse.Sample(sampDiffuse, modTexel / textureSize.xy);
#if defined(RETRO_REV02) 
	outColor *= screenDim.x;
#endif
	return outColor;
}