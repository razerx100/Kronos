#include "Structures.hlsl"

PSInput VertexMain(float4 position : POSITION, float4 color : COLOR) {
	PSInput output;
	output.position = position;
	output.color = color;
	return output;
}