//Vertex
struct VS_Out
{
	float4 position			: SV_POSITION;
	float2 uv				: TEX;
};

VS_Out VS_main( uint index : SV_VertexID )
{
	VS_Out output = (VS_Out)0;

	if( index == 0 )
	{
		output.position = float4( -1.0f, -1.0f, 0.0f, 1.0f );
		output.uv		= float2( 0.0f, 1.0f );
	}
	else if( index == 1 )
	{
		output.position = float4( -1.0f, 1.0f, 0.0f, 1.0f );
		output.uv		= float2( 0.0f, 0.0f );
	}
	else if( index == 2 )
	{
		output.position = float4( 1.0f, -1.0f, 0.0f, 1.0f );
		output.uv		= float2( 1.0f, 1.0f );
	}
	else( index == 3 )
	{
		output.position = float4( 1.0f, 1.0f, 0.0f, 1.0f );
		output.uv		= float2( 1.0f, 0.0f );
	}
	return output;
}

//Pixel

Texture2D<float4> albedoBuffer	: register( t0 );
Texture2D<float4> normalBuffer	: register( t1 );

SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	return float4( albedoBuffer.Sample( pointSampler, input.uv ).xyz, 1.0f );
}