#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <Graphics.h>
#include "RenderManager.h"
#include "Input.h"

#define MAX_AMOUNT_OF_HEALTBAR_ASSETS 85

struct HealthBars
{
	UINT				mAssetID;
	DirectX::XMFLOAT2	mTopLeftCorner;
	DirectX::XMFLOAT2	mSize;
};

class HealthBar
{

	private:
		HealthBars				mHealthBars[MAX_AMOUNT_OF_HEALTBAR_ASSETS];
		UINT					mNrOfHealthAssetsToRender;

		AssetID					mAllyHealth;
		DirectX::XMFLOAT2		mSizeAllyHealth;
		float					mSpaceAllyHealth;
		float					mSpaceAllyHealthToBar;
		int						mNrOfHealths;

		DirectX::XMFLOAT2		mAllyHealthOffset;

		AssetID					mAllyHealthBar;
		DirectX::XMFLOAT2		mSizeAllyHealthBar;
		float					mSpaceAllyHealthBar;
		float					mSpaceAllyHealthBarToEdge;

		AssetID					mAllyHealthFrame;
		DirectX::XMFLOAT2		mSizeAllyHealthFrame;

		AssetID					mShipHealth;
		XMFLOAT2				mSizeShipHealth;
		float					mSizeShipHealthTop;
		float					mSpaceShipHealth;
		XMFLOAT2				mStartShipHealth;
		XMFLOAT2				mEndShipHealth;

		AssetID					mShipHealthBar;
		XMFLOAT2				mSizeShipHealthBar;

		DirectX::XMFLOAT2		mShipHealthOffset;

	protected:
	public:

	private:
	protected:
	public:
		HRESULT	Update( int nrOfAllies, float alliesHP[], float shipHP );
		HRESULT	Render();
		HRESULT	Initialize();
		void	Release();
				HealthBar();
		virtual ~HealthBar();
};

#endif