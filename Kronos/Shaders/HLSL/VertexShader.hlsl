#include "Structures.hlsl"

PSInputColor VertexMainC(float4 position : POSITION, float4 color : COLOR) {
	PSInputColor output;
	output.position = position;
	output.color = color;
	return output;
}

PSInputTexture VertexMainT(float4 position : POSITION, float4 uv : TEXCOORD) {
	PSInputTexture result;

	result.position = position;
	result.uv = uv;

	return result;
}
