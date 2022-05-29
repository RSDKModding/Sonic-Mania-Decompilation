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
	// Adapted from https://github.com/rsn8887/Sharp-Bilinear-Shaders/releases, used in RetroArch 
	
    float2 texel = input.tex.xy * float4(textureSize, 1.0 / textureSize).xy;

    float2 texelFloored = floor(texel);
    float2 s            = frac(texel);
    float2 regionRange  = 0.5 - 0.5 / 2.0;

    float2 centerDist   = s - 0.5;
    float2 f            = (centerDist - clamp(centerDist, -regionRange, regionRange)) * 2.0 + 0.5;

    float2 modTexel = texelFloored + f;
	
    float4 outColor = tex2D(texDiffuse, modTexel / textureSize.xy);
#if defined(RETRO_REV02) 
	outColor *= screenDim.x;
#endif
	return outColor;
}