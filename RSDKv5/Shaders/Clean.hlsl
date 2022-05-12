// =======================
// VARIABLES
// =======================

float2 pixelSize: register(c0);         // internal game resolution (usually 424x240 or smth)
#if defined(RETRO_REV02)                // if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
float2 screenDim: register(c3);         // screen dimming percent
#endif
SamplerState texDiffuse: register(s0);  // screen display texture
float2 textureSize: register(c1);       // size of the internal framebuffer texture
float2 viewSize: register(c2);          // window viewport size


// =======================
// STRUCTS
// =======================

struct VertexInput
{
    float4 pos      : POSITION0;
    float4 color    : COLOR0;
    float4 tex      : TEXCOORD0;
};

struct VertexOutput
{
    float4 pos      : POSITION0;
    float4 color    : COLOR0;
    float4 tex      : TEXCOORD0;
};

struct PixelInput
{
    float2 tex : TEXCOORD0;
};


// =======================
// FUNCTIONS
// =======================

float4 cmp(float4 src0, float4 src1, float4 src2) {
	return float4(
		src0.x >= 0 ? src1.x : src2.x,
		src0.y >= 0 ? src1.y : src2.y,
		src0.z >= 0 ? src1.z : src2.z,
		src0.w >= 0 ? src1.w : src2.w
	);
}

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
    float2 scale = float2(2, 2);

    float2 texel_floored = floor(texel);
    float2 s             = frac(texel);
    float2 region_range  = 0.5 - 0.5 / scale;

    float2 center_dist = s - 0.5;
    float2 f           = (center_dist - clamp(center_dist, -region_range, region_range)) * scale + 0.5;

    float2 mod_texel = texel_floored + f;
	
#if defined(RETRO_REV02) 
	return tex2D(texDiffuse, (mod_texel / textureSize.xy)) * screenDim.x;
#else
	return tex2D(texDiffuse, (mod_texel / textureSize.xy));
#endif
}