#ifndef ASSETBASE_H
#define ASSETBASE_H

#include <d3d11.h>
#include <DirectXMath.h>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; ( x ) = nullptr; }

struct AssetBase
{
	private:
	protected:
	public:
		UINT				mAssetId;

		ID3D11Buffer*		mVertexBuffer;
		ID3D11Buffer*		mIndexBUffer;

		//DirectX::XMMATRIX	mWorld;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
				AssetBase();
		virtual		~AssetBase();

};
#endif