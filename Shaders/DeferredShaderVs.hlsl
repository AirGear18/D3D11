#pragma pack_matrix(row_major)


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
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float2 depthDiv : TEXCOORD1;
};


PixelInputType main(VertexInputType input)
{
	//V_OUT sendToRasterizer = (V_OUT)0;
	//sendToRasterizer.colorOut = colorOut0;
	PixelInputType output = (PixelInputType)0;
	// ensures translation is preserved during matrix multiply  
	input.position.w = 1.0f;
	float4 localH = input.position;
		// move local space vertex from vertex buffer into world space.
		localH = mul(localH, worldMatrix);
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projectionMatrix);
	output.position = localH;
	output.depthDiv = output.position.zw;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;


	// Calculate the normal vector against the world matrix only.
	output.normal = mul(float4(input.normal,0),worldMatrix).xyz;
	//output.normal = input.normal;
	output.normal = normalize(output.normal);


	//float4 surfacenormal = float4(input.Norm, 0);
	//	surfacenormal = mul(surfacenormal, worldMatrix);
	//float4 lightratio =(dot(LightDir, surfacenormal));

	/*
	LIGHTDIR = NORMALIZE( LIGHTPOS – SURFACEPOS )
	LIGHTRATIO = CLAMP( DOT( LIGHTDIR, SURFACENORMAL ) )
	RESULT = LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR


	*/

	return output; // send projected vertex to the rasterizer stage
}