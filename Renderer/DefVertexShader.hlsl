cbuffer PerObjectConstantBuffer : register(b0)
{
	matrix mWorld;       // world matrix for object
	matrix View;        // view matrix
	matrix Projection;  // projection matrix
	matrix TexTransform;
};

struct VS_INPUT
{
	float3 Position		: POSITION;
	float2 Texture		: TEXCOORD;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float3 Binormal		: BINORMAL;
};

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION; // Vertex shaders must output SV_POSITION
	float2 Texture		: TEXCOORD;
	float3x3 TBNMatrix	: TBNMATRIX;
};

VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT Output;

	// Convert float3 to float4
	float4 pos = float4(input.Position, 1.0f);
	float4 normal = float4(input.Normal, 1.0f);
	float4 tangent = float4(input.Tangent, 1.0f);
	float4 binormal = float4(input.Binormal, 1.0f);


	// Transform the position from object space to homogeneous projection space
	pos = mul(pos, mWorld);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	Output.Position = pos;

	// Transform Texture Space
	Output.Texture = mul(float4(input.Texture, 0.0f, 1.0f), TexTransform).xy;

	// Transform and output tangent
	Output.TBNMatrix[0] = mul(tangent, mWorld).xyz;

	// Transform and output binormal
	Output.TBNMatrix[1] = mul(binormal, mWorld).xyz;

	// Transform and output normal
	Output.TBNMatrix[2] = mul(normal, mWorld).xyz;

	return Output;
}
