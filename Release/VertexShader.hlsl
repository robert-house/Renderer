struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // Vertex shaders must output SV_POSITION
	float2 UV    : TEXCOORD;
};

VS_OUTPUT VS(uint VertexID : SV_VertexID)
{
	VS_OUTPUT Output;

	// Transform Texture Space
	Output.UV = float2((VertexID << 1) & 2, VertexID & 2);

	//Fullscreen QUAD
	Output.Position = float4(Output.UV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

	return Output;
}
