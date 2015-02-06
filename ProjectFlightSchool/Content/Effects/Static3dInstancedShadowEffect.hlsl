//Vertex
cbuffer CbufferPerFrameShadow	: register( b2 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4		cameraPosition;
}

struct VS_In
{
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float2 uv				: TEX;
	float4x4 worldMatrix	: WORLD;
};

struct VS_Out
{
	float4 position			: SV_POSITION;
	float2 uv				: TEX;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output			= (VS_Out)0;
	output.position			= mul( float4( input.position, 1.0f ), input.worldMatrix );
	output.position			= mul( output.position, viewMatrix );
	output.position			= mul( output.position, projectionMatrix );

	output.uv				= input.uv;

	return output;
}

Texture2D<float4> diffuseTexture	: register( t0 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
void PS_main( VS_Out input )
{	
	clip ( diffuseTexture.Sample( linearSampler, input.uv ).w < 0.7f ? -1:1 );
}