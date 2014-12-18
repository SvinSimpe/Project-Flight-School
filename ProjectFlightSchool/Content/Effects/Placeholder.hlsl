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
	float2 uv				: TEX;
};

VS_Out VS_main( VS_In input )
{
	VS_Out output			= (VS_Out)0;
	output.position			= mul( float4( input.position, 1.0f ), worldMatrix );
	output.worldPosition	= output.position.xyz;
	output.position			= mul( output.position, viewMatrix );
	output.position			= mul( output.position, projectionMatrix );

	output.normal	= mul( input.normal.xyz, (float3x3)worldMatrix );

	output.uv		= input.uv;

	return output;
}

//Pixel
Texture2D<float4> diffuseTexture	: register( t0 );
Texture2D<float4> normalTexture		: register( t1 );
Texture2D<float4> specularTexture	: register( t2 );
SamplerState pointSampler			: register( s0 );

float4 PS_main( VS_Out input ) : SV_TARGET0
{	
	//======== PHONG SHADING ===========
	float3 ambient		= float3( 0.1f, 0.1f,  0.1f );
	float3 diffuse		= float3( 0.0f, 0.0f,  0.0f );
	float3 specular		= float3( 0.1f, 0.1f,  0.1f );
	float3 color		= float3( 1.0f, 1.0f,  1.0f );

	float3 lightDirection	= float3( -0.5f, -1.0f, 0.3f );

	//Calculate diffuse factor
	float diffuseFactor = max( 0, dot( input.normal, -lightDirection ) );
	diffuse				= color * diffuseFactor;

	//Calculate specular factor
	float3 reflection		= normalize( reflect( -lightDirection, input.normal ) );
	float3 viewVector		= normalize( cameraPosition - input.position ).xyz;
	float specularFactor	= max( 0, dot( reflection, viewVector ) );
	float specularPower		= 64.0f;
	specular				= float3( color * pow( specularFactor, specularPower ) );

	float3 finalColor		= float3( ( ambient + diffuse + specular ) * color );
	return float4( finalColor *	diffuseTexture.Sample( pointSampler, input.uv ).xyz, 1.0f );

}