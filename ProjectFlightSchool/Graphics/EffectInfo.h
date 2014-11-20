#ifndef _EFFECTINFO_H_
#define _EFFECTINFO_H_

struct EffectInfo
{
	char* fileName					= "";
	bool isVertexShaderIncluded		= false;
	bool isHullShaderIncluded		= false;
	bool isDomainShaderIncluded		= false;
	bool isGeometryShaderIncluded	= false;
	bool isPixelShaderIncluded		= false;

	//Reset ALL struct variables
	void Reset()
	{
		fileName					= "";
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