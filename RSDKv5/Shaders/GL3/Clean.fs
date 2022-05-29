#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D texDiffuse;

uniform float screenDim;
uniform vec2 pixelSize;
uniform vec2 textureSize;
uniform vec2 viewSize;

vec4 cmp(vec4 src0, vec4 src1, vec4 src2)
{
    return vec4(src0.x >= 0 ? src1.x : src2.x, src0.y >= 0 ? src1.y : src2.y, src0.z >= 0 ? src1.z : src2.z, src0.w >= 0 ? src1.w : src2.w);
}

void main()
{
    vec2 texel = ex_UV * vec4(textureSize, 1.0 / textureSize).xy;
    vec2 scale = vec2(2);

    vec2 texel_floored = floor(texel);
    vec2 s             = fract(texel);
    vec2 region_range  = 0.5 - 0.5 / scale;

    // Figure out where in the texel to sample to get correct pre-scaled bilinear.
    // Uses the hardware bilinear interpolator to avoid having to sample 4 times manually.

    vec2 center_dist = s - 0.5;
    vec2 f           = (center_dist - clamp(center_dist, -region_range, region_range)) * scale + 0.5;

    vec2 mod_texel = texel_floored + f;

    out_color = texture2D(texDiffuse, (mod_texel / textureSize.xy)) * screenDim;
}