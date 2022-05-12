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
	float4 r0;
    r0.xy = textureSize * input.tex;
    float4 r1 = frac(r0.xyxy);
    
	r0.xy = (r0 + -r1.zwzw).xy;
    r1 += float2(-0.5, -0.25).xxyy;
	
	r0.zw = cmp(r1, r1.xyxy, -0.25).zw;
	
    r1.zw = max(-0.25, -r0).zw;
    r0.zw = (r1 + r1.xyxy).zw;
    r0.xy = ((r0.zwzw * 2.0) + r0).xy;
    r0.xy = (r0 + 0.5).xy;
	
	r1.x = 1.0 / textureSize.x;
	r1.y = 1.0 / textureSize.y;
	
#if defined(RETRO_REV02) 
	return tex2D(texDiffuse, (r0 * r1).xy) * screenDim.x;
#else
	return tex2D(texDiffuse, (r0 * r1).xy);
#endif
}