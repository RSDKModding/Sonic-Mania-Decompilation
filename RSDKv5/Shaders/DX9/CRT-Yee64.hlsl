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
    float3 c5 = float3(0.5, -0.5, 1.5);
    float4 c8 = float4(-0.333000004, -0.666000009, 0.899999976, 1.20000005);

	float4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;
	float4 texSize = textureSize.xyyy;

	r0.z = 0.0;
    r1.x = 1.0 / pixelSize.x;
    r1.y = 1.0 / pixelSize.y;
    r1.xy = (r1 * texSize).xy;
    r1.xy = (r1 * input.tex.xyyy).xy;
    r2.x = 1.0 / texSize.x;
    r2.y = 1.0 / texSize.y;
    r1.zw = (r2.xyxy * pixelSize.xyxy).zw;
    r1.zw = (r1 * r1.xyxy).zw;
    r1.xy = (r1 * viewSize.xyxy).xy;
    r2.zw = (r1 * texSize.xyxy).zw;
    r2.zw = frac(r2).zw;
    r0.xy = (-r2.zwzw).xy;
    r3.xy = (r1.zwzw * texSize + r0.xzzw).xy;
    r4.yz = (r1.xzww * texSize.xxyw + r0.xzyw).yz;
    r3.z = r0.y + r3.y;
    r5 = r3.xzxz + -c5.zyxy;
    r3 = r3.xzxz + float3(1.5, 0.5, 2.5).xyzy;
    r3 = r2.xyxy * r3;
    r5 = r2.xyxy * r5;

    r6 = tex2D(texDiffuse, r5.zw);
    r5 = tex2D(texDiffuse, r5.xy);
    r5.xyz = (r5 * 1.25).xyz;
    r7 = r1.zwzw * texSize.xyxy + r0.xyxy;
    r0.zw = (r1 * texSize.xyxy + -r7).zw;
    r8.x = -1.0;
    r1.zw = (r1 * texSize.xyxy + r8.x).zw;
    r1.zw = (r0.xyxy + r1).zw;
    r4.x = r0.x + r4.y;
    r4 = r4.xzxz + c5.xyxz;
    r4 = r2.xyxy * r4;
    r0.xy = (r1.zwzw + 0.5).xy;
    r0.xy = (r2 * r0).xy;

    r8 = tex2D(texDiffuse, r0.xy);
    r8.xyz = (r8 * 1.25).xyz;
    r0.xy = (r0.zwzw + -0.5).xy;
    r9 = -r0.x + float4(-1.0, 1.0, -2.0, 2.0);
    r9 = r9 * r9;
    r9 = r9 * -3.0;
    r0.z = pow(2, r9.x);
    r6.xyz = (r6 * r0.z).xyz;
    r6.xyz = (r6 * 1.25).xyz;
    r0.w = pow(2, r9.z);
    r5.xyz = (r5 * r0.w + r6).xyz;
    r0.w = r0.z + r0.w;
    r6 = r7.zwzw + c5.zyxx;
    r7 = r7 + c5.yzzz;
    r7 = r2.xyxy * r7;
    r2 = r2.xyxy * r6;

    r6 = tex2D(texDiffuse, r2.zw);
    r2 = tex2D(texDiffuse, r2.xy);
    r2.xyz = (r2 * 1.25).xyz;
    r1.zw = (r0.xyxy * r0.xyxy).zw;
    r0.xy = (-r0.y + float4(-1.0, 1.0, -2.0, 2.0)).xy;
    r0.xy = (r0 * r0).xy;
    r0.xy = (r0 * -8.0).xy;
    r1.zw = (r1 * float2(-3, -8).xyxy).zw;
    r1.z = pow(2, r1.z);
    r1.w = pow(2, r1.w);
    r6.xyz = (r6 * r1.z).xyz;
    r5.xyz = (r6 * 1.25 + r5).xyz;

    r6 = tex2D(texDiffuse, r3.xy);
    r3 = tex2D(texDiffuse, r3.zw);
    r3.xyz = (r3 * 1.25).xyz;
    r2.w = pow(2, r9.y);
    r3.w = pow(2, r9.w);
    r6.xyz = (r6 * r2.w).xyz;
    r5.xyz = (r6 * 1.25 + r5).xyz;
    r3.xyz = (r3 * r3.w + r5).xyz;
    r0.w = r0.w + r1.z;
    r0.w = r2.w + r0.w;
    r0.w = r3.w + r0.w;
    r0.w = 1.0 / r0.w;
    r3.xyz = (r0.w * r3).xyz;
    r3.xyz = (r1.w * r3).xyz;

    r5 = tex2D(texDiffuse, r4.xy);
    r4 = tex2D(texDiffuse, r4.zw);
    r4.xyz = (r1.z * r4).xyz;
    r4.xyz = (r4 * 1.25).xyz;
    r5.xyz = (r5 * 1.25).xyz;
    r5.xyz = (r1.z * r5).xyz;
    r0.w = r0.z + r1.z;
    r0.w = r2.w + r0.w;
    r0.w = 1.0 / r0.w;
    r5.xyz = (r8 * r0.z + r5).xyz;
    r2.xyz = (r2 * r2.w + r5).xyz;
    r2.xyz = (r0.w * r2).xyz;
    r0.x = pow(2, r0.x);
    r0.y = pow(2, r0.y);
    r2.xyz = (r2 * r0.x + r3).xyz;
	
    r3 = tex2D(texDiffuse, r7.xy);
    r5 = tex2D(texDiffuse, r7.zw);
    r5.xyz = (r2.w * r5).xyz;
    r3.xyz = (r0.z * r3).xyz;
    r3.xyz = (r3 * 1.25 + r4).xyz;
    r3.xyz = (r5 * 1.25 + r3).xyz;
    r0.xzw = (r0.w * r3.xyyz).xzw;
    r0.xyz = (r0.xzww * r0.y + r2).xyz;
    r1.zw = frac(r1.xyxy).zw;
    r1.xy = (-r1.zwzw + r1).xy;
    r1.xy = (r1 + 0.5).xy;
    r0.w = r1.y * 3.0 + r1.x;
    r0.w = r0.w * 0.166666672;
    r0.w = frac(r0.w);
    r1.xy = (r0.w + c8).xy;
    r2.yz = cmp(r1.y, c8.xzww, c8.xwzw).yz;
    r2.x = 0.899999976;
    r1.xyz = cmp(r1.x, r2, c8.wzzw).xyz;
    r1.xyz = (r0 * r1).xyz;
    r2.z = 720;
    r0.w = r2.z + -viewSize.y;

	float4 outColor;
    outColor.rgb = cmp(r0.w, r0, r1).rgb;
    outColor.a = 1.0;
	
#if defined(RETRO_REV02) 
	return outColor * screenDim.x;
#else
	return outColor;
#endif
}