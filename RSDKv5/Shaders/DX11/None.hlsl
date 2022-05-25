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
	float4 r0, r2, r3, r4, r5;
	float4 pixelPerfectColor, pixelImperfectColor;
    
    r0.x = frac((1.0 / pixelSize.x) * viewSize.x);
    r0.y = frac((1.0 / pixelSize.y) * viewSize.y);

    r0.xy = (r0 + -0.00999999978).xy;
    r0.y = cmp(r0.y, 0.0, 1.0).y;
    r0.x = cmp(r0.x, 0.0, r0.y).x;

    if (r0.x != -r0.x) {
#if defined(RETRO_REV02) 
        pixelPerfectColor = texDiffuse.Sample(sampDiffuse, input.tex.xy) * screenDim.x;
#else
        pixelPerfectColor = texDiffuse.Sample(sampDiffuse, input.tex.xy);
#endif
    }
    else {
        pixelPerfectColor = 0.0;
    }

    r0.w = abs(ddx(input.tex.x));
    r0.z = abs(ddy(input.tex.y));
    r0.y = (r0.w * (r0.z * 0.5005005)) * 2.002002;

    r2.zw = mad(r0, 0.5005005, input.tex.xyyx).zw;
    r2.xy = mad(r0.wzzw, -0.5005005, input.tex.xyxy).xy;
    r3.y = 1.0 / textureSize.x;
    r3.x = 1.0 / textureSize.y;

    r0.zw = mad(input.tex.xyyx, textureSize.xyyx, 0.5).zw;
    r3.zw = frac(r0).zw;
    r0.zw = (r0 + -r3).zw;
    r0.zw = (r3.xyxy * r0).zw;
    r3.xy = max(r0.zwzw, r2.yxzw).xy;
    r0.zw = min(r2, r3.xyxy).zw;

    r3.xy = (-r2 + r0.wzzw).xy;
    r0.zw = (-r0 + r2).zw;
    r3.zw = (r0 * r3.xyxy).zw;
    r0.y = 1.0 / r0.y;
    r3.zw = (r0.y * r3).zw;

    r4 = texDiffuse.Sample(sampDiffuse, r2.xz);
    r0.z = r0.z * r0.w;
    r0.z = r0.z * r0.y;

    r5 = texDiffuse.Sample(sampDiffuse, r2.wz);
    r5 = r0.z * r5;
    r4 = mad(r3.z, r4, r5);

    r0.z = r3.y * r3.x;
    r0.y = r0.y * r0.z;
    r5 = texDiffuse.Sample(sampDiffuse, r2.xy);

    r4 = mad(r0.y, r5, r4);

    r2 = texDiffuse.Sample(sampDiffuse, r2.wy);
    
    pixelImperfectColor = mad(r3.w, r2, r4);
#if defined(RETRO_REV02) 
    pixelImperfectColor = pixelImperfectColor * screenDim.x;
#endif
    
    return cmp(-r0.x, pixelImperfectColor, pixelPerfectColor);
}