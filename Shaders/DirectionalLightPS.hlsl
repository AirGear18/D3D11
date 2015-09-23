
////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypePoint : register(s0);


//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer LightBuffer : register(b0)
{
	float4 lightDirection;
	float4 m_ambientColor;
	float4 m_diffuseColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float4 colors, normals, ambientColor;
	float3 lightDir;
	float nDotL, lightIntensity;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	colors = colorTexture.Sample(SampleTypePoint, input.tex);
	//return colors;
	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	normals = normalTexture.Sample(SampleTypePoint, input.tex);
	normals = (normals * 2) - 1;
	// Invert the light direction for calculations.
	//lightDir = -lightDirection;
	nDotL = saturate(dot(normals, -lightDirection));
	colors = float4(nDotL * colors.xyz*m_diffuseColor.xyz, 1);

	// Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(normals.xyz, lightDir));

	// Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.
	//outputColor = saturate(colors * lightIntensity);

	ambientColor = float4((m_ambientColor.xyz *colors.xyz), 1);

	return ambientColor *(colors);
}
