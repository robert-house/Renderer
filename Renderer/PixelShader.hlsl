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

float4 cook_torrance
(
in float3 normal,
in float3 viewer,
in float3 light,
uniform int roughness_mode
)
{
	// Compute any aliases and intermediary values
	// -------------------------------------------
	float3 half_vector = normalize(light + viewer);
	float NdotL = saturate(dot(normal, light));
	float NdotH = saturate(dot(normal, half_vector));
	float NdotV = saturate(dot(normal, viewer));
	float VdotH = saturate(dot(viewer, half_vector));
	float r_sq = roughness_value * roughness_value;



	// Evaluate the geometric term
	// --------------------------------
	float geo_numerator = 2.0f * NdotH;
	float geo_denominator = VdotH;

	float geo_b = (geo_numerator * NdotV) / geo_denominator;
	float geo_c = (geo_numerator * NdotL) / geo_denominator;
	float geo = min(1.0f, min(geo_b, geo_c));



	// Now evaluate the roughness term
	// -------------------------------
	float roughness;

	if (ROUGHNESS_LOOK_UP == roughness_mode)
	{
		// texture coordinate is:
		float2 tc = { NdotH, roughness_value };

			// Remap the NdotH value to be 0.0-1.0
			// instead of -1.0..+1.0
			tc.x += 1.0f;
		tc.x /= 2.0f;

		// look up the coefficient from the texture:
		roughness = texRoughness.Sample(sampRoughness, tc);
	}
	if (ROUGHNESS_BECKMANN == roughness_mode)
	{
		float roughness_a = 1.0f / (4.0f * r_sq * pow(NdotH, 4));
		float roughness_b = NdotH * NdotH - 1.0f;
		float roughness_c = r_sq * NdotH * NdotH;

		roughness = roughness_a * exp(roughness_b / roughness_c);
	}
	if (ROUGHNESS_GAUSSIAN == roughness_mode)
	{
		// This variable could be exposed as a variable
		// for the application to control:
		float c = 1.0f;
		float alpha = acos(dot(normal, half_vector));
		roughness = c * exp(-(alpha / r_sq));
	}



	// Next evaluate the Fresnel value
	// -------------------------------
	float fresnel = pow(1.0f - VdotH, 5.0f);
	fresnel *= (1.0f - ref_at_norm_incidence);
	fresnel += ref_at_norm_incidence;



	// Put all the terms together to compute
	// the specular term in the equation
	// -------------------------------------
	float3 Rs_numerator = (fresnel * geo * roughness);
		float Rs_denominator = NdotV * NdotL;
	float3 Rs = Rs_numerator / Rs_denominator;



		// Put all the parts together to generate
		// the final colour
		// --------------------------------------
		float3 final = max(0.0f, NdotL) * (cSpecular * Rs + cDiffuse);



		// Return the result
		// -----------------
		return float4(final, 1.0f);
}


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
	float3 camera = float3(1, 1, 1);

		//Output.Color = saturate(diffuse * lightint);
		Output.Color = cook_torrance(normal.xyz, camera, lightdir, 1);
	//Output.Color = gAlbedo.Sample(gSampleType , In.Texture);
	//Output.Color = gNormal.Sample(gSampleType, In.Texture);
	//Output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//Output.Color = gDepth.Sample(gSampleType, In.Texture);

	return Output;
}