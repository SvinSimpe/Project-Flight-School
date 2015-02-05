#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects, DirectX::XMFLOAT3 remotePlayerPos, std::string remotePlayerName, int remotePlayerID, bool updateRemotePlayerName )
{
	mRadar->Update( playerPos, radarObjects, nrOfObjects );
	
	
	if( updateRemotePlayerName )
	{
		//Updating remote player names
		DirectX::XMMATRIX view, proj;
		Graphics::GetInstance()->GetViewMatrix( view );
		Graphics::GetInstance()->GetProjectionMatrix( proj );
		view = DirectX::XMMatrixTranspose( view );
		proj = DirectX::XMMatrixTranspose( proj );

	
		DirectX::XMFLOAT4 textPos = XMFLOAT4( remotePlayerPos.x, remotePlayerPos.y + 3.25f, remotePlayerPos.z, 1.0f );
		DirectX::XMStoreFloat4( &textPos, DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &textPos ), DirectX::XMMatrixMultiply( view, proj ) ) );

		textPos.x /= textPos.w;
		textPos.y /= textPos.w;

		textPos.x = ( textPos.x + 1.0f ) * ( 0.5f * Input::GetInstance()->mScreenWidth );
		textPos.y = ( 1.0f - textPos.y ) * ( 0.5f * Input::GetInstance()->mScreenHeight );
		mPlayerNames[remotePlayerID].SetText( remotePlayerName );
		mPlayerNames[remotePlayerID].SetPosition( textPos.x - mPlayerNames[remotePlayerID].GetMiddleXPoint( mPlayerNames[remotePlayerID].GetText(), mPlayerNames[remotePlayerID].GetScale() ), textPos.y );
		//-------------------------------------------------------------
	}

	return S_OK;
}

HRESULT Gui::Render()
{
	mRadar->Render();

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Render();
	}

	return S_OK;
}

HRESULT Gui::Initialize()
{
	mRadar = new Radar();
	mRadar->Initialize();

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Initialize( "", "", 0, 0, 0.25f );
	}

	return S_OK;
}

void Gui::Release()
{
	mRadar->Release();
	SAFE_DELETE( mRadar );
}

Gui::Gui()
{
	mRadar = nullptr;
}

Gui::~Gui()
{

}