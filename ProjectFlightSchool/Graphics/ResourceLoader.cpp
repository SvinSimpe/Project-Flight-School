#include "ResourceLoader.h"
#include <algorithm>
#include <sys/stat.h>
bool ResourceLoader::FileExists( std::string fileName )
{
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
std::wstring ResourceLoader::StringToWstring( std::string fileName )
{
	return std::wstring( fileName.begin(), fileName.end() );
}
std::string ResourceLoader::FindFileExtension( const std::string &str )
{
	std::string result = "N/A";
	int found = str.find_last_of('.');
	if( found != -1)
	{
		result = str.substr( found + 1 );
	}
	return result;
}
std::string ResourceLoader::ToDDS( std::string fileName )
{
	std::string result = "N/A";
	int found = fileName.find_last_of('.');
	if( found != -1)
	{
		result = fileName.substr( 0, found + 1 ) + "dds";
	}
	return result;
}
void ResourceLoader::ToLower( std::string &in )
{
	std::transform( in.begin(), in.end(), in.begin(), ::tolower );
}
HRESULT ResourceLoader::LoadDDS( ID3D11Device* device, ID3D11DeviceContext* dc, const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size )
{
	return DirectX::CreateDDSTextureFromFile( device, dc, fileName, texture, srv );
}
HRESULT ResourceLoader::LoadWIC( ID3D11Device* device, ID3D11DeviceContext* dc, const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size )
{
	return CreateWICTextureFromFile( device, dc, fileName, texture, srv );
}
HRESULT ResourceLoader::LoadResource( ID3D11Device* device, ID3D11DeviceContext* dc, std::string fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size )
{
	std::string fileExt = FindFileExtension( fileName );

	ToLower( fileExt );
	HRESULT hr = S_OK;

	if( fileExt == "dds" || fileExt == "DDS"  )
	{
		hr = LoadDDS( device, dc, StringToWstring( fileName ).c_str(), texture, srv );
	}
	else
	{
		std::string fileDDS = ToDDS( fileName );
		if( FileExists( fileDDS ) )
		{
			hr = LoadDDS( device, dc, StringToWstring( fileDDS ).c_str(), texture, srv );
		}
		else
		{
			hr = LoadWIC( device, dc, StringToWstring( fileName ).c_str(), texture, srv );
		}
	}
	return hr;
}
ResourceLoader::ResourceLoader()
{
}
ResourceLoader::~ResourceLoader()
{
}