//Vertex
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

struct VS_In
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float4 uv		: TEX;
};

struct VS_Out
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output	= (VS_Out)0;
	output.position = mul( float4( input.position.xyz, 1.0f ), viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.normal = input.normal.xyz;

	return output;
}

//Pixel
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	//return float4( 1.0f, 1.0f, 1.0f, 1.0f );
	return float4( input.normal.xyz, 1.0f );
}