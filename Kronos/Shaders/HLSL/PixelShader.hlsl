#include "Structures.hlsl"

float4 PixelMain(PSInput input) : SV_TARGET {
	return input.color;
}