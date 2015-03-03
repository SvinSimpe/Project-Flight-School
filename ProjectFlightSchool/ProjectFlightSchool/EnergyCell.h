#ifndef ENERGYCELL_H
#define ENERGYCELL_H

#include "Graphics.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"

#define MAX_ENERGY_CELLS 2

class EnergyCell
{
	private:
		AnimationTrack		mAnimationTrack;
		AssetID				mAnimation;
		UINT				mOwnerID;
		bool				mPickedUp;
		BoundingCircle*		mPickUpRadius;
		float				mHooverFactor;
		bool				mSecured;



	protected:
	public:

	private:
	protected:
	public:
		void				SetOwnerID( UINT id );
		void				SetPickedUp( bool pickedUp );
		void				SetPosition( DirectX::XMFLOAT3 position );
		void				SetSecured( bool secured );

		UINT				GetOwnerID() const;
		bool				GetPickedUp() const;
		DirectX::XMFLOAT3	GetPosition() const;
		BoundingCircle*		GetPickUpRadius() const;
		bool				GetSecured() const;

		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		void				Reset();
		HRESULT				Initialize( DirectX::XMFLOAT3 position );
		void				Release();
							EnergyCell();
		virtual				~EnergyCell();
};

#endif