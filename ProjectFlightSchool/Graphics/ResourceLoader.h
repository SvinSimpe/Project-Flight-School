#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H
#include <string>
#include <d3d11.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

class ResourceLoader
{
private:
	void ToLower( std::string &in );
	std::string ToDDS( std::string fileName );
	bool FileExists( std::string fileName );
	std::wstring StringToWstring( std::string fileName );
	std::string FindFileExtension( const std::string &str );
	HRESULT LoadDDS( ID3D11Device* device, ID3D11DeviceContext* dc, const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size = 0 );
	HRESULT LoadWIC( ID3D11Device* device, ID3D11DeviceContext* dc, const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size = 0 );
public:
	ResourceLoader();
	ResourceLoader( const ResourceLoader& );
	~ResourceLoader();
	HRESULT LoadResource( ID3D11Device* device, ID3D11DeviceContext* dc, std::string fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size = 0 );
};
#endif