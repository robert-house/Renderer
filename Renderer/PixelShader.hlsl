struct PS_INPUT
{
	float4 Position : SV_POSITION;  // interpolated vertex position
	float2 Texture    : TEXCOORD;       // interpolated diffuse color
};

struct PS_OUTPUT
{
	float4 Color : SV_TARGET;
};

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gDepth : register(t2);

SamplerState gSampleType;

PS_OUTPUT PS(PS_INPUT In)
{
	PS_OUTPUT Output;

	float4 diffuse;
	float4 normal;
	float3 lightdir;
	float lightint;
	float4 outputColor;

	diffuse = gAlbedo.Sample(gSampleType, In.Texture); // Get Color
	normal = gNormal.Sample(gSampleType, In.Texture); // Get Normals
	
	// Hardcoded light direction. Need to implement cBuffer for lights and light object
	lightdir = -float3(0.5f, -0.75f, -0.0f); 
	lightint = saturate(dot(normal.xyz, lightdir));

	Output.Color = saturate(diffuse * lightint);
	//Output.Color = gAlbedo.Sample(gSampleType , In.Texture);
	//Output.Color = gNormal.Sample(gSampleType, In.Texture);
	//Output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//Output.Color = gDepth.Sample(gSampleType, In.Texture);

	return Output;
}
