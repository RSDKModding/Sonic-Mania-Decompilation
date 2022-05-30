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
// DEFINITIONS
// =======================
#define RSDK_PI     3.14159                 // PI
#define viewSizeHD  720                     // how tall viewSize.y has to be before it simulates the dimming effect
#define intencity   float3(1.1, 0.9, 0.9)   // how much to "dim" the screen when simulating a CRT effect

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
    float2 viewPos      = floor((textureSize.xy / pixelSize.xy) * input.tex.xy * viewSize.xy) + 0.5;
    float intencityPos  = frac((viewPos.y * 3.0 + viewPos.x) * 0.166667);

    float4 scanlineIntencity;
    if (intencityPos < 0.333)
        scanlineIntencity.rgb = intencity.xyz;
    else if (intencityPos < 0.666)
        scanlineIntencity.rgb = intencity.zxy;
    else
        scanlineIntencity.rgb = intencity.yzx;

    float2 pixelPos         = input.tex.xy * textureSize.xy;
    float2 roundedPixelPos  = floor(pixelPos.xy);

    scanlineIntencity.a = clamp(abs(sin(pixelPos.y * RSDK_PI)) + 0.25, 0.5, 1.0);
    pixelPos.xy         = frac(pixelPos.xy) + -0.5;

    float2 invTexPos = -input.tex.xy * textureSize.xy + (roundedPixelPos + 0.5);
    
    float2 newTexPos;
    newTexPos.x = clamp(-abs(invTexPos.x * 0.5) + 1.5, 0.8, 1.25);
    newTexPos.y = clamp(-abs(invTexPos.y * 2.0) + 1.25, 0.5, 1.0);

    float2 colorMod;
    colorMod.x = newTexPos.x * newTexPos.y;
    colorMod.y = newTexPos.x * ((scanlineIntencity.a + newTexPos.y) * 0.5);

    scanlineIntencity.a *= newTexPos.x;

    float2 texPos   = ((pixelPos.xy + -clamp(pixelPos.xy, -0.25, 0.25)) * 2.0 + roundedPixelPos + 0.5) / textureSize.xy;
    float4 texColor = tex2D(texDiffuse, texPos.xy);

    float3 blendedColor;
    blendedColor.r  = scanlineIntencity.a * texColor.r;
    blendedColor.gb = colorMod.xy * texColor.gb;

    float4 outColor;
    outColor.rgb    = viewSize.y >= viewSizeHD ? (scanlineIntencity.rgb * blendedColor.rgb) : blendedColor.rgb;
    outColor.a      = texColor.a;

#if defined(RETRO_REV02) 
	outColor *= screenDim.x;
#endif
	return outColor;
}