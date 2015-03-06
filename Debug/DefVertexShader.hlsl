cbuffer PerObjectConstantBuffer : register(b0)
{
	matrix mWorld;       // world matrix for object
	matrix View;        // view matrix
	matrix Projection;  // projection matrix
	matrix TexTransform;
};

struct VS_INPUT
{
	float3 Position   : POSITION;
	float2 Texture : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // Vertex shaders must output SV_POSITION
	float2 Texture    : TEXCOORD;
	float3 Normal : NORMAL;
};

VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT Output;

	float4 pos = float4(input.Position, 1.0f);

	// Transform the position from object space to homogeneous projection space
	pos = mul(pos, mWorld);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	Output.Position = pos;

	// Transform Texture Space
	Output.Texture = mul(float4(input.Texture, 0.0f, 1.0f), TexTransform).xy;

	// Output Normal
	Output.Normal = mul(input.Normal, (float3x3)mWorld);
	Output.Normal = normalize(Output.Normal);

	return Output;
}
