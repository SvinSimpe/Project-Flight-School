//Vertex
struct VS_In
{
	float3 unused			: POSITION;
	float3 position			: WORLDPOSITION;
	float  age				: AGE;
	float  timeTillDeath	: TIMETILLDEATH;
};

VS_In VS_main( VS_In input )
{
	return input;
}

//Geometry
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4x4	invViewProjMatrix;
	float4		cameraPosition;
	float4		shipsPosAndRad[2];
	int			numPointLights;
	float		timeVariable;
}

struct GS_Out
{
	float4 position			: SV_POSITION;
	float  timeTillDeath	: TIMETILLDEATH;
	float2 uv				: TEX;
};

[maxvertexcount(4)]
void GS_main( point VS_In input[1], inout TriangleStream<GS_Out> outputStream )
{
	float3 vecToCam = normalize( ( input[0].position - cameraPosition.xyz ) );
	float3 rightVec = float3( 1.0f, 0.0f, 0.0f );
	float3 upVec = normalize( cross( vecToCam, rightVec ) );

	float size = 0.02f;


	//Get vertices for the quad
	float3 vert[4];
	vert[0]	= input[0].position - rightVec * size - upVec * size;
	vert[1]	= input[0].position - rightVec * size + upVec * size;
	vert[2]	= input[0].position	+ rightVec * size - upVec * size;
	vert[3]	= input[0].position + rightVec * size + upVec * size;
								
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
		outputVert.position			= mul( mul( float4( vert[i], 1.0f ), viewMatrix ), projectionMatrix );
		outputVert.timeTillDeath	= input[0].timeTillDeath;
		outputVert.uv				= texCoord[i];
		outputStream.Append( outputVert );
	}
}

//Pixel
Texture2D<float4> diffuseTexture	: register( t0 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
float4 PS_main(GS_Out input) : SV_TARGET0
{	
	//clip( diffuseTexture.Sample( linearSampler, input.uv ).w < 0.7f ? -1:1 );
	//return float4( diffuseTexture.Sample( linearSampler, input.uv ) );
	float4 diffuse = float4( diffuseTexture.Sample( linearSampler, input.uv ) );

	diffuse.w = diffuse.w * input.timeTillDeath;

	return diffuse;
}