
//TextureCube gCubeMap : register(t1);

SamplerState SampleTypeWrap : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float2 depthDiv : TEXCOORD1;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 Depth : SV_Target2;
};

PixelOutputType main(PixelInputType Input)
{
	PixelOutputType output;
	output.color = float4(1, 1, 1, 1);
	output.normal = float4(.5, .5, .5, 1);

	// Sample the color from the texture and store it for output to the render target.
	//output.color = gCubeMap.Sample(SampleTypeWrap, float3(Input.tex, 0));

	// Store the normal for output to the render target.
	output.normal = float4((Input.normal + 1)*.5f, 1.0);
	output.Depth = Input.depthDiv.x / Input.depthDiv.y;
	return output;
}