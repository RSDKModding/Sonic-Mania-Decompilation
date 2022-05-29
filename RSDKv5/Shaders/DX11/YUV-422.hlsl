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
    float4 outColor;

    float3 yuv;
    yuv.r  = texDiffuse.Sample(sampDiffuse, input.tex.xy).r;
    yuv.gb = texDiffuse.Sample(sampDiffuse, float2(clamp(input.tex.x / 2.0, 0, 0.499), input.tex.y)).gb;
    yuv -= float3(16.0 / 256, 0.5, 0.5);

    outColor.r = 1.164 * yuv.r + 1.596 * yuv.b;
    outColor.g = 1.164 * yuv.r - 0.392 * yuv.g - 0.813 * yuv.b;
    outColor.b = 1.164 * yuv.r + 2.017 * yuv.g;
    outColor.a = 1.0;

#if defined(RETRO_REV02) 
	outColor *= screenDim.x;
#endif
	return outColor;
}