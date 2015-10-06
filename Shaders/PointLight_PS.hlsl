#pragma pack_matrix(row_major)
/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D DepthTexture : register(t2);

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer : register(b0)
{
	/// The position the light shines from.
	float4 position;

	float4 Attenuation;
	/// The color of the diffuse component of the light.
	float3 diffuseColor;
	/// The maximum distance this light should effect, adjusts the resulting attenuation to match
	float range;
	/// The color of the ambient component of the light.
	//float3 ambientColor;
	/// The color of the specular component of the light.
	//float3 specularColor;
	///(x = constant attenuation, y = linear attenuation, z = quadratic attenuation)
	float3 CameraPosition;
	/// Controls the brightness of the specualr reflection.
	//float specularIntensity;

	/// Controls how big the specular reflection is. A smaller value creates a large reflection.
	//float specularPower;
	float padding;
};

cbuffer InvViewProjBuffer : register(b1)
{
	/// The current cameras current inverse view-projection matrix
	float4x4	gInvViewProj;
}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 pixelPosition : TEXCOORD0;
	//float4 positionWP : WorldPos;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
};
/////////////////////////////
float4 CalculateWorldSpacePosition(float2 pixelPosition, float pixelDepth,
	float4x4 inverseViewProjection)
{
	float4 ViewPos = float4(pixelPosition, pixelDepth, 1);

		float4 WorldPos = mul(ViewPos, inverseViewProjection);

		WorldPos /= WorldPos.w;
	return WorldPos;
}
/////////////
float CalculateAttenuation(float3 attenuationFactors, float distanceToLight, float lightRange,
	float numerator = 1.0)
{
	float Attenuation;

	Attenuation = numerator / (attenuationFactors.x + attenuationFactors.y*distanceToLight + attenuationFactors.z*distanceToLight*distanceToLight);

	return saturate(Attenuation);
}
//float2 HalfPixel = float2(.2f, .2f);

float4 main(PixelInputType input) : SV_Target0
{
	PixelOutputType output;
	//output.color = float4(1, 1, 1, 1);

	float2 ClipSpace, texCoord;
	float4 /*specular,*/ posWorld, diffuse,
		finalAmbient, finalDiffuse/*, finalSpecular*/;
	float3 toLight, normal/*, reflectionVector*/, directionToCamera;
	float toLightLength, attenuation = 0.1, nDotL/*, specMod*/, Depth;


	ClipSpace = float2(input.pixelPosition.xy / input.pixelPosition.w);
	texCoord = float2(ClipSpace.x, -ClipSpace.y)*.5f + .5f;

	//Depth = DepthTexture.Load(int3(texCoord.xy, 0));
	Depth = DepthTexture.Sample(SampleTypePoint, texCoord).x;

	diffuse = colorTexture.Sample(SampleTypePoint, texCoord);

	normal = normalTexture.Sample(SampleTypePoint, texCoord).xyz;
	normal = normal * 2 - 1;

	posWorld = CalculateWorldSpacePosition(ClipSpace, Depth, gInvViewProj);

	toLight = position.xyz - posWorld.xyz;
	toLightLength = length(toLight);
	// attenuation
	attenuation = max(0, 1.0f - (toLightLength / range)); //= CalculateAttenuation(Attenuation.xyz, toLightLength, range);
	toLight /= toLightLength;

	nDotL = saturate(dot(normal, toLight));
	finalDiffuse = float4(nDotL*diffuse* float4(diffuseColor, 1));
	////
	finalAmbient = float4((float3(1, 1, 1) *diffuse.xyz) *diffuse.w, 1);


	//float alpha = toLightLength / range;
	//float damping = saturate(1.0f - alpha*alpha);
	//attenuation *= damping;
	//return float4(1, 1, 1, 1);
	return (finalDiffuse /*+ finalAmbient*/)*attenuation;
}

//// attenuation
//attenuation = CalculateAttenuation(attenuation, toLightLength, range);


//
//finalDiffuse = float4(/*nDotL**/diffuse.xyz* diffuseColor, 1);

//float alpha = toLightLength / range;
//float damping = saturate(1.0f - alpha*alpha);
//attenuation *= damping;

//output.color = float4(1, 0, 0, 1); //finalDiffuse;// *attenuation;

//return output;


