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
	
	output.pos      = float4(input.pos.xyz, 1.0);
	output.tex      = float4(input.tex.xy, 0.0, 0.0);
	
	return output;
}

float4 PSMain(PixelInput input) : SV_TARGET
{
    float4 c8 = float4(0.899999976, 1.10000002, 0, 0);

	float4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;

    r0.x = 1.0 / pixelSize.x;
    r0.y = 1.0 / pixelSize.y;
    r0.xy = (r0 * textureSize.xyxy).xy;
    r0.xy = (r0 * input.tex.xyxy).xy;
    r0.xy = (r0 * viewSize.xyxy).xy;
    r0.zw = frac(r0.xyxy).zw;
    r0.xy = (-r0.zwzw + r0).xy;
    r0.xy = (r0 + 0.5).xy;
    r0.x = r0.y * 3.0 + r0.x;
    r0.x = r0.x * 0.166666672;
    r0.x = frac(r0.x);
    r0.xy = (r0.x + float3(-0.333000004, -0.666000009, 0.899999976).xyyz).xy;
    r1.yz = cmp(r0.y, c8.xxyw, c8.xyxw).yz;
    r1.x = 0.899999976;
    r0.xyz = cmp(r0.x, r1, c8.yxxw).xyz;
    r1.xy = textureSize * input.tex;
    r0.w = r1.y * 0.5 + 0.5;
    r0.w = frac(r0.w);
    r0.w = r0.w * 6.28318548 + -3.14159274;
    r2.y = sin(r0.w);
    r1.zw = (abs(r2).y + float4(6.28318548, -3.14159274, 0.25, -0.25)).zw;
	r1.z = saturate(r1.z);
    r0.w = cmp(r1.w, r1.z, 0.5).w;
    r2 = frac(r1.xyxy);
    r1.xy = (r1 + -r2.zwzw).xy;
    r2 = r2 + float2(-0.5, -0.25).xxyy;
    r1.zw = (r1.xyxy + 0.5).zw;
    r1.zw = (input.tex.xyxy * -textureSize.xyxy + r1).zw;
    r1.w = r1.w + r1.w;
    r1.z = r1.z * 0.5;
    r1.z = -abs(r1).z + 1.5;
    r3.x = max(0.800000012, r1.z);
    r4.x = min(r3.x, 1.25);
    r1.zw = (-abs(r1).w + float4(1.5, 0.800000012, 1.25, 0.75)).zw;
	r1.z = saturate(r1.z);
    r1.z = cmp(r1.w, r1.z, 0.5).z;
    r4.y = r0.w + r1.z;
    r0.w = r0.w * r4.x;
    r1.z = r1.z * r4.x;
    r3.xy = (r4 * float4(1, 0.5, 720, 3)).xy;
    r1.w = r3.y * r3.x;
    r2.zw = cmp(r2, r2.xyxy, -0.25).zw;
    r3.xy = max(-0.25, -r2.zwzw).xy;
    r2.xy = (r2 + r3).xy;
    r1.xy = (r2 * 2.0 + r1).xy;
    r1.xy = (r1 + 0.5).xy;
    r2.x = 1.0 / textureSize.x;
    r2.y = 1.0 / textureSize.y;
    r1.xy = (r1 * r2).xy;
	
    float4 outColor = texDiffuse.Sample(sampDiffuse, r1.xy);
    r3.x = r0.w * outColor.r;
    r3.yz = (r1.xzww * outColor).gb;
    r0.xyz = (r0 * r3).xyz;
    r1.z = 720.0;
    r0.w = r1.z + -viewSize.y;

	outColor.rgb = cmp(r0.w, r3, r0).xyz;
#if defined(RETRO_REV02) 
	return outColor * screenDim.x;
#else
	return outColor;
#endif
}