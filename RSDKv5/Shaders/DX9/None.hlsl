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
	float4 temp;
	float4 pixelPerfectColor, pixelImperfectColor;
    
    temp.x = frac((1.0 / pixelSize.x) * viewSize.x);
    temp.y = frac((1.0 / pixelSize.y) * viewSize.y);

    temp    = temp + -0.01;
    temp.y  = cmp(temp.y, 0.0, 1.0).y;
    temp.x  = cmp(temp.x, 0.0, temp.y).x;

    if (temp.x != -temp.x) {
#if defined(RETRO_REV02) 
        return tex2D(texDiffuse, input.tex) * screenDim.x;
#else
        return tex2D(texDiffuse, input.tex);
#endif
    }

    float2 adjacent;
    adjacent.x = abs(ddx(input.tex.x));
    adjacent.y = abs(ddy(input.tex.y));

    float4 texPos;
    texPos.zw = adjacent.yx * float2(0.500501, 0.500501) + input.tex.yx;
    texPos.xy = -adjacent.xy * float2(0.500501, 0.500501) + input.tex.xy;

    float2 texSize  = float2(1.0, 1.0) / textureSize.yx;
    float2 texCoord = clamp(texSize.xy * round(input.tex.yx / texSize.xy), texPos.yx, texPos.zw);
    
    float4 blendFactor;
    blendFactor.xy = -texPos.xy +  texCoord.yx;
    blendFactor.zw =  texPos.zw + -texCoord.xy;

    float strength = adjacent.x * adjacent.y * 0.500501 * 2.002;

    float4 blend;
    blend.x = (blendFactor.x * blendFactor.y) / strength;
    blend.y = (blendFactor.z * blendFactor.w) / strength;
    blend.z = (blendFactor.z * blendFactor.x) / strength;
    blend.w = (blendFactor.w * blendFactor.y) / strength;

    float4 blendedColor = 
        blend.x * tex2D(texDiffuse, texPos.xy) + 
        blend.y * tex2D(texDiffuse, texPos.wz) + 
        blend.z * tex2D(texDiffuse, texPos.xz) +
        blend.w * tex2D(texDiffuse, texPos.wy); 
    
#if defined(RETRO_REV02) 
    blendedColor *= screenDim.x;
#endif
    return blendedColor;
}