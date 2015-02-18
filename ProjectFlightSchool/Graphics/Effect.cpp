#include "Effect.h"

HRESULT Effect::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

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

std::wstring Effect::stringToWstring( const std::string &s )
{
	int stringLength = (int)s.length() + 1;
	int bufferLength = MultiByteToWideChar( CP_ACP, 0, s.c_str(), stringLength, 0, 0 );

	wchar_t* buffer = new wchar_t[bufferLength];
	MultiByteToWideChar( CP_ACP, 0, s.c_str(), stringLength, buffer, bufferLength );
	std::wstring wString( buffer );

	delete[] buffer;

	return wString;
}

ID3D11InputLayout*	Effect::GetInputLayout() const
{
	return mInputLayout;
}

ID3D11VertexShader*	Effect::GetVertexShader() const
{
	return mVertexShader;
}

ID3D11GeometryShader* Effect::GetGeometryShader() const
{
	return mGeometryShader;
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
		std::ifstream fileReader;
		std::string file( effectInfo->fileName );
		std::string checkFileExist = "../Content/Effects/CompiledShaders/" + file + "_VS.cso";
		fileReader.open( checkFileExist.c_str() );
		
		if( fileReader.is_open() )
		{
			D3DReadFileToBlob( stringToWstring( checkFileExist ).c_str(), &vertexShaderBlob );
			hr = device->CreateVertexShader( vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), 0, &mVertexShader );

			if ( FAILED( hr ) )
			{
				OutputDebugStringA( "Failed to load from file " );
				OutputDebugStringA( (LPCSTR)effectInfo->filePath );
				OutputDebugStringA( " at VertexShader stage" );
			}
		}
		else
		{
			

			if ( SUCCEEDED( hr = CompileShader( effectInfo->filePath, "VS_main", "vs_5_0", nullptr, &vertexShaderBlob ) ) )
			{
				if ( SUCCEEDED( hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					nullptr,
					&mVertexShader) ) )
				{
					CreateDirectory( L"../Content/Effects/CompiledShaders/", NULL );
					std::ofstream fileWriter( checkFileExist.c_str() );
					if( fileWriter.is_open() )
					{
						fileWriter << "";
						fileWriter.close();
					}
					
					D3DWriteBlobToFile( vertexShaderBlob, stringToWstring( checkFileExist ).c_str(), true );
				}
				
			}
		}

		if ( FAILED( hr ) )
		{
			OutputDebugStringA( "Failed to compile  " );
			OutputDebugStringA( (LPCSTR)effectInfo->filePath );
			OutputDebugStringA( " at VertexShader stage" );
		}
		else
		{
			if( effectInfo->vertexType == STATIC_VERTEX_TYPE )
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
			else if( effectInfo->vertexType == STATIC_INSTANCED_VERTEX_TYPE )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						//Vertex buffer data
						{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TANGENT"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEX"			, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						//Instance buffer data
						{ "WORLD"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
							
				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
			else if( effectInfo->vertexType == ANIMATED_VERTEX_TYPE )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
						{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
						{ "TANGENT"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
						{ "TEX"			, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
						{ "WEIGHTS"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
						{ "JOINTINDEX"	, 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
			else if( effectInfo->vertexType == ANIMATED_VERTEX_INSTANCED_TYPE )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						//Vertex buffer data
						{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TANGENT"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEX"			, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "WEIGHTS"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "JOINTINDEX"	, 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						//Instance buffer data
						{ "WORLD"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WORLD"		, 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "INSTANCEID"	, 0, DXGI_FORMAT_R32_UINT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
			else if( effectInfo->vertexType == BILLBOARD_VERTEX_TYPE )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "WORLDPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "WIDTH", 0, DXGI_FORMAT_R32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "HEIGHT", 0, DXGI_FORMAT_R32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
			else if( effectInfo->vertexType == PARTICLE_VERTEX_TYPE )
			{
				D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "WORLDPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
						{ "TIMETILLDEATH", 0, DXGI_FORMAT_R32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

				};
				hr = device->CreateInputLayout( inputDesc,
					ARRAYSIZE( inputDesc ),
					vertexShaderBlob->GetBufferPointer(),
					vertexShaderBlob->GetBufferSize(),
					&mInputLayout );
			}
		}
		SAFE_RELEASE( vertexShaderBlob );
		fileReader.close();
	}

	if ( effectInfo->isGeometryShaderIncluded )
	{
		//--------------------------
		// Compile Geometry Shader | 
		//--------------------------
		ID3DBlob* geometryShaderBlob = nullptr;
		std::ifstream fileReader;
		std::string file( effectInfo->fileName );
		std::string checkFileExist = "../Content/Effects/CompiledShaders/" + file + "_GS.cso";
		fileReader.open( checkFileExist.c_str() );
		
		if( fileReader.is_open() )
		{
			D3DReadFileToBlob( stringToWstring( checkFileExist ).c_str(), &geometryShaderBlob );
			hr = device->CreateGeometryShader( geometryShaderBlob->GetBufferPointer(), geometryShaderBlob->GetBufferSize(), 0, &mGeometryShader );

			if ( FAILED( hr ) )
			{
				OutputDebugStringA( "Failed to load from file " );
				OutputDebugStringA( (LPCSTR)effectInfo->filePath );
				OutputDebugStringA( " at GeometryShader stage" );
			}
		}
		else 
		{
			if( SUCCEEDED( hr = CompileShader( effectInfo->filePath, "GS_main", "gs_5_0", nullptr, &geometryShaderBlob ) ) )
			{
				hr = device->CreateGeometryShader( geometryShaderBlob->GetBufferPointer(),
					geometryShaderBlob->GetBufferSize(),
					nullptr,
					&mGeometryShader );

			
			}

			if ( FAILED( hr ) )
			{
				OutputDebugStringA( "Failed to compile " );
				OutputDebugStringA( (LPCSTR)effectInfo->filePath );
				OutputDebugStringA( " at GeometryShader stage" );
			}
			else
			{
				CreateDirectory( L"../Content/Effects/CompiledShaders/", NULL );
				std::ofstream fileWriter( checkFileExist.c_str() );
				if( fileWriter.is_open() )
				{
					fileWriter << "";
					fileWriter.close();
				}
				D3DWriteBlobToFile( geometryShaderBlob, stringToWstring( checkFileExist ).c_str(), true );
			}
		}

		SAFE_RELEASE( geometryShaderBlob );
		fileReader.close();
	}
	

	if ( effectInfo->isPixelShaderIncluded )
	{
		//-----------------------
		// Compile Pixel Shader | 
		//-----------------------
		ID3DBlob* pixelShaderBlob = nullptr;
		std::ifstream fileReader;
		std::string file( effectInfo->fileName );
		std::string checkFileExist = "../Content/Effects/CompiledShaders/" + file + "_PS.cso";
		fileReader.open( checkFileExist.c_str() );
		
		if( fileReader.is_open() )
		{
			D3DReadFileToBlob( stringToWstring( checkFileExist ).c_str(), &pixelShaderBlob );
			hr = device->CreatePixelShader( pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), 0, &mPixelShader );

			if ( FAILED( hr ) )
			{
				OutputDebugStringA( "Failed to load from file " );
				OutputDebugStringA( (LPCSTR)effectInfo->filePath );
				OutputDebugStringA( " at PixelShader stage" );
			}
		}
		else 
		{
			if( SUCCEEDED( hr = CompileShader( effectInfo->filePath, "PS_main", "ps_5_0", nullptr, &pixelShaderBlob ) ) )
			{
				hr = device->CreatePixelShader( pixelShaderBlob->GetBufferPointer(),
					pixelShaderBlob->GetBufferSize(),
					nullptr,
					&mPixelShader );

			
			}

			if ( FAILED( hr ) )
			{
				OutputDebugStringA( "Failed to compile " );
				OutputDebugStringA( (LPCSTR)effectInfo->filePath );
				OutputDebugStringA( " at PixelShader stage" );
			}
			else
			{
				CreateDirectory( L"../Content/Effects/CompiledShaders/", NULL );
				std::ofstream fileWriter( checkFileExist.c_str() );
				if( fileWriter.is_open() )
				{
					fileWriter << "";
					fileWriter.close();
				}
				D3DWriteBlobToFile( pixelShaderBlob, stringToWstring( checkFileExist ).c_str(), true );
			}
		}

		SAFE_RELEASE( pixelShaderBlob );
		fileReader.close();
	}

	return hr;
}

void Effect::Release()
{
	SAFE_RELEASE( mInputLayout );
	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mHullShader );
	SAFE_RELEASE( mDomainShader );
	SAFE_RELEASE( mGeometryShader );
	SAFE_RELEASE( mPixelShader ); 
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
