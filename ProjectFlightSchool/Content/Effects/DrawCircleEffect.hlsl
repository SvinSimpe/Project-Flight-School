
static const float PI = 3.14159265f;
//Vertex
struct VS_In
{
	float3	pos		: POSITION;
	float	radius	: RADIUS;
	float3	color	: TEX;
};

VS_In VS_main( VS_In input )
{
	return input;
}

//Geometry
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

struct GS_Out
{
	float4 position	: SV_POSITION;
	float3 color	: TEX;
};

[maxvertexcount(32)]
void GS_main( point VS_In input[1], inout LineStream<GS_Out> outputStream )
{
	int circlePrec = 32;
	GS_Out outputVert = (GS_Out)0;

	float step	= ( 2.0f * PI ) / circlePrec;
	[unroll]
	for( int i = 0; i < circlePrec; i++ )
	{
		float x = input[0].pos.x + ( input[0].radius * cos( step * ( i + 1 ) ) );
		float z = input[0].pos.z + ( input[0].radius * sin( step * ( i + 1 ) ) );

		float3 vert = float3( x, input[0].pos.y, z );
		outputVert.position = mul( mul( float4( vert, 1.0f ), viewMatrix ), projectionMatrix );
		outputVert.color	= input[0].color;
		outputStream.Append( outputVert );
	}
}
//Pixel
float4 PS_main(GS_Out input) : SV_TARGET0
{	
	return float4( input.color, 1.0f );
}