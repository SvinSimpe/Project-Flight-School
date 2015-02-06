//Vertex
static const int MAX_INSTANCE = 32;
struct PerInstanceData
{
	float4x4 boneTransforms[32];
};

cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4	 cameraPosition;
}

cbuffer CbufferPerObject : register( b1 )
{
	PerInstanceData perInstance[MAX_INSTANCE];
}

cbuffer CbufferPerFrameShadow	: register( b2 )
{
	float4x4	viewMatrix;
	float4x4	projectionMatrix;
	float4		cameraPosition;
}

struct VS_In
{
	float3			position	: POSITION;
	float3			normal		: NORMAL;
	float3			tangent		: TANGENT;
	float2			uv			: TEX;
	float4			weights		: WEIGHTS;
	uint4			jointIndex	: JOINTINDEX;
	float4x4		worldMatrix	: WORLD;
	unsigned int	instanceID	: SV_InstanceID;

};

struct VS_Out
{
	float4 position			: SV_POSITION;
	float2 uv				: TEX;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output	= (VS_Out)0;
	
	float3 transformedPosition	= float3( 0.0f, 0.0f, 0.0f );
	float3 transformedNormal	= float3( 0.0f, 0.0f, 0.0f );
	float3 transformedTangent	= float3( 0.0f, 0.0f, 0.0f );
	[unroll]
	for( int i = 0; i < 4; i++ )
	{
		transformedPosition += mul( float4( input.position, 1.0f ), perInstance[input.instanceID].boneTransforms[input.jointIndex[i]] ).xyz * input.weights[i];
		transformedNormal	+= mul( input.normal, (float3x3)perInstance[input.instanceID].boneTransforms[input.jointIndex[i]] ) * input.weights[i];
		transformedTangent	+= mul( input.tangent, (float3x3)perInstance[input.instanceID].boneTransforms[input.jointIndex[i]] ) * input.weights[i];
	}
	
	output.position			= mul( float4( transformedPosition, 1.0f ), input.worldMatrix );
	output.position			= mul( output.position, LightViewMatrix );
	output.position			= mul( output.position, LightProjectionMatrix );

	output.uv		= input.uv;

	return output;
}

//Pixel

Texture2D<float4> diffuseTexture	: register( t0 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
void PS_main( VS_Out input )
{	
	clip ( diffuseTexture.Sample( linearSampler, input.uv ).w < 0.7f ? -1:1 );
}