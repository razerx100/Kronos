#include "Structures.hlsl"

float4 PixelMainC(PSInputColor input) : SV_TARGET {
	return input.color;
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 PixelMainT(PSInputTexture input) : SV_TARGET{
	return g_texture.Sample(g_sampler, input.uv);
}