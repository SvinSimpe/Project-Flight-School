#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <fstream>

struct EffectInfo
{
	char* fileName					= "";
	bool isVertexShaderIncluded		= false;
	bool isHullShaderIncluded		= false;
	bool isDomainShaderIncluded		= false;
	bool isGeometryShaderIncluded	= false;
	bool isPixelShaderIncluded		= false;

	//Reset all struct variables
	void Reset()
	{
		fileName = "";
		isVertexShaderIncluded		= false;
		isHullShaderIncluded		= false;
		isDomainShaderIncluded		= false;
		isGeometryShaderIncluded	= false;
		isPixelShaderIncluded		= false;
	}
};

class Effect
{
	private:	
		ID3D11InputLayout*		mInputLayout;
		ID3D11VertexShader*		mVertexShader;
		ID3D11HullShader*		mHullShader;
		ID3D11DomainShader*		mDomainShader;
		ID3D11GeometryShader*	mGeometryShader;
		ID3D11PixelShader*		mPixelShader;

	private:
		HRESULT CompileShader(char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader);

	public:
		HRESULT Update( float deltaTime );
		HRESULT Render(float deltaTime);

		
		HRESULT Intialize();
		void Release();
		Effect( ID3D11Device* device, EffectInfo effectInfo );
		~Effect();	
};
#endif