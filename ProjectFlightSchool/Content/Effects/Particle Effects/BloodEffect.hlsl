//Vertex
struct VS_In
{
	float3 unused	: POSITION;
	float3 position	: WORLDPOSITION;
	float  lifeTime : LIFETIME;
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
	float4	 cameraPosition;
}

struct GS_Out
{
	float4 position	: SV_POSITION;
	float  lifeTime : LIFETIME;
	float2 uv		: TEX;
};

[maxvertexcount(4)]
void GS_main( point VS_In input[1], inout TriangleStream<GS_Out> outputStream )
{
	float3 vecToCam = normalize( ( input[0].position - cameraPosition.xyz ) );
	float3 rightVec = float3( 1.0f, 0.0f, 0.0f );
	float3 upVec = normalize( cross( vecToCam, rightVec ) );

	//Get vertices for the quad
	float3 vert[4];
	vert[0]	= input[0].position - rightVec * 0.25f - upVec * 0.25f;
	vert[1]	= input[0].position - rightVec * 0.25f + upVec * 0.25f;
	vert[2]	= input[0].position	+ rightVec * 0.25f - upVec * 0.25f;
	vert[3]	= input[0].position + rightVec * 0.25f + upVec * 0.25f;
															
	//Get texture coordinates
	float2 texCoord[4];
	texCoord[0]	= float2( 0.0f, 1.0f );
	texCoord[1]	= float2( 0.0f, 0.0f );
	texCoord[2]	= float2( 1.0f, 1.0f );
	texCoord[3]	= float2( 1.0f, 0.0f );

	GS_Out outputVert = (GS_Out)0;
	[unroll]
	for( int i = 0; i < 4; i++ )
	{
		outputVert.position = mul( mul( float4( vert[i], 1.0f ), viewMatrix ), projectionMatrix );
		outputVert.lifeTime	= input[0].lifeTime;
		outputVert.uv		= texCoord[i];
		outputStream.Append( outputVert );
	}
}

//Pixel
Texture2D<float4> diffuseTexture	: register( t0 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
float4 PS_main(GS_Out input) : SV_TARGET0
{	

	clip( diffuseTexture.Sample( linearSampler, input.uv ).w < 0.7f ? -1:1 );
	return float4( diffuseTexture.Sample( linearSampler, input.uv ).xyz, input.lifeTime );
}