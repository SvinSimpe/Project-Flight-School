//Vertex
struct VS_In
{
	float3 unused			: POSITION;
	float3 position			: WORLDPOSITION;
	float  age				: AGE;
	float  timeTillDeath	: TIMETILLDEATH;
};

struct VS_Out
{
	float3	worldPosition	: WPOSITION;
	float	grassBool		: GRASS;
};

Texture2D<float4>	worldPosition	: register( t0 );
SamplerState		pointSampler	: register( s0 );
VS_Out VS_main( uint index : SV_VertexID )
{
	VS_Out output = (VS_Out)0;
	float4 inputSample		= worldPosition.SampleLevel( pointSampler, float2( (float)( index % 1920 ) / 1920.f, (float)( index / 1920 ) / 1080.0f ), 0 );
	output.worldPosition	= inputSample.xyz;
	output.grassBool		= inputSample.w;
	return output;
}

//Geometry
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4		cameraPosition;
	int			numPointLights;
	float		timeVariable;
}

struct GS_Out
{
	float4 position			: SV_POSITION;
	float3 worldPosition	: WPOSITION;
	float2 uv				: UV;
};

[maxvertexcount(4)]
void GS_main( point VS_Out input[1], inout TriangleStream<GS_Out> outputStream )
{
	if( input[0].grassBool > 0.5f )
	{
		float4 verts[4];

		float3 offset = float3( sin( timeVariable + input[0].worldPosition.x * 0.15f ) * 0.2f,
								0.0f,
								cos( timeVariable * 0.5f + input[0].worldPosition.z * 0.15f ) * 0.1f );

		verts[0] = float4( input[0].worldPosition + float3( -0.5,	0.0f, 0.0f ), 1.0f );
		verts[1] = float4( input[0].worldPosition + float3( -0.5f,	1.0f, 0.1f ) + offset, 1.0f );
		verts[2] = float4( input[0].worldPosition + float3( 0.5f,	0.0f, 0.0f ), 1.0f );
		verts[3] = float4( input[0].worldPosition + float3( 0.5f,	1.0f, 0.1f ) + offset, 1.0f );

		float2 uvs[4];
		uvs[0] = float2( 0.0f, 1.0f );
		uvs[1] = float2( 0.0f, 0.0f );
		uvs[2] = float2( 1.0f, 1.0f );
		uvs[3] = float2( 1.0f, 0.0f );

		GS_Out outputVert = (GS_Out)0;
		[unroll]
		for( int i = 0; i < 4; i++ )
		{
			outputVert.position			= mul( mul( verts[i], viewMatrix ), projectionMatrix );
			outputVert.worldPosition	= verts[i];
			outputVert.uv				= uvs[i];
			outputStream.Append( outputVert );
		}
	}
}

//Pixel
struct PS_Out
{
	float4 albedoSpec		: SV_Target0;
	float4 normal			: SV_Target1;
	float4 worldPosition	: SV_Target2;
};

Texture2D<float4> grassTexture : register( t0 );
PS_Out PS_main( GS_Out input )
{	
	PS_Out output = (PS_Out)0;
	
	float4 color = grassTexture.Sample( pointSampler, input.uv );

	clip( color.w < 0.1f ? -1:1 );

	output.normal			= float4( 0.0f, 0.0f, -1.0f, 0.0f );
	output.albedoSpec		= float4( color.xyz, 1.0f );
	output.worldPosition	= float4( input.worldPosition, 0.0f );
	return output;
}