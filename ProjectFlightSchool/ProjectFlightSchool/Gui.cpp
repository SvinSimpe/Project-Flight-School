#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects )
{
	mRadar->Update( playerPos, radarObjects, nrOfObjects );
	return S_OK;
}

HRESULT Gui::Render()
{
	mRadar->Render();
	return S_OK;
}

HRESULT Gui::Initialize()
{
	mRadar = new Radar();
	mRadar->Initialize();
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