// =======================
// VARIABLES
// =======================
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D texDiffuse; // screen display texture

uniform vec2 pixelSize;   // internal game resolution (usually 424x240 or smth)
uniform vec2 textureSize; // size of the internal framebuffer texture
uniform vec2 viewSize;    // window viewport size
#if defined(RETRO_REV02)  // if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
uniform float screenDim; // screen dimming percent
#endif


// =======================
// DEFINITIONS
// =======================
#define RSDK_PI     3.14159                 // PI
#define viewSizeHD  720                     // how tall viewSize.y has to be before it simulates the dimming effect
#define intencity   vec3(1.1, 0.9, 0.9)   // how much to "dim" the screen when simulating a CRT effect

void main()
{
    vec2 viewPos      = floor((textureSize.xy / pixelSize.xy) * ex_UV.xy * viewSize.xy) + vec2(0.5);
    float intencityPos  = fract((viewPos.y * 3.0 + viewPos.x) * 0.166667);

    vec4 scanlineIntencity;
    if (intencityPos < 0.333)
        scanlineIntencity.rgb = intencity.xyz;
    else if (intencityPos < 0.666)
        scanlineIntencity.rgb = intencity.zxy;
    else
        scanlineIntencity.rgb = intencity.yzx;

    vec2 pixelPos         = ex_UV.xy * textureSize.xy;
    vec2 roundedPixelPos  = floor(pixelPos.xy);

    scanlineIntencity.a = clamp(abs(sin(pixelPos.y * RSDK_PI)) + 0.25, 0.5, 1.0);
    pixelPos.xy         = fract(pixelPos.xy) + vec2(-0.5, -0.5);

    vec2 invTexPos = -ex_UV.xy * textureSize.xy + (roundedPixelPos + vec2(0.5, 0.5));
    
    vec2 newTexPos;
    newTexPos.x = clamp(-abs(invTexPos.x * 0.5) + 1.5, 0.8, 1.25);
    newTexPos.y = clamp(-abs(invTexPos.y * 2.0) + 1.25, 0.5, 1.0);

    vec2 colorMod;
    colorMod.x = newTexPos.x * newTexPos.y;
    colorMod.y = newTexPos.x * ((scanlineIntencity.a + newTexPos.y) * 0.5);

    scanlineIntencity.a *= newTexPos.x;

    vec2 texPos   = ((pixelPos.xy + -clamp(pixelPos.xy, vec2(-0.25, -0.25), vec2(0.25, 0.25))) * 2.0 + roundedPixelPos + 0.5) / textureSize.xy;
    vec4 texColor = texture2D(texDiffuse, texPos.xy);

    vec3 blendedColor;
    blendedColor.r  = scanlineIntencity.a * texColor.r;
    blendedColor.gb = colorMod.xy * texColor.gb;

    out_color.rgb    = viewSize.y >= viewSizeHD ? (scanlineIntencity.rgb * blendedColor.rgb) : blendedColor.rgb;

#if defined(RETRO_REV02) 
	out_color.rgb *= screenDim;
#endif
}