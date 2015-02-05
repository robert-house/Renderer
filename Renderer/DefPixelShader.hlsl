struct PS_INPUT
{
	float4 Position : SV_POSITION;  // interpolated vertex position
	float2 Texture    : TEXCOORD;   // interpolated diffuse color
	float3 Normal : NORMAL;		// Interpolated Normal
};

struct PS_OUTPUT
{
	float4 Color : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 Depth	: SV_TARGET2;
};

Texture2D gDiffuse : register(t0);
Texture2D gNormal : register(t1);

SamplerState gSampleType;

PS_OUTPUT PS(PS_INPUT In)
{
	PS_OUTPUT Output;

	Output.Color = gDiffuse.Sample(gSampleType, In.Texture);
	//Output.Normal = gNormal.Sample(gSampleType, In.Texture);
	Output.Normal = float4(In.Normal/2.0f + 0.5f, 1.0f);
	Output.Depth = In.Position.z / In.Position.w;

	return Output;
}
