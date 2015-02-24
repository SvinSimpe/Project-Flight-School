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

struct PointLight
{
	float4 position;
	float4 colorAndRadius;
};

cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4		cameraPosition;
	int			numPointLights;
}

cbuffer CbufferPerFrameShadow : register( b1 )
{
	float4x4	shadowViewMatrix;
	float4x4	shadowProjectionMatrix;
	float4		shadowCameraPosition;
}

Texture2D<float4>				albedoSpecBuffer		: register( t0 );
Texture2D<float4>				normalBuffer			: register( t1 );
Texture2D<float4>				worldPositionBuffer		: register( t2 );
Texture2D<float4>				shadowMap				: register( t4 );
StructuredBuffer<PointLight>	lightStructure			: register( t5 );

SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
float4 PS_main( VS_Out input ) : SV_TARGET0
{
	float3	albedoSample	= albedoSpecBuffer.Sample( pointSampler, input.uv ).xyz;
	float	specularSample	= albedoSpecBuffer.Sample( pointSampler, input.uv ).w;
	float3	normalSample	= normalBuffer.Sample( pointSampler, input.uv ).xyz;
	float3	worldSample		= worldPositionBuffer.Sample( linearSampler, input.uv).xyz;

	//========== Screen Space Ambient Occlusion =============
	// USED FOR TESTING SSAO ONLY, SUCKY VERSION, JOCKE PLEZ HALP
	float ssao	= 0.0f;
	float rad	= 0.08f;

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
			float l		= length( dif ) * 0.5f;
			dif			= normalize( dif );
		
			ssao +=	max( 0.0f, dot( normalSample, dif ) - 0.01f ) * 1.0f / ( 1.0f + l ) * 1.0f;
		}
	}

	ssao = 1.0f - ssao / 16.0f;

	//-------------------------------------------------------------------------------------------------
	//	SHADOW MAP
	//-------------------------------------------------------------------------------------------------
	float SHADOW_BIAS = 0.001f;

	float shadowSamples = 0.0f;

	float4 posLightH	= mul( float4( worldSample, 1.0f ), shadowViewMatrix );
	posLightH			= mul( posLightH, shadowProjectionMatrix );
	posLightH.xy	   /= posLightH.w;

	float2 smTex = float2( 0.5f * posLightH.x, -0.5f * posLightH.y ) + 0.5f;

	float depth = posLightH.z / posLightH.w;

	float dx = 1.0f / 512.0f;
	float dy = 1.0f / 512.0f;
	smTex	-= float2( dx * 0.5f, dy * 0.5f );

	//25 samples
	for( int i = -2; i < 3; i++ )
		for( int j = -2; j < 3; j++ )
			shadowSamples += shadowMap.Sample( linearSampler, smTex + float2( dx * i, dy * j ) ).r + SHADOW_BIAS < depth ? 0.0f : 1.0f;

	float shadowFactor = shadowSamples / 25.0f;

	//======== SHADOW MAP POINTLIGHT ===========
	float3 ambient		= float3( 0.8f, 0.8f,  0.8f );
	float3 color		= float3( 0.6f, 0.3f,  0.6f );

	float3 lightDirection	= worldSample - shadowCameraPosition.xyz;
	float dShadow			= length( lightDirection );
	lightDirection			/= dShadow;

	//Calculate diffuse factor
	float	diffuseFactor	= saturate( dot( -lightDirection, normalSample ) );
	float3	diffuse			= float3( diffuseFactor, diffuseFactor, diffuseFactor );

	//Calculate specular factor
	float3 reflection		= normalize( reflect( -lightDirection, normalSample ) );
	float3 viewVector		= normalize( worldSample - cameraPosition.xyz );
	float specularFactor	= saturate( dot( reflection, viewVector ) );
	float specularPower		= 4.0f;
	float3 specular			= float3( float3( 1.0f, 1.0f, 1.0f ) * pow( specularFactor, specularPower ) ) * specularSample;

	float3 finalColor		= ( ( ( ambient * ssao + diffuse + specular ) * color ) * shadowFactor ) / ( dShadow * 0.01f + dShadow * dShadow * 0.005f );


	//-------------------------------------------------------------------------------------------------
	//	POINT LIGHT FUCK YOU MAX
	//-------------------------------------------------------------------------------------------------

	for( int i = 0; i < numPointLights; i++ )
	{
		float3 lightDir = worldSample - lightStructure[i].position.xyz;
		float d			= length( lightDir );
		lightDir		/= d;
		
		float3 N = normalSample;
		float3 V = cameraPosition.xyz;
		float3 R = reflect( lightDir, N );

		float diff	= saturate( dot( -lightDir, N ) );
		float3 spec	= float3( lightStructure[i].colorAndRadius.xyz * pow( dot( R, V ), specularPower ) ) * specularSample;

		//float radiusInverse = 1.0f / lightStructure[i].colorAndRadius.w;
		//finalColor += ( ( diffuse + specular ) * lightStructure[i].colorAndRadius.xyz ) 
		//				/ ( 0.5f + d * ( 1.0f * radiusInverse ) + d * d * ( 1.0f * radiusInverse ) );

		float denom			= d / lightStructure[i].colorAndRadius.w + 1.0f;
		float attenuation	= 1.0f / ( denom * denom );

		finalColor += ( diffuse + specular ) * lightStructure[i].colorAndRadius.xyz * attenuation;
	}

	saturate( finalColor );

	//Water hotfix?
	if( worldSample.y < -0.5f )
		albedoSample = ( albedoSample * 0.3 + float3( 0.6f, 0.6f, 0.7f ) * 0.7f ) * max( 0.4f, saturate( ( 1 + ( worldSample.y + 0.5f ) * 0.3f ) ) );

	return float4( albedoSample, 1.0f );
	//return float4( specularSample, specularSample, specularSample, 1.0f );
	//return float4( normalSample, 1.0f );
	//return float4( ssao, 0.0f, 0.0f, 1.0f );

	//return float4( finalColor * ( shadowFactor * 0.3f + 0.7f ) * albedoSample, 1.0f );
}