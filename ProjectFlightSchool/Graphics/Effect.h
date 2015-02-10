#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "EffectInfo.h"

#if !defined(SAFE_DELETE)
#define SAFE_DELETE( x ) if( x ){ delete x; x = nullptr; }
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY( x ) if( x ){ delete [] x; x = nullptr; }
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE( x ) if( x ){ x->Release(); x = nullptr; }
#endif

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
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		std::wstring stringToWstring( const std::string &s );

	public:
		ID3D11InputLayout*		GetInputLayout() const;
		ID3D11VertexShader*		GetVertexShader() const;
		ID3D11GeometryShader*	GetGeometryShader() const;
		ID3D11PixelShader*		GetPixelShader() const;

		HRESULT Intialize( ID3D11Device* device, EffectInfo* effectInfo );
		void Release();
		Effect();
		~Effect();
};
#endif