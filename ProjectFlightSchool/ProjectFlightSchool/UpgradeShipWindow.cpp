#include "UpgradeShipWindow.h"

void UpgradeShipWindow::Render()
{
	XMFLOAT2 topLeftCorner;
	XMFLOAT2 widthHeight;

	widthHeight.x = 20.0f;
	widthHeight.y = 20.0f;

	topLeftCorner.x = (float)( Input::GetInstance()->mCurrentMousePos.x );
	topLeftCorner.y = (float)( Input::GetInstance()->mCurrentMousePos.y );

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, topLeftCorner, widthHeight );

	widthHeight.x = mSize.x;
	widthHeight.y = mSize.y;

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, mTopLeftCorner, widthHeight );
}

void UpgradeShipWindow::Release()
{
	
}

HRESULT UpgradeShipWindow::Initialize()
{
	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	mSize.x = Input::GetInstance()->mScreenWidth * 0.45f;
	mSize.y = Input::GetInstance()->mScreenHeight * 0.26f;

	mTopLeftCorner.x = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - (float)( mSize.x / 2.0f );
	mTopLeftCorner.y = Input::GetInstance()->mScreenHeight - mSize.y;

	return result;
}

UpgradeShipWindow::UpgradeShipWindow()
{

}

UpgradeShipWindow::~UpgradeShipWindow()
{

}