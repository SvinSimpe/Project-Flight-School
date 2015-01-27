#ifndef _EFFECTINFO_H_
#define _EFFECTINFO_H_

#define STATIC_VERTEX_TYPE 0
#define ANIMATED_VERTEX_TYPE 1
#define ANIMATED_VERTEX_INSTANCED_TYPE 2
#define STATIC_INSTANCED_VERTEX_TYPE 3
#define BILLBOARD_VERTEX_TYPE 4

struct EffectInfo
{
	char*	filePath					= "";
	char*	fileName					= "";
	UINT	vertexType					= 0;
	bool	isVertexShaderIncluded		= false;
	bool	isHullShaderIncluded		= false;
	bool	isDomainShaderIncluded		= false;
	bool	isGeometryShaderIncluded	= false;
	bool	isPixelShaderIncluded		= false;

	//Reset ALL struct variables
	void Reset()
	{
		filePath					= "";
		fileName					= "";
		vertexType					= 0;
		isVertexShaderIncluded		= false;
		isHullShaderIncluded		= false;
		isDomainShaderIncluded		= false;
		isGeometryShaderIncluded	= false;
		isPixelShaderIncluded		= false;
	}

	//Returns TRUE all shader flags are OFF
	bool IsAllNull() const
	{
		return ( !isVertexShaderIncluded	&&
				 !isHullShaderIncluded		&&
				 !isDomainShaderIncluded	&&
				 !isGeometryShaderIncluded	&&
				 !isPixelShaderIncluded );
	}
};

#endif