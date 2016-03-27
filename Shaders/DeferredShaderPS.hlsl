
//////////////
// TEXTURES //
//////////////
Texture2D shaderTexture : register(t0);


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypeWrap : register(s0);


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float2 depthDiv : TEXCOORD1;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 Depth : SV_Target2;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PixelOutputType main(PixelInputType input)
{
	PixelOutputType output;
	output.color = float4(1, 1, 1, 1);
	output.normal = float4(.5, .5, .5, 1);

	// Sample the color from the texture and store it for output to the render target.
	output.color = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// Store the normal for output to the render target.
	output.normal = float4((input.normal + 1)*.5f,1.0);
	output.Depth = input.depthDiv.x / input.depthDiv.y;
	output.Depth.w = 1.0f;
	return output;
}