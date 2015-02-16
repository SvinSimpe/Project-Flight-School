#ifndef ENERGYCELL_H
#define ENERGYCELL_H

#include "Graphics.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"

#define MAX_ENERGY_CELLS 10	

class EnergyCell
{
	private:
		AssetID				mAssetID;
		UINT				mOwnerID;
		bool				mPickedUp;
		BoundingCircle*		mPickUpRadius;

	protected:
	public:

	private:
	protected:
	public:
		void				SetOwnerID( UINT id );
		void				SetPickedUp( bool pickedUp );
		void				SetPosition( DirectX::XMFLOAT3 position );

		UINT				GetOwnerID() const;
		bool				GetPickedUp() const;
		DirectX::XMFLOAT3	GetPosition() const;
		BoundingCircle*		GetPickUpRadius() const;

		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		HRESULT				Initialize( DirectX::XMFLOAT3 position );
		void				Release();
							EnergyCell();
		virtual				~EnergyCell();
};

#endif