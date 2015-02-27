#include "LoadOutMenu.h"

void LoadOutMenu::Activate()
{
	mIsActive = true;
}

void LoadOutMenu::DeActivate()
{
	mIsActive	= false;
	mBackButton.SetExitCooldown();
}

void LoadOutMenu::Update( float deltaTime )
{
	for( int i = 0; i < NR_OF_WEAPONS; i++ )
	{
		mButtons[i].Update( deltaTime );
	}

	for( int i = 0; i < NR_OF_WEAPONS; i++ )
	{
		if( mButtons[i].Pressed() )
		{
			IEventPtr E1( new Event_Change_Weapon( (int)mButtons[i].weaponType ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			if( mButtons[i].weaponType == MINIGUN || mButtons[i].weaponType == SHOTGUN || mButtons[i].weaponType == GRENADELAUNCHER || mButtons[i].weaponType == SNIPER )
			{
				mCurrentRanged = i;
			}
			else
			{
				mCurrentMelee = i;
			}
		}
	}
	mBackButton.Update( deltaTime );
	if( mBackButton.LeftMousePressed() )
	{
		DeActivate();
	}
}

void LoadOutMenu::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );

	for( int i = 0; i < NR_OF_WEAPONS; i++ )
	{
		mButtons[i].Render();
	}
	mBackButton.Render();

	RenderManager::GetInstance()->AddObject2dToList( mMarker, mButtons[mCurrentMelee].button.GetPosition(), XMFLOAT2( 528.0f, 80.0f ) );
	RenderManager::GetInstance()->AddObject2dToList( mMarker, mButtons[mCurrentRanged].button.GetPosition(), XMFLOAT2( 528.0f, 80.0f ) );
}

void LoadOutMenu::Reset()
{
	mCurrentRanged	= 0;
	mCurrentMelee	= 5;
}

void LoadOutMenu::Release()
{
	for( int i = 0; i < NR_OF_WEAPONS; i++ )
	{
		mButtons[i].button.Release();
		mButtons[i].text.Release();
	}
	mBackButton.Release();
}

HRESULT LoadOutMenu::Initialize()
{
	mIsActive	= false;

	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/loadout_background_menu.dds", mBackground );

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/choosenWeaponMarker.dds", mMarker );

	std::string texts[NR_OF_WEAPONS]  = { "../Content/Assets/Textures/Menu/lobby_loadout_menu/textMinigun.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textShotgun.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textGrenadelauncher.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textSniper.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textSword.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textHammer.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textBlowtorch.dds", "../Content/Assets/Textures/Menu/lobby_loadout_menu/textChainsaw.dds" };

	float offsetY = 0.0f;
	for( int i = 0; i < NR_OF_WEAPONS / 2; i++ )
	{
		mButtons[i].button.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/loadout_text_frame.dds", 300.0f, 330.0f + offsetY, 528.0f, 80.0f );
		mButtons[i].text.Initialize( texts[i], 300.0f, 330.0f + offsetY, 528.0f, 80.0f );
		offsetY += 124.0f;
	}

	offsetY = 0.0f;
	for( int i = NR_OF_WEAPONS / 2; i < NR_OF_WEAPONS; i++ )
	{
		mButtons[i].button.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/loadout_text_frame.dds", 1050.0f, 330.0f + offsetY, 528.0f, 80.0f );
		mButtons[i].text.Initialize( texts[i], 1050.0f, 330.0f + offsetY, 528.0f, 80.0f );
		offsetY += 124.0f;
	}
	
	mBackButton.Initialize( "../Content/Assets/Textures/Menu/Back.png", 820.0f, 820.0f, 200.0f, 200.0f );
	
	WeaponType types[NR_OF_WEAPONS] = { MINIGUN, SHOTGUN, GRENADELAUNCHER, SNIPER, CLAYMORE, HAMMER, BLOWTORCH, SAW };
	for( int i = 0; i < NR_OF_WEAPONS; i++ )
	{
		mButtons[i].weaponType = types[i];
	}

	return result;
}

LoadOutMenu::LoadOutMenu()
{
	mIsActive		= false;
	mBackground		= 0;
	mCurrentRanged	= 0;
	mCurrentMelee	= 5;
}

LoadOutMenu::~LoadOutMenu()
{

}

bool LoadOutMenu::IsActive() const
{
	return mIsActive;
}