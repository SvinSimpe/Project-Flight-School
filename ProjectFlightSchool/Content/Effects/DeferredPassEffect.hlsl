//Vertex

struct VS_In
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 uv		: TEX;
};

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
	else
	{
		output.position = float4( 1.0f, 1.0f, 0.0f, 1.0f );
		output.uv		= float2( 1.0f, 0.0f );
	}
	return output;
}

//Pixel

cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4	 cameraPosition;
}

Texture2D<float4> albedoSpecBuffer		: register( t0 );
Texture2D<float4> normalBuffer			: register( t1 );
Texture2D<float4> worldPositionBuffer	: register( t2 );

SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	float3	albedoSample	= albedoSpecBuffer.Sample( pointSampler, input.uv ).xyz;
	float	specularSample	= albedoSpecBuffer.Sample( pointSampler, input.uv ).w;
	float3	normalSample	= normalBuffer.Sample( pointSampler, input.uv ).xyz;
	float3	worldSample		= worldPositionBuffer.Sample( pointSampler, input.uv).xyz;

	//========== Screen Space Ambient Occlusion =============
	// USED FOR TESTING SSAO ONLY, SUCKY VERSION, JOCKE PLEZ HALP
	float ssao	= 0.0f;
	float rad	= 0.008f;

	float2 vec[8] = {	float2( 1.0f, 0.0f ), float2( -1.0f, 0.0f ),
						float2( 0.0f, 1.0f ), float2( 0.0f, -1.0f ),
						normalize( float2( 1.0f, 1.0f ) ), normalize( float2( -1.0f, -1.0f ) ),
						normalize( float2( -1.0f, 1.0f ) ), normalize( float2( 1.0f, -1.0f ) ), };

	[unroll]
	for( int j = 0; j < 8; j++ )
	{
		for( float k = 0.0f; k < 1.01f; k += 0.5f )
		{
			float3 dif	= worldPositionBuffer.Sample( pointSampler, input.uv + vec[j] * rad * k ).xyz - worldSample;
			float l		= length( dif ) * 2.0f;
			dif			= normalize( dif );
		
			ssao +=	max( 0.0f, dot( normalSample, dif ) - 0.01f ) * 1.0f / ( 1.0f + l ) * 10.0f;
		}
	}

	ssao = 1.0f - ssao / 16.0f;

	//======== PHONG SHADING ===========
	float3 ambient		= float3( 0.2f, 0.2f,  0.2f );
	float3 diffuse		= float3( 0.0f, 0.0f,  0.0f );
	float3 specular		= float3( 0.1f, 0.1f,  0.1f );
	float3 color		= float3( 0.75f, 0.8f,  0.8f );

	float3 lightDirection	= float3( -0.5f, -1.0f, 0.3f );

	//Calculate diffuse factor
	float diffuseFactor = max( 0, dot( normalSample, -lightDirection ) );
	diffuse				= color * diffuseFactor;

	//Calculate specular factor
	float3 reflection		= normalize( reflect( -lightDirection, normalSample ) );
	float3 viewVector		= normalize( worldSample - cameraPosition.xyz );
	float specularFactor	= saturate( dot( reflection, viewVector ) );
	float specularPower		= 4.0f;
	specular				= float3( float3( 1.0f, 1.0f, 1.0f ) * pow( specularFactor, specularPower ) ) * specularSample;

	float3 finalColor		= float3( ( ambient * ssao + diffuse + specular ) * color );

	//return float4( ambient * ssao, 1.0f );
	//return float4( specular, 1.0f );
	//return float4( normalSample, 1.0f );
	return float4( finalColor *	albedoSample, 1.0f );
}