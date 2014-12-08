#ifndef ASSETBASE_H
#define ASSETBASE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; ( x ) = nullptr; }

typedef UINT AssetID;

struct AssetBase
{
	private:
	protected:
	public:
		AssetID				mAssetId;
		std::string			mFileName;

	private:
	protected:
	public:
		std::string			GetFileName();
		virtual	HRESULT		Initialize()	= 0;
		virtual void		Release()		= 0;
							AssetBase();
		virtual				~AssetBase();

};
#endif