struct PS_INPUT
{
	float4 Position : SV_POSITION;  // interpolated vertex position
	float2 Texture    : TEXCOORD;       // interpolated diffuse color
};

struct PS_OUTPUT
{
	float4 Color : SV_TARGET;
};

Texture2D gDiffuse : register(t0);
Texture2D gNormal : register(t1);
Texture2D gSpecular : register(t2);

SamplerState gSampleType;

// COOK TORRANCE BRDF!
PS_OUTPUT PS(PS_INPUT In)
{
	PS_OUTPUT Output;

	// Sample the textures
	float3  Normal			= gNormal.Sample(gSampleType, In.Texture);
	float3  Specular		= gSpecular.Sample(gSampleType, In.Texture);
	float3  Diffuse			= gDiffuse.Sample(gSampleType, In.Texture);
	float  Roughness		= 0.1f;

	// Regulate to the CBUFFERs once they are complete
	float3 LightColor	= float3(1.0f, 1.0f, 1.0f);
	float3 cViewDir		= float3(1.0f, 0.5f, 0.5f);
	float3 cLightDir	= float3 (0.0f, 1.0f, 1.0f);


	// Correct the input and compute aliases
	float3  ViewDir			= normalize(cViewDir);
	float3  LightDir		= normalize(cLightDir);
	float3  vHalf			= normalize(LightDir + ViewDir);
	float  NormalDotHalf	= dot(Normal, vHalf);
	float  ViewDotHalf		= dot(vHalf, ViewDir);
	float  NormalDotView	= dot(Normal, ViewDir);
	float  NormalDotLight	= dot(Normal, LightDir);

	// Compute the geometric term
	float  G1		= (2.0f * NormalDotHalf * NormalDotView) / ViewDotHalf;
	float  G2		= (2.0f * NormalDotHalf * NormalDotLight) / ViewDotHalf;
	float  G		= min(1.0f, max(0.0f, min(G1, G2)));

	// Compute the fresnel term
	float  F		= Roughness + (1.0f - Roughness) * pow(1.0f - NormalDotHalf, 5.0f);
	//float F0 = pow((1 - Roughness) / (1 + Roughness), 2);
	//float  F = F0 + (1.0f - F0) * pow(1.0f - NormalDotHalf, 5.0f);

	// Compute the roughness term D
	float  R_2		= Roughness * Roughness;
	float  NDotH_2	= NormalDotHalf * NormalDotHalf;
	float  r1		= 1.0f / (R_2 * NDotH_2 * NDotH_2);
	float  r2		= (NDotH_2 - 1.0f) / (R_2 * NDotH_2);
	float  D		= r1 * exp(r2);

	// Compute the final term
	float3  S = ((G * F * D) / (4 *(NormalDotLight * NormalDotView)));
	float3  Final = LightColor * max(0.0f,NormalDotLight) * (Diffuse + (S * Specular));

	Output.Color = float4(Final, 1.0f);
	//Output.Color = float4(Normal, 1.0f);

	return Output;
}