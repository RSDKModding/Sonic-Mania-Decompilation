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
    float4 c8 = float4(0.899999976, 1.10000002, 0, 0);

	float4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;
	float4 outColor;

	float4 texPos = input.tex.xyyy;
	
	r0.x = 1.0 / pixelSize.x;
	r0.y = 1.0 / pixelSize.y;
	r0.xy = (r0 * textureSize.xyxy).xy;
	r0.xy = (r0 * texPos).xy;
	r0.xy = (r0 * viewSize.xyxy).xy;
	r0.zw = frac(r0.xyxy).zw;
	r0.xy = (-r0.zwzw + r0).xy;
	r0.xy = (r0 + 0.5).xy;
	r0.x = r0.y * 3.0 + r0.x;
	r0.x = r0.x * 0.166666672;
	r0.x = frac(r0.x);
	r0.xy = (r0.xxxx + float2(-0.666000009, 0.899999976).yxxy).xy;
	r1.yz = (r0.y >= 0 ? c8.xxyw : c8.xyxw).yz;
	r1.x = 0.899999976;
	r0.xyz = (r0.x >= 0 ? r1 : c8.yxxw).xyz;
	r1.xy = (textureSize.xyxy * texPos).xy;
	r0.w = r1.y * 0.5 + 0.5;
	r0.w = frac(r0.w);
	r0.w = r0.w * 6.28318548 + -3.14159274;
	r2.y = sin(r0.w);
	r1.zw = (abs(r2).yyyy + float4(6.28318548, -3.14159274, 0.25, -0.25)).zw;
	r1.z = saturate(r1.z);
	r0.w = r1.w >= 0 ? r1.z : 0.5;
	r2 = frac(r1.xyxy);
	r1.xy = (r1 + -r2.zwzw).xy;
	r2 = r2 + float2(-0.5, -0.25).xxyy;
	r1.zw = (r1.xyxy + 0.5).zw;
	r1.zw = (texPos.xyxy * -textureSize.xyxy + r1).zw;
	r1.w = r1.w + r1.w;
	r1.z = r1.z * 0.5;
	r1.z = -abs(r1).z + 1.5;
	r3.x = max(0.800000012, r1.z);
	r4.x = min(r3.x, 1.25);
	r1.zw = (-abs(r1).wwww + float4(1.5, 0.800000012, 1.25, 0.75)).zw;
	r1.z = saturate(r1.z);
	r1.z = r1.w >= 0 ? r1.z : 0.5;
	r4.y = r0.w + r1.z;
	r0.w = r0.w * r4.x;
	r1.z = r1.z * r4.x;
	r3.xy = (r4 * float4(1, 0.5, 720, 3)).xy;
	r1.w = r3.y * r3.x;
    r2.z = cmp(r2, r2.xyxy, -0.25).z;
    r3.xy = max(-0.25, -r2.zwzw).xy;
    r2.xy = (r2 + r3).xy;
    r1.xy = (r2 * 2.0 + r1).xy;
    r1.xy = (r1 + 0.5).xy;
    r2.x = 1.0 / textureSize.x;
    r2.y = 1.0 / textureSize.y;
    r1.xy = (r1 * r2).xy;
    r2 = tex2D(texDiffuse, r1.xy);
    r3.x = r0.w * r2.x;
    r3.yz = (r1.xzww * r2).yz;
    r0.xyz = (r0 * r3).xyz;
    r0.w = 720.0 + -viewSize.y;
	
    outColor.a = r2.w;
#if defined(RETRO_REV02) 
	outColor.rgb = (r0.w >= 0 ? r3 : r0).rgb * screenDim.x;
#else
	outColor.rgb = (r0.w >= 0 ? r3 : r0).rgb;
#endif
	return outColor;
}