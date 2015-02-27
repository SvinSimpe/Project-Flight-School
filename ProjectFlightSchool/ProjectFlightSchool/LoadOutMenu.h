#ifndef LOADOUTMENU_H
#define LOADOUTMENU_H

#include "Button.h"
#include "RenderManager.h"
#include "Font.h"
#include "SoundBufferHandler.h"
#include "WeaponInfo.h"
#include "MovingButton.h"

#define	NR_OF_WEAPONS 8

struct WeaponButtonStruct
{
	Button		button;
	Image		text;
	WeaponType	weaponType;

	bool Pressed()
	{
		if( button.LeftMousePressed() )
		{
			return true;
		}
		return false;
	}

	void Update( float deltaTime )
	{
		button.Update( deltaTime );
	}

	void Render()
	{
		button.Render();
		text.Render();
	}
};

class LoadOutMenu
{
private:
	bool				mIsActive;
	AssetID				mBackground;
	AssetID				mMarker;
	int					mCurrentRanged;
	int					mCurrentMelee;
	WeaponButtonStruct	mButtons[NR_OF_WEAPONS];
	MovingButton		mBackButton;

protected:
public:

private:
	void	HandleInput();
protected:
public:
	void	Activate();
	void	DeActivate();
	void	Update( float deltaTime );
	void	Render();
	void	Release();
	HRESULT	Initialize();
	LoadOutMenu();
	~LoadOutMenu();

	bool	IsActive() const;
};

#endif // !INGAMEWINDOW_H