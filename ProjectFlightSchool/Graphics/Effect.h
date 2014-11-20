#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "EffectInfo.h"

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }

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

	public:
		ID3D11InputLayout*	GetInputLayout() const;
		ID3D11VertexShader*	GetVertexShader() const;
		ID3D11PixelShader*	GetPixelShader() const;

		HRESULT Intialize( ID3D11Device* device, EffectInfo* effectInfo );
		void Release();
		Effect();
		~Effect();
};
#endif