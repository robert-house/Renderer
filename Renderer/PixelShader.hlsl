cbuffer PerFrame		: register(b0)
{
	float4 viewVector;
	float4 globalLightVector;
	float4 globalLightColor;
}

struct PS_INPUT
{
	float4 Position		: SV_POSITION;  // interpolated vertex position
	float2 Texture		: TEXCOORD;       // interpolated diffuse color
};

struct PS_OUTPUT
{
	float4 Color		: SV_TARGET;
};

Texture2D gDiffuse		: register(t0);
Texture2D gNormal		: register(t1);
Texture2D gRoughness	: register(t2);

SamplerState gSampleType : register(s0);

// COOK TORRANCE BRDF!
PS_OUTPUT PS(PS_INPUT In)
{
	PS_OUTPUT Output;

	// Sample the textures
	float3 Normal			= gNormal.Sample(gSampleType, In.Texture).rgb;
	float  Specular			= gNormal.Sample(gSampleType, In.Texture).a;
	float3 Diffuse			= gDiffuse.Sample(gSampleType, In.Texture).rgb;
	float  Roughness		= gRoughness.Sample(gSampleType, In.Texture).r;

	// Grab CBuffer values (per frame)
	float4 LightColor		= globalLightColor;
	float3 cViewDir			= viewVector;
	float3 cLightDir		= globalLightVector;

	// Helper variables to make life easier
	float3 ViewDir			= normalize(cViewDir);
	float3 LightDir			= normalize(cLightDir);
	float3 vHalf			= normalize(LightDir + ViewDir);
	float  NormalDotHalf	= dot(Normal, vHalf);
	float  ViewDotHalf		= dot(vHalf, ViewDir);
	float  NormalDotView	= dot(Normal, ViewDir);
	float  NormalDotLight	= dot(Normal, LightDir);

	// Compute the geometric term
	float  G1				= (2.0f * NormalDotHalf * NormalDotView) / ViewDotHalf;
	float  G2				= (2.0f * NormalDotHalf * NormalDotLight) / ViewDotHalf;
	float  G				= min(1.0f, max(0.0f, min(G1, G2)));

	// Compute the fresnel term F (Schlick Approximation)
	//float  F		= Roughness + (1.0f - Roughness) * pow(1.0f - NormalDotHalf, 5.0f);
	float  F0				= pow((1 - Roughness) / (1 + Roughness), 2);
	float  F				= F0 + (1.0f - F0) * pow(1.0f - NormalDotHalf, 5.0f);

	// Compute the roughness term D (Beckmann Distribution)
	float  R_2				= Roughness * Roughness;
	float  NDotH_2			= NormalDotHalf * NormalDotHalf;
	float  r1				= 1.0f / (R_2 * NDotH_2 * NDotH_2);
	float  r2				= (NDotH_2 - 1.0f) / (R_2 * NDotH_2);
	float  D				= r1 * exp(r2);

	// Compute the final term
	float3 S				= Specular * ((G * F * D) / 
								(4 * (NormalDotLight * NormalDotView)));
	float3 Final			= LightColor * max(0.0f, NormalDotLight) * (Diffuse + S);

	Output.Color = float4(Final, 1.0f);
	//Output.Color = gDiffuse.Sample(gSampleType, In.Texture);
	//Output.Color = float4(gRoughness.Sample(gSampleType, In.Texture).r, 1.0f, 0.0f, 1.0f);
	//Output.Color = float4(gNormal.Sample(gSampleType, In.Texture).rgb, 1.0f);

	return Output;
}