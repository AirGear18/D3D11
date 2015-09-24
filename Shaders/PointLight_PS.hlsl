/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypePoint : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 pixelPosition : TEXCOORD0;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

PixelOutputType main(PixelInputType input)
{
	PixelOutputType output;
	output.color = float4(1, 1, 1, 1);
	output.normal = float4(.5, .5, .5, 1);

	float2 ClipSpace, texCoord;

	ClipSpace = float2(input.pixelPosition.xy / input.pixelPosition.w);
	texCoord = float2(ClipSpace.x, -ClipSpace.y) *.5f + .5f;

	output.color = colorTexture.Sample(SampleTypePoint, texCoord);
	output.normal = normalTexture.Sample(SampleTypePoint, texCoord);
	return output;
}