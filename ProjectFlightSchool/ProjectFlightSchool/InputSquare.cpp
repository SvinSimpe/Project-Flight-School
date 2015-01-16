#include "InputSquare.h"

void InputSquare::Add(std::string letter)
{
	mCurrentText += letter;
}

void InputSquare::Pop()
{
	if(mCurrentText.size() > 0)
	{
		mCurrentText.pop_back();
	}
}

bool InputSquare::IsActive()
{
	return mActive;
}

void InputSquare::SwitchActive()
{
	if( mActive )
		mActive = false;
	else
		mActive = true;
}

void InputSquare::HandleInput()
{
	for(int i = 0; i < KEYS::KEYS_AMOUNT; i++)
	{
		if(Input::GetInstance()->mCurrentFrame.at(i))
		{
			mKeyCD = KEYCOOLDOWN;
			switch(i)
			{
			case KEYS::KEYS_A:
				Add("A");
				break;
			case KEYS::KEYS_BACKSPACE:
				Pop();
				break;
			}
			break;
		}
	}
}

void InputSquare::Render()
{
	mInputImage.Render();
	mText.WriteText( mCurrentText, 0.0f, 0.0f, 1.0f );
	RenderManager::GetInstance()->Render();
}

bool InputSquare::Update( float deltaTime )
{
	Entry::Update( deltaTime );
	if( mActive && mKeyCD <= 0.0f )
	{
		HandleInput();
	}
	else
	{
		mKeyCD -= deltaTime;
	}
	return true;
}

bool InputSquare::Initialize( UINT x, UINT y, UINT width, UINT height )
{
	Entry::Initialize( x, y, width, height );

	mInputImage.Initialize( "../Content/Assets/Textures/Menu/Input.png", x, y, width, height );
	mText.Initialize( "../Content/Assets/Fonts/mv_boli_26_red/" );

	return true;
}

void InputSquare::Release()
{
	mInputImage.Release();
	mText.Release();
}

InputSquare::InputSquare()
{
	mCurrentText	= "ABC";
	mActive			= false;
	mKeyCD			= KEYCOOLDOWN;
}


InputSquare::~InputSquare()
{
}
