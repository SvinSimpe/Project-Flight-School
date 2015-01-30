//cbuffer CB_PER_FRAME : register(b0)
//{
//	float4x4	viewProjection;
//	float3		cameraPosition;
//};
//
//cbuffer CB_PER_OBJECT : register(b0)
//{
//	float4x4	world;
//};
//
//struct VSIN
//{
//	float3	position : POSITION;
//	float	lifeTime : LIFE_TIME;
//};
//
//struct GSIN
//{
//	float4	position : POSITION;
//	float	lifeTime : LIFE_TIME;
//};
//
//struct PSIN
//{
//	float4	position : SV_POSITION;
//	float	lifeTime : LIFE_TIME;
//};
//
//
//
//GSIN VS_MAIN( VSIN input )
//{
//	GSIN output = (GSIN)0;
//
//	output.position = float4( input.position, 1.0f );
//
//	return output;
//}
//
//[maxvertexcount(4)]
//void GS_MAIN( point GSIN input[1], inout TriangleStream<PSIN> tStream )
//{
//	PSIN output = (PSIN)0;
//
//	float3 camToParticle = normalize( input[0].position.xyz - cameraPosition );
//	float3 right		 = normalize( cross( float3( 0.0f, 1.0f, 0.0f ), camToParticle ) );
//	float3 up			 = normalize( cross( camToParticle, right ) );
//
//	float particleSize = 0.2f;
//
//	for( int i = 0; i < 4; i++ )
//	{
//		output.position = input[0].position;
//		if( i == 0 )
//			output.position += float4( -right - up, 0.0f ) * particleSize;
//
//		else if( i == 1 )
//			output.position += float4( -right + up, 0.0f ) * particleSize;
//
//		else if( i == 2 )
//			output.position += float4(  right - up, 0.0f ) * particleSize;
//		
//		else
//			output.position += float4(  right + up, 0.0f ) * particleSize;
//
//		output.position = mul( output.position, world );
//		output.position = mul( output.position, viewProjection );
//		//output.lifeTime = input.lifeTime;
//
//
//		tStream.Append( temp );
//	}
//}
//
//
//float4 PS_MAIN( GSIN input ) : SV_Target
//{
//	return float4( 1.0f, 1.0f, 1.0f, input.lifeTime );
//}