#include "UpgradeShipWindow.h"

void UpgradeShipWindow::HandleInput()
{

}

HRESULT UpgradeShipWindow::Update( float deltaTime )
{
	for( int i = 0; i < MAX_NR_OF_UPGRADES; i++ )
	{
		mTurretButtons[i].emptyButton->Update( deltaTime );
		mForceFieldButtons[i].emptyButton->Update( deltaTime );
		mBuffButtons[i].emptyButton->Update( deltaTime );
		mEngineButtons[i * 2].emptyButton->Update( deltaTime );
		mEngineButtons[i * 2 + 2].emptyButton->Update( deltaTime );

		mTurretButtons[i].filledButton->Update( deltaTime );
		mForceFieldButtons[i].filledButton->Update( deltaTime );
		mBuffButtons[i].filledButton->Update( deltaTime );
		mEngineButtons[i * 2].filledButton->Update( deltaTime );
		mEngineButtons[i * 2 + 2].filledButton->Update( deltaTime );
	}
	return S_OK;
}

void UpgradeShipWindow::Render()
{
	XMFLOAT2 topLeftCorner;
	XMFLOAT2 widthHeight;

	widthHeight.x = Input::GetInstance()->mScreenWidth * 0.45f;
	widthHeight.y = Input::GetInstance()->mScreenHeight * 0.26f;

	topLeftCorner.x = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - (float)( widthHeight.x / 2.0f );
	topLeftCorner.y = Input::GetInstance()->mScreenHeight - widthHeight.y;

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, topLeftCorner, widthHeight );

	//for( int i = 0; i < MAX_NR_OF_UPGRADES; i++ )
	//{
	//	if ( mTurretButtons[i].filled )
	//	{
	//		mTurretButtons[i].emptyButton->Render();
	//	}
	//	else
	//	{
	//		mTurretButtons[i].filledButton->Render();
	//	}

	//	if ( mTurretButtons[i].filled )
	//	{
	//		mForceFieldButtons[i].filledButton->Render();
	//	}
	//	else
	//	{
	//		mForceFieldButtons[i].emptyButton->Render();
	//	}

	//	if ( mBuffButtons[i].filled )
	//	{
	//		mBuffButtons[i].emptyButton->Render();
	//	}
	//	else
	//	{
	//		mBuffButtons[i].filledButton->Render();
	//	}
	//	
	//	if ( mEngineButtons[i * 2].filled )
	//	{
	//		mEngineButtons[i * 2].emptyButton->Render();
	//	}
	//	else
	//	{
	//		mEngineButtons[i * 2].filledButton->Render();
	//	}
	//	
	//	if ( mEngineButtons[i * 2 + 2].filled )
	//	{
	//		mEngineButtons[i * 2 + 2].emptyButton->Render();
	//	}
	//	else
	//	{
	//		mEngineButtons[i * 2 + 2].filledButton->Render();
	//	}
	//}
}

void UpgradeShipWindow::Release()
{
	for( int i = 0; i < MAX_NR_OF_UPGRADES; i++ )
	{
		mTurretButtons[i].emptyButton->Release();
		SAFE_DELETE( mTurretButtons[i].emptyButton );
		mForceFieldButtons[i].emptyButton->Release();
		SAFE_DELETE( mForceFieldButtons[i].emptyButton );
		mBuffButtons[i].emptyButton->Release();
		SAFE_DELETE( mBuffButtons[i].emptyButton );
		mEngineButtons[i * 2].emptyButton->Release();
		SAFE_DELETE( mEngineButtons[i * 2].emptyButton );
		mEngineButtons[i * 2 + 2].emptyButton->Release();
		SAFE_DELETE( mEngineButtons[i * 2 + 2].emptyButton );

		mTurretButtons[i].filledButton->Release();
		SAFE_DELETE( mTurretButtons[i].filledButton );
		mForceFieldButtons[i].filledButton->Release();
		SAFE_DELETE( mForceFieldButtons[i].filledButton );
		mBuffButtons[i].filledButton->Release();
		SAFE_DELETE( mBuffButtons[i].filledButton );
		mEngineButtons[i * 2].filledButton->Release();
		SAFE_DELETE( mEngineButtons[i * 2].filledButton );
		mEngineButtons[i * 2 + 2].filledButton->Release();
		SAFE_DELETE( mEngineButtons[i * 2 + 2].filledButton );
	}
}

HRESULT UpgradeShipWindow::Initialize()
{
	HRESULT result;
	for( int i = 0; i < MAX_NR_OF_UPGRADES; i++ )
	{
		//mTurretButtons[i]->Initialize();
		//mForceFieldButtons[i]->Initialize();
		//mBuffButtons[i]->Initialize();
		//mEngineButtons[i * 2]->Initialize();
		//mEngineButtons[i * 2 + 2]->Initialize();
	}

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	return result;
}

UpgradeShipWindow::UpgradeShipWindow()
{

}

UpgradeShipWindow::~UpgradeShipWindow()
{

}