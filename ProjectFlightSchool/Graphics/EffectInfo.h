#ifndef _EFFECTINFO_H_
#define _EFFECTINFO_H_

#define STATIC_VERTEX_TYPE 0
#define ANIMATED_VERTEX_TYPE 1
#define ANIMATED_INSTANCED_VERTEX_TYPE 2

struct EffectInfo
{
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