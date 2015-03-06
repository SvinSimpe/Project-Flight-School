#ifndef ENDGAMEWINDOW_H
#define ENDGAMEWINDOW_H

#include "Button.h"
#include "RenderManager.h"
#include "Font.h"

struct EndGameButton
{
	Button				mButton;
	DirectX::XMFLOAT2	mButtonTextPosition;
	float				mButtonTextScale;

	bool IsPressed()
	{
		if( mButton.LeftMousePressed() )
		{
			return true;
		}
		else return false;
	}

	void Update( float deltaTime )
	{
		mButton.Update( deltaTime );
	}

	void Render( Font font )
	{
		mButton.Render();
		font.WriteText( "END GAME", mButtonTextPosition.x, mButtonTextPosition.y, mButtonTextScale );
	}
};

class EndGameWindow
{
	private:
		AssetID			mWinScreen;
		AssetID			mLoseScreen;
		Font			mFont;
		EndGameButton	mEndGameButton;
		bool			mWonGame;

	protected:
	public:

	private:
	protected:
	public:

		void	Update( float deltaTime, bool wonGame );
		void	Render();
		HRESULT	Initialize();
		void	Reset();
		void	Release();
				EndGameWindow();
		virtual ~EndGameWindow();
};

#endif