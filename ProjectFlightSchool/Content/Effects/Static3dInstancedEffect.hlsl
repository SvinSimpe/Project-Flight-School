//Vertex
cbuffer CbufferPerFrame	: register( b0 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4	 cameraPosition;
}

struct VS_In
{
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float2 uv				: TEX;
	float4x4 worldMatrix	: WORLD;
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
	output.position			= mul( float4( input.position, 1.0f ), input.worldMatrix );
	output.worldPosition	= output.position.xyz;
	output.position			= mul( output.position, viewMatrix );
	output.position			= mul( output.position, projectionMatrix );

	output.normal	= mul( input.normal, (float3x3)input.worldMatrix );
	output.tangent	= mul( input.tangent, (float3x3)input.worldMatrix );

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

Texture2D<float4> diffuseTexture	: register( t0 );
Texture2D<float4> normalTexture		: register( t1 );
Texture2D<float4> specularTexture	: register( t2 );
SamplerState pointSampler			: register( s0 );
SamplerState linearSampler			: register( s1 );
PS_Out PS_main( VS_Out input )
{	
	PS_Out output = (PS_Out)0;

	clip ( diffuseTexture.Sample( linearSampler, input.uv ).w < 0.7f ? -1:1 );

	//======== NORMAL MAPPING ==========
	float4 bumpNormal	= normalTexture.Sample( linearSampler, input.uv.xy );
	bumpNormal			= ( 2.0f * bumpNormal ) - 1.0f;
	input.tangent		= normalize( input.tangent - dot( input.tangent, input.normal ) * input.normal );
	float3 biTangent	= cross( input.normal.xyz, input.tangent.xyz );
	float3x3 texSpace	= float3x3( input.tangent.xyz, biTangent, input.normal.xyz );
	input.normal		+= mul( bumpNormal.xyz, texSpace );
	
	output.normal			= float4( normalize( input.normal ), 0.0f );
	output.albedoSpec		= float4( diffuseTexture.Sample( linearSampler, input.uv ).xyz, specularTexture.Sample( linearSampler, input.uv ).x );
	output.worldPosition	= float4( input.worldPosition, 1.0f );
	return output;
}