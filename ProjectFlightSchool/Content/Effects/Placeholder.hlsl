//Vertex
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

struct VS_In
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 uv		: TEX;
};

struct VS_Out
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEX;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output	= (VS_Out)0;
	output.position = mul( float4( input.position, 1.0f ), viewMatrix );
	output.position = mul( output.position, projectionMatrix );

	output.normal	= input.normal.xyz;

	output.uv		= input.uv;

	return output;
}

//Pixel
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	// Derp light to notice anything
	float lightIntensity	= dot( input.normal, normalize( float3( 0.5f, 1.0f, -0.3f ) ) ); 
	float3 color			= ( 1.0f, 1.0f, 1.0f );

	return float4( color * lightIntensity, 1.0f );
	//return float4( input.normal, 1.0f );
}