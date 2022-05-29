#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D texDiffuse;

uniform float screenDim;
uniform vec2 pixelSize;
uniform vec2 textureSize;
uniform vec2 viewSize;

void main()
{
    out_color = texture(texDiffuse, ex_UV);
#if defined(RETRO_REV02) 
    out_color.rgb *= screenDim;
#endif
}