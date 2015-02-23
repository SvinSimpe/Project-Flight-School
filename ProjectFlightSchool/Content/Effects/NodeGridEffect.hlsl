//Vertex
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4	 cameraPosition;
}

cbuffer CbufferPerObject : register( b1 )
{
	float4x4 worldMatrix;
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
	float4 position			: SV_POSITION;
	float3 worldPosition	: WORLD_POSITION;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float2 uv				: TEX;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output			= (VS_Out)0;
	output.position			= mul( float4( input.position, 1.0f ), worldMatrix );
	output.worldPosition	= output.position.xyz;
	output.position			= mul( output.position, viewMatrix );
	output.position			= mul( output.position, projectionMatrix );

	output.normal	= mul( input.normal, (float3x3)worldMatrix );
	output.tangent	= mul( input.tangent, (float3x3)worldMatrix );

	output.uv		= input.uv;

	return output;
}

//Pixel

struct PS_Out
{
	float4 albedoSpec		: SV_Target0;
	float4 normal			: SV_Target1;
	float4 worldPosition	: SV_Target2;
};

Texture2D<float4> mudBlendMap		: register( t0 );
Texture2D<float4> grassBlendMap		: register( t1 );
Texture2D<float4> sandBlendMap		: register( t2 );
Texture2D<float4> blendMapWeights	: register( t3 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
PS_Out PS_main( VS_Out input )
{	
	PS_Out output = (PS_Out)0;
	
	float2 mapUv = float2( frac( input.worldPosition.x * 0.058f ), frac( input.worldPosition.z * 0.058f ) );

	float3 weights = normalize( blendMapWeights.Sample( linearSampler, input.uv ).xyz );
	float3 diffuse = mudBlendMap.Sample( linearSampler, mapUv ).xyz * weights.x;
	diffuse		  += grassBlendMap.Sample( linearSampler, mapUv ).xyz * weights.y;
	diffuse		  += sandBlendMap.Sample( linearSampler, mapUv ).xyz * weights.z;

	output.normal			= float4( normalize( input.normal ), 0.0f );
	output.albedoSpec		= float4( diffuse, 0.0f );
	output.worldPosition	= float4( input.worldPosition, 1.0f );
	return output;
}