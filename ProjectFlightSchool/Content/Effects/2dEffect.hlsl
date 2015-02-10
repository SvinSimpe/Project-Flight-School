//Vertex

struct VS_In
{
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float2 uv				: TEX;
};

struct VS_Out
{
	float4 position			: SV_POSITION;
	float2 uv				: TEX;
};

VS_Out VS_main(VS_In input)
{
	VS_Out output	= (VS_Out)0;
	output.position = float4( input.position, 1.0f );
	output.uv		= input.uv;

	return output;
}

//Pixel

cbuffer CbufferPerObject : register( b0 )
{
	float4 color;
}

Texture2D<float4> diffuseTexture	: register( t0 );
SamplerState linearSampler			: register( s1 );

float4 PS_main(VS_Out input) : SV_TARGET0
{
	return diffuseTexture.Sample( linearSampler, input.uv ) * color;
}