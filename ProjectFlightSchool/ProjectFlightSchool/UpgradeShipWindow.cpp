#include "UpgradeShipWindow.h"

void UpgradeShipWindow::Render()
{
	XMFLOAT2 topLeftCorner;
	XMFLOAT2 widthHeight;

	widthHeight.x = Input::GetInstance()->mScreenWidth * 0.45f;
	widthHeight.y = Input::GetInstance()->mScreenHeight * 0.26f;

	topLeftCorner.x = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - (float)( widthHeight.x / 2.0f );
	topLeftCorner.y = Input::GetInstance()->mScreenHeight - widthHeight.y;

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, topLeftCorner, widthHeight );
}

void UpgradeShipWindow::Release()
{
	
}

HRESULT UpgradeShipWindow::Initialize()
{
	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	return result;
}

UpgradeShipWindow::UpgradeShipWindow()
{

}

UpgradeShipWindow::~UpgradeShipWindow()
{

}