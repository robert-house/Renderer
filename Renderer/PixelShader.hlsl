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

/*
float4 CookTorrance(PS_INPUT In)
{
	// Sample the textures
	float3  Normal = gNormal.Sample(gSampleType, In.Texture);
	float3  Specular = gSpecular.Sample(gSampleType, In.Texture);
	float3  Diffuse = gDiffuse.Sample(gSampleType, In.Texture);
	float  Roughness = 0.2f;

	Roughness *= 3.0f;

	// Regulate to the CBUFFERs once they are complete
	float3 LightColor = (255.0f, 255.0f, 255.0f);
	vector cViewDir = (1, 1, 1);
	vector cLightDir = (0.5f, -0.75f, -0.0f);


	// Correct the input and compute aliases
	float3  ViewDir = normalize(cViewDir);
		float3  LightDir = normalize(cLightDir);
		float3  vHalf = normalize(LightDir + ViewDir);
		float  NormalDotHalf = dot(Normal, vHalf);
	float  ViewDotHalf = dot(vHalf, ViewDir);
	float  NormalDotView = dot(Normal, ViewDir);
	float  NormalDotLight = dot(Normal, LightDir);

	// Compute the geometric term
	float  G1 = (2.0f * NormalDotHalf * NormalDotView) / ViewDotHalf;
	float  G2 = (2.0f * NormalDotHalf * NormalDotLight) / ViewDotHalf;
	float  G = min(1.0f, max(0.0f, min(G1, G2)));

	// Compute the fresnel term g
	float  F = Roughness + (1.0f - Roughness) * pow(1.0f - NormalDotView, 5.0f);

	// Compute the roughness term r
	float  R_2 = Roughness * Roughness;
	float  NDotH_2 = NormalDotHalf * NormalDotHalf;
	float  A = 1.0f / (4.0f * R_2 * NDotH_2 * NDotH_2);
	float  B = exp(-(1.0f - NDotH_2) / (R_2 * NDotH_2));
	float  R = A * B;

	// Compute the final term
	float3  S = Specular * ((G * F * R) / (NormalDotLight * NormalDotView));
	float3  Final = LightColor * max(0.0f, NormalDotLight) * (Diffuse + S);

		return float4(Final, 1.0f);
}
*/


PS_OUTPUT PS(PS_INPUT In)
{
	PS_OUTPUT Output;

	float4 diffuse;
	float4 normal;
	float3 lightdir;
	float lightint;
	float4 outputColor;

	diffuse = gDiffuse.Sample(gSampleType, In.Texture); // Get Color
	normal = gNormal.Sample(gSampleType, In.Texture); // Get Normals

	// Hardcoded light direction. Need to implement cBuffer for lights and light object
	lightdir = -float3(0.0f, -1.0f, 0.0f);
	lightint = saturate(dot(normal.xyz, lightdir));
	float3 camera = float3(1, 1, 1);

	//float3 N = (2.0 * normal – 1.0);
	
	// reflection
	//float3 R = normalize(2 * D * N – diffuse);

	// specular
	//float S = pow(saturate(dot(R, V)), 2);

	// calculate light (ambient + diffuse + specular)
	//const float4 Ambient = float4(0.3, 0.3, 0.3, 1.0);
	//return Color*Ambient + Color * D + Color*S;

	

	//Output.Color = saturate(diffuse * lightint);
	//Output.Color = gDiffuse.Sample(gSampleType , In.Texture);
	Output.Color = gNormal.Sample(gSampleType, In.Texture);
	//Output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//Output.Color = gSpecular.Sample(gSampleType, In.Texture);

	//Output.Color = CookTorrance(In);

	return Output;
}