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
	float4 positionAndIntensity;
	float4 colorAndRadius;
};

cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4		cameraPosition;
	int			numPointLights;
	float		timeVariable;
}

cbuffer CbufferPerFrameShadow : register( b1 )
{
	float4x4	shadowViewMatrix;
	float4x4	shadowProjectionMatrix;
	float4		shadowCameraPosition;
}

static const float SSAO_RAD		= 0.08f;
static const float SHADOW_BIAS	= 0.001f;
static const float SHADOW_DX	= 1.0f / 512.0f;

Texture2D<float4>				albedoSpecBuffer		: register( t0 );
Texture2D<float4>				normalBuffer			: register( t1 );
Texture2D<float4>				worldPositionBuffer		: register( t2 );
Texture2D<float4>				shadowMap				: register( t4 );
Texture2D<float4>				waterNormalMap			: register( t5 );
StructuredBuffer<PointLight>	lightStructure			: register( t6 );

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

	float2 vec[8] = {	float2( 1.0f, 0.0f ), float2( -1.0f, 0.0f ),
						float2( 0.0f, 1.0f ), float2( 0.0f, -1.0f ),
						float2( 0.707f, 0.707f ), float2( -0.707f, -0.707f ),
						float2( -0.707f, 0.707f ), float2( 0.707f, -0.707f ), };

	[unroll]
	for( int j = 0; j < 8; j++ )
	{
		for( float k = 0.0f; k < 1.01f; k += 0.5f )
		{
			float3 dif	= worldPositionBuffer.Sample( pointSampler, input.uv + vec[j] * SSAO_RAD * k ).xyz - worldSample;
			float l		= length( dif ) * 0.5f;
			dif			= normalize( dif );
		
			ssao +=	max( 0.0f, dot( normalSample, dif ) - 0.01f ) * 1.0f / ( 1.0f + l );
		}
	}

	ssao = 1.0f - ssao / 16.0f;

	//-------------------------------------------------------------------------------------------------
	//	SHADOW MAP
	//-------------------------------------------------------------------------------------------------

	float shadowSamples = 0.0f;

	float4 posLightH	= mul( float4( worldSample, 1.0f ), shadowViewMatrix );
	posLightH			= mul( posLightH, shadowProjectionMatrix );
	posLightH.xy	   /= posLightH.w;

	float2 smTex = float2( posLightH.x, -posLightH.y ) * 0.5f + 0.5f;

	float depth = posLightH.z / posLightH.w;

	smTex	-= float2( SHADOW_DX, SHADOW_DX ) * 0.5f;

	//16 samples
	[unroll]
	for( int k = -2; k < 2; k++ )
		[unroll]
		for( int l = -2; l < 2; l++ )
			shadowSamples += shadowMap.Sample( linearSampler, smTex + float2( SHADOW_DX * k, SHADOW_DX * l ) ).r + SHADOW_BIAS < depth ? 0.0f : 1.0f;

	float shadowFactor = shadowSamples * 0.0625f; // division by 16.0f;

	//======== SHADOW MAP POINTLIGHT ===========
	float3 ambient		= float3( 0.1f, 0.1f,  0.1f );
	float3 color		= float3( 0.6f, 0.4f, 0.6f ); //float3( 0.6f, 0.3f,  0.6f );

	float3 lightDirection	= worldSample - shadowCameraPosition.xyz;
	float dShadow			= length( lightDirection );
	lightDirection			/= dShadow;

	//Calculate diffuse factor
	float	diffuseFactor	= saturate( dot( -lightDirection, normalSample ) );
	float3	diffuse			= float3( diffuseFactor, diffuseFactor, diffuseFactor );

	//Calculate specular factor
	float specularFactor	= saturate( dot( normalize( reflect( -lightDirection, normalSample ) ), normalize( worldSample - cameraPosition.xyz ) ) );
	float specularPower		= 4.0f;
	float3 specular			= float3( 1.0f, 1.0f, 1.0f ) * pow( specularFactor, specularPower ) * specularSample;

	float3 finalColor		= ambient * ssao + ( ( ( diffuse + specular ) * color ) * shadowFactor ) / ( dShadow * 0.01f + dShadow * dShadow * 0.005f );


	//-------------------------------------------------------------------------------------------------
	//	POINT LIGHT FUCK YOU MAX
	//-------------------------------------------------------------------------------------------------

	for( int i = 0; i < numPointLights; i++ )
	{
		if( lightStructure[i].colorAndRadius.w > 0.01f )
		{
			float3 lightDir = worldSample - lightStructure[i].positionAndIntensity.xyz;
			float d			= length( lightDir );
			if( d < lightStructure[i].colorAndRadius.w * 2.0f )
			{
				lightDir		/= d;

				float diff	= saturate( dot( -lightDir, normalSample ) );
				float3 spec	= float3( lightStructure[i].colorAndRadius.xyz * pow( dot( reflect( lightDir, normalSample ), cameraPosition.xyz ), specularPower ) ) * specularSample;

				float denom			= d / lightStructure[i].colorAndRadius.w + 1.0f;
				float attenuation	= min( lerp( 1.0f, 0.0f, d / ( lightStructure[i].colorAndRadius.w * 2.0f ) ), 1.0f / ( denom * denom ) );

				finalColor += ( diffuse + specular ) * lightStructure[i].colorAndRadius.xyz * lightStructure[i].positionAndIntensity.w * attenuation;
			}
		}
	}

	saturate( finalColor );

	//-------------------------------------------------------------------------------------------------
	//	WATER SIMULATION
	//-------------------------------------------------------------------------------------------------
	float2 offset = float2( timeVariable, timeVariable ) / 25.132f;

	float3 toCamera = normalize( cameraPosition.xyz - worldSample );

	//ray vs plane
	float t = ( -0.5f -dot( worldSample, float3( 0.0f, -1.0f, 0.0f ) ) ) / dot( toCamera, float3( 0.0f, -1.0f, 0.0f ) );

	float3 waterWorldSample	= worldSample + toCamera * t;

	float3 waterNormal	= float3( 0.0f, 1.0f, 0.0f );
	float3 waterTangent	= float3( 0.71f, 0.0f, -0.71f );

	float4 waterSample = waterNormalMap.Sample( linearSampler, float2( waterWorldSample.x * 0.1f, waterWorldSample.z * 0.1f ) + offset );

	float waterLevel = -0.5f - waterSample.y;

	if( worldSample.y < waterLevel )
	{
		float4 bumpNormal	= waterSample;
		bumpNormal			= ( 2.0f * bumpNormal ) - 1.0f;
		waterTangent		= normalize( waterTangent - dot( waterTangent, waterNormal ) * waterNormal );
		float3 biTangent	= cross( waterNormal, waterTangent );
		float3x3 texSpace	= float3x3( waterTangent, biTangent, waterNormal );
		waterNormal			+= mul( bumpNormal.xyz, texSpace );

		//ShadowMap light
		lightDirection	= waterWorldSample - shadowCameraPosition.xyz;
		dShadow			= length( lightDirection );
		lightDirection			/= dShadow;

		//Calculate diffuse factor
		diffuseFactor	= saturate( dot( -lightDirection, normalSample ) );
		diffuse			= float3( diffuseFactor, diffuseFactor, diffuseFactor );

		//Calculate specular factor
		specularFactor	= saturate( dot( normalize( reflect( -lightDirection, waterNormal ) ), normalize( waterWorldSample - cameraPosition.xyz ) ) );
		specularPower		= 4.0f;
		specular			= float3( 1.0f, 1.0f, 1.0f ) * pow( specularFactor, specularPower );

		float3 waterColor	= ( ( ( ambient * ssao + diffuse + specular ) * float3( 0.4f, 0.3f, 0.6f ) ) * shadowFactor ) / ( dShadow * 0.01f + dShadow * dShadow * 0.005f );

		//Point lights
		for( int i = 0; i < numPointLights; i++ )
		{
			if( lightStructure[i].colorAndRadius.w > 0.01f )
			{
				float3 lightDir = worldSample - lightStructure[i].positionAndIntensity.xyz;
				float d			= length( lightDir );
				if( d < lightStructure[i].colorAndRadius.w * 2.0f )
				{
					lightDir		/= d;

					float diff	= saturate( dot( -lightDir, normalSample ) );
					float3 spec	= float3( lightStructure[i].colorAndRadius.xyz * pow( dot( reflect( lightDir, normalSample ), cameraPosition.xyz ), specularPower ) ) * specularSample;

					float denom			= d / lightStructure[i].colorAndRadius.w + 1.0f;
					float attenuation	= min( lerp( 1.0f, 0.0f, d / ( lightStructure[i].colorAndRadius.w * 2.0f ) ), 1.0f / ( denom * denom ) );

					waterColor += ( diffuse + specular ) * lightStructure[i].colorAndRadius.xyz * lightStructure[i].positionAndIntensity.w * attenuation;
				}
			}
		}

		saturate( waterColor );

		float fresnel	= max( 0.1f, dot( waterNormal, toCamera ) );

		float depthVar	= saturate( abs( worldSample.y + waterLevel ) * 0.2f );

		float3 depthCol	= ( 1.0f - depthVar ) + depthVar * float3( 0.2f, 0.2f, 0.15f );

		finalColor = albedoSample * ( finalColor * depthCol * ( 1.0f - fresnel ) + waterColor * depthCol * fresnel );
	}
	else
	{
		finalColor *= albedoSample;
	}

	if( abs( worldSample.y - waterLevel ) <= 0.25f )
	{
		float foamVar	= max( 0.0f, worldSample.y - waterLevel * 1.25f );
		foamVar			*= frac( timeVariable * albedoSample.x + worldSample.z );
		finalColor		= finalColor * ( 1.0f - foamVar ) + float3( 0.8f, 0.8f, 0.8f ) * foamVar;
	}

	//-------------------------------------------------------------------------------------------------
	//	COOL EFFECT SPONSORED BY JOHANNES ANDERSSON
	//-------------------------------------------------------------------------------------------------

	float3 originToPos	= worldSample - float3( 0.0f, 0.0f, 0.0f );
	float rangeCheck	= length( originToPos );

	if( rangeCheck > 40.0f )
	{
		float rangeInterpol;
		float3 test = fmod( worldSample + float3( 1000.0f, 0.0f, 1000.0f ), 0.5f);

		if( rangeCheck < 40.2f )
			rangeInterpol = 0.6f;
		else if( test.x * 0.5f + test.z * 0.5f > 0.17f && test.x * 0.5f + test.z * 0.5f < 0.28f )
		//else if ( test.x + test.z > 0.18f && test.x + test.z < 0.23f || abs( test.z ) + test.x > 0.1f && abs( test.z ) + test.x < 0.15f )
		//else if ( abs( test.x - test.z ) > 0.3f ) //|| abs( test.z ) > 0.1f && abs( test.z ) < 0.15f )
			rangeInterpol = 0.5f;
		else
			rangeInterpol = 0.2f;

		rangeInterpol *=  0.75f + sin( timeVariable * 2.0f ) * 0.25f;

		finalColor = ( 1.0f - rangeInterpol ) * finalColor + rangeInterpol * float4( 1.0f, 0.15f, 0.05f, 1.0f );
	}



	//return float4( albedoSample, 1.0f );
	//return float4( specularSample, specularSample, specularSample, 1.0f );
	//return float4( normalSample, 1.0f );
	//return float4( ssao, ssao, ssao, 1.0f );

	return float4( finalColor * ( shadowFactor * 0.4f + 0.6f ), 1.0f );
}