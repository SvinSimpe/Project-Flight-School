#include "Effect.h"

HRESULT Effect::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS;
	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );
	if ( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( (unsigned int)in.tellg() );
		in.seekg( 0, std::ios::beg);
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompile( shader_code.data(),
		shader_code.size(),
		NULL,
		pDefines,
		nullptr,
		pEntrypoint,
		pTarget,
		dwShaderFlags,
		NULL,
		pCompiledShader,
		&pErrorBlob );
	if ( pErrorBlob )
	{
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		SAFE_RELEASE( pErrorBlob );
	}
	return hr;
}

ID3D11InputLayout*	Effect::GetInputLayout() const
{
	return mInputLayout;
}

ID3D11VertexShader*	Effect::GetVertexShader() const
{
	return mVertexShader;
}

ID3D11PixelShader*	Effect::GetPixelShader() const
{
	return mPixelShader;
}

HRESULT Effect::Intialize( ID3D11Device* device, EffectInfo* effectInfo )
{
	HRESULT hr = S_OK;

	if ( effectInfo->fileName == "" || effectInfo->IsAllNull() )
	{
		OutputDebugStringA( "EffectInfo was missing vital information, resetting effect struct..." );
		effectInfo->Reset();
		hr = E_FAIL;
	}

	if ( effectInfo->isVertexShaderIncluded )
	{
		//------------------------
		// Compile Vertex Shader |
		//------------------------
		ID3DBlob* vertexShaderBlob = nullptr;
		if ( SUCCEEDED( hr = CompileShader( effectInfo->fileName, "VS_main", "vs_5_0", nullptr, &vertexShaderBlob ) ) )
		{
			if ( SUCCEEDED( hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
				vertexShaderBlob->GetBufferSize(),
				nullptr,
				&mVertexShader) ) )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEX"		, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
			SAFE_RELEASE( vertexShaderBlob );
		}

		if ( FAILED( hr ) )
		{
			OutputDebugStringA( "Failed to compile  " );
			OutputDebugStringA( (LPCSTR)effectInfo->fileName );
			OutputDebugStringA( " at VertexShader stage" );
		}
	}



	if ( effectInfo->isPixelShaderIncluded )
	{
		//-----------------------
		// Compile Pixel Shader | 
		//-----------------------
		ID3DBlob* pixelShaderBlob = nullptr;
		if ( SUCCEEDED( hr = CompileShader(effectInfo->fileName, "PS_main", "ps_5_0", nullptr, &pixelShaderBlob ) ) )
		{
			hr = device->CreatePixelShader( pixelShaderBlob->GetBufferPointer(),
				pixelShaderBlob->GetBufferSize(),
				nullptr,
				&mPixelShader );

			SAFE_RELEASE( pixelShaderBlob );
		}

		if ( FAILED( hr ) )
		{
			OutputDebugStringA( "Failed to compile " );
			OutputDebugStringA( (LPCSTR)effectInfo->fileName );
			OutputDebugStringA( " at PixelShader stage" );
		}
	}

	return hr;
}

void Effect::Release()
{
	SAFE_RELEASE( mInputLayout );
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mHullShader);
	SAFE_RELEASE(mDomainShader);
	SAFE_RELEASE(mGeometryShader);
	SAFE_RELEASE(mPixelShader); 
}

Effect::Effect()
{
	mInputLayout	= nullptr;
	mVertexShader	= nullptr;
	mHullShader		= nullptr;
	mDomainShader	= nullptr;
	mGeometryShader = nullptr;
	mPixelShader	= nullptr;
}

Effect::~Effect()
{}