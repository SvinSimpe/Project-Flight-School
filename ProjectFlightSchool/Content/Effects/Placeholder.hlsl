//Vertex

struct VS_In
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float3 uv		: TEX;
};

struct VS_Out
{
	float4 position	: SV_POSITION;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output	= (VS_Out)0;
	output.position = float4( input.position, 1.0f );
	return output;
}

//Pixel
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}