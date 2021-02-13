struct PSInputColor {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct PSInputTexture {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};
