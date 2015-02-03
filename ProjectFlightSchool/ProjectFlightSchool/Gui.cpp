#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects )
{
	HRESULT result = mRadar->Update( playerPos, radarObjects, nrOfObjects );
	return result;
}

HRESULT Gui::Render()
{
	HRESULT result = mRadar->Render();
	
	int			nrOfHealths;
	XMFLOAT2	sizeAllyHealth;
	XMFLOAT2	sizeAllyHealthBar;
	XMFLOAT2	sizeAllyHealthFrame;
	float		spaceAllyHealthBar;
	float		spaceAllyHealth;
	float		spaceAllyHealthBarToEdge;
	float		SpaceAllyHealthToBar;

	nrOfHealths							= 20;
	SpaceAllyHealthToBar					= 4.0f;
	sizeAllyHealthBar					= XMFLOAT2( 215.0f, 40.0f );
	sizeAllyHealth						= XMFLOAT2( ( ( sizeAllyHealthBar.x - ( SpaceAllyHealthToBar * 2 ) ) / nrOfHealths ), ( sizeAllyHealthBar.y - ( SpaceAllyHealthToBar * 2 ) ) );
	sizeAllyHealthFrame					= XMFLOAT2( 110.0f, 110.0f );
	spaceAllyHealth						= 1.0f;
	spaceAllyHealthBar					= 15.0f;
	spaceAllyHealthBarToEdge			= 27.0f;

	XMFLOAT2 lowestAllyHealthTopLeft;

	XMFLOAT2 topLeft;
	XMFLOAT2 widthHeight;
	topLeft.x					= mScreenWidth - spaceAllyHealthBarToEdge - sizeAllyHealthBar.x;
	topLeft.y					= mScreenHeight - spaceAllyHealthBarToEdge - sizeAllyHealthBar.y;
	widthHeight					= sizeAllyHealthBar;
	lowestAllyHealthTopLeft		= topLeft;

	RenderManager::GetInstance()->AddObject2dToList( mAllyHealthBar, topLeft, widthHeight );

	topLeft.x		= lowestAllyHealthTopLeft.x + SpaceAllyHealthToBar;
	topLeft.y		= lowestAllyHealthTopLeft.y + SpaceAllyHealthToBar;
	widthHeight		= sizeAllyHealth;

	RenderManager::GetInstance()->AddObject2dToList( mAllyHealth, topLeft, widthHeight );

	topLeft.x		= lowestAllyHealthTopLeft.x - spaceAllyHealthBar;
	topLeft.y		= lowestAllyHealthTopLeft.y - spaceAllyHealthBar;
	widthHeight		= sizeAllyHealthFrame;

	RenderManager::GetInstance()->AddObject2dToList( mAllyHealthFrame, topLeft, widthHeight );

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
	mScreenHeight	= Input::GetInstance()->mScreenHeight;
	mScreenWidth	= Input::GetInstance()->mScreenWidth;

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