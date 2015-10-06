#pragma pack_matrix(row_major)


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 pixelPosition : TEXCOORD0;
	//float4 positionWP : WorldPos;
};


PixelInputType main(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;

	output.position = mul(float4(input.position.xyz,1), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.position.z = min(output.position.w, output.position.z);


	//output.positionWP = mul(float4(input.position.xyz, 1), worldMatrix);
	output.pixelPosition = output.position;

	return output;
}