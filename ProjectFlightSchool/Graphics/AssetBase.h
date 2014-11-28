#ifndef ASSETBASE_H
#define ASSETBASE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; ( x ) = nullptr; }

struct AssetBase
{
	private:
	protected:
	public:
		UINT				mAssetId;
		std::string			mFileName;

		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		ID3D11Buffer*		mIndexBUffer;

	private:
	protected:
	public:
		std::string		GetFileName();
		UINT			GetVertexCount();
		HRESULT			Initialize();
		void			Release();
						AssetBase();
		virtual			~AssetBase();

};
#endif