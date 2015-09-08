
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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
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
	output.normal = float4((input.normal + 1)*.5f,0.0);

	

	return output;
}