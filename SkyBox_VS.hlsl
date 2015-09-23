#pragma pack_matrix(row_major)



///

cbuffer MatrixBuffer : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}
//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

//


PixelInputType main(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;

	input.position.w = 1.0f;
	float4 localH = input.position;
		// move local space vertex from vertex buffer into world space.
		localH = mul(localH, worldMatrix);
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projectionMatrix);
	output.position = localH;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(float4(input.normal, 0), worldMatrix).xyz;
	//output.normal = input.normal;
	output.normal = normalize(output.normal);

	return output; // send projected vertex to the rasterizer stage
}
