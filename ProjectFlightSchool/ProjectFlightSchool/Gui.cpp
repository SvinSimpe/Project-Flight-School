#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects )
{
	HRESULT result = mRadar->Update( playerPos, radarObjects, nrOfObjects );
	return result;
}

HRESULT Gui::Render( int nrOfAllys, float allysHP[] )
{
	HRESULT result = mRadar->Render();
	XMFLOAT2 topLeft;
	XMFLOAT2 widthHeight;


	//////////////////////
	//Allys hp
	//////////////////////
	XMFLOAT2	latestAllyHealthTopLeft;
	XMFLOAT2	latestAllyHealthBottomRight;
	float		offSetForHealths = mSpaceAllyHealthToBar;
	int			nrOfHealthsToRender;
	for (int i = 0; i < nrOfAllys; i++)
	{
		topLeft.x						= mScreenWidth - mSpaceAllyHealthBarToEdge - mSizeAllyHealthBar.x;
		topLeft.y						= mScreenHeight - mSpaceAllyHealthBarToEdge - mSizeAllyHealthBar.y;
		widthHeight						= mSizeAllyHealthBar;
		latestAllyHealthTopLeft			= topLeft;
		latestAllyHealthBottomRight.x	= topLeft.x + mSizeAllyHealthBar.x;
		latestAllyHealthBottomRight.y	= topLeft.y + mSizeAllyHealthBar.y;
		RenderManager::GetInstance()->AddObject2dToList(mAllyHealthBar, topLeft, widthHeight);

		nrOfHealthsToRender = (20.0f * allysHP[i]);

		if (nrOfHealthsToRender > 0)
		{
			for (int j = 0; j < nrOfHealthsToRender; j++)
			{

				topLeft.x = latestAllyHealthBottomRight.x - offSetForHealths - mSizeAllyHealth.x;
				topLeft.y = latestAllyHealthBottomRight.y - mSpaceAllyHealthToBar - mSizeAllyHealth.y;
				widthHeight = mSizeAllyHealth;
				RenderManager::GetInstance()->AddObject2dToList(mAllyHealth, topLeft, widthHeight);
				offSetForHealths += mSpaceAllyHealth + mSizeAllyHealth.x;
			}
		}
	}

	if (nrOfAllys > 0)
	{
		topLeft.x = latestAllyHealthTopLeft.x - mSpaceAllyHealthBar - 4.0f;
		topLeft.y = latestAllyHealthTopLeft.y - mSpaceAllyHealthBar - 4.0f;
		widthHeight = mSizeAllyHealthFrame;
		RenderManager::GetInstance()->AddObject2dToList(mAllyHealthFrame, topLeft, widthHeight);
	}

	return result;
}

HRESULT Gui::Initialize()
{
	mRadar = new Radar();
	HRESULT result	= mRadar->Initialize();
	if( FAILED( result ) )
	{
		return result;
	}
	mScreenHeight						= Input::GetInstance()->mScreenHeight;
	mScreenWidth						= Input::GetInstance()->mScreenWidth;
	mNrOfHealths						= 20;
	mSpaceAllyHealthToBar				= 6.0f;
	mSpaceAllyHealth					= 2.0f;
	mSpaceAllyHealthBar					= 15.0f;
	mSpaceAllyHealthBarToEdge			= 27.0f;
	mSizeAllyHealthBar					= XMFLOAT2( 215.0f, 40.0f );
	mSizeAllyHealth						= XMFLOAT2( ( ( mSizeAllyHealthBar.x - ( mSpaceAllyHealthToBar * 2 ) - ( ( mNrOfHealths ) * mSpaceAllyHealth ) ) / mNrOfHealths ), ( mSizeAllyHealthBar.y - ( mSpaceAllyHealthToBar * 2 ) ) );
	mSizeAllyHealthFrame				= XMFLOAT2( 110.0f, 110.0f );

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyhealth.dds", mAllyHealth );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthBar.dds", mAllyHealthBar );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthFrame.dds", mAllyHealthFrame );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/playerHealth+XP.dds", mPlayerBar );
	if( FAILED( result ) )
	{
		return result;
	}

	return result;
}

void Gui::Release()
{
	mRadar->Release();
	SAFE_DELETE( mRadar );
}

Gui::Gui()
{
	mRadar				= nullptr;
}

Gui::~Gui()
{

}