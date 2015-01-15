#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <Graphics.h>
#include "RenderStructs.h"

static const UINT MAX_AMOUNT_OF_OBJECT3D	= 500;
static const UINT MAX_AMOUNT_OF_OBJECT2D	= 50;
static const UINT MAX_AMOUNT_OF_ANIM3D		= 100;
static const UINT MAX_AMOUNT_OF_PLANES		= 10;


class RenderManager
{
	private:
		Object3dInfo	mObject3dArray[MAX_AMOUNT_OF_OBJECT3D];
		Object2dInfo	mObject2dArray[MAX_AMOUNT_OF_OBJECT2D];
		Anim3dInfo		mAnim3dArray[MAX_AMOUNT_OF_ANIM3D];
		PlaneInfo		mPlaneArray[MAX_AMOUNT_OF_PLANES];

	protected:
	public:
		UINT mNrOfObject3d;
		UINT mNrOfAnim3d;
		UINT mNrOfObject2d;
		UINT mNrOfPlane;

	private:
		void Clear();
		RenderManager();
		virtual~RenderManager();
	protected:
	public:
		void AddObject3dToList( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		void AddObject2dToList( AssetID assetId, DirectX::XMFLOAT2 topLeftCorner, DirectX::XMFLOAT2 widthHeight );
		void AddAnim3dToList( AssetID modelAssetId, AssetID animationAssetId, float* animationTime );
		void AddPlaneToList( AssetID assetId, DirectX::XMFLOAT3 topTriangle, DirectX::XMFLOAT3 bottomTriangle );
		HRESULT Update( float deltaTime );	//Currently clearing the arrays to standard values and reseting number of active objects variable for them in update
		HRESULT Render();	
		HRESULT Initialize();
		void Release();
		static RenderManager* GetInstance();


};

#endif