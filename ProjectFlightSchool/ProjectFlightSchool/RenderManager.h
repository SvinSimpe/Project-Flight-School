#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "LightManager.h"
#include "RenderStructs.h"

class RenderManager
{
	private:
		Object3dInfo	mObject3dArray[MAX_AMOUNT_OF_OBJECT3D];
		Object2dInfo	mObject2dArray[MAX_AMOUNT_OF_OBJECT2D];
		Anim3dInfo		mAnim3dArray[MAX_AMOUNT_OF_ANIM3D];
		PlaneInfo		mPlaneArray[MAX_AMOUNT_OF_PLANES];
		BoxInfo			mBoxArray[MAX_AMOUNT_OF_BOXES];
		LightManager*	mLightManager;

	protected:
	public:
		UINT mNrOfObject3d;
		UINT mNrOfAnim3d;
		UINT mNrOfObject2d;
		UINT mNrOfPlane;
		UINT mNrOfBoxes;

	private:
		void SetLightStructuredBuffer();
		void Clear();
		RenderManager();
		virtual~RenderManager();
	protected:
	public:
		void AddObject3dToList( AssetID assetId, DirectX::XMFLOAT3 position = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ), DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		void AddObject3dToList( AssetID assetId, DirectX::XMFLOAT4X4 world );
		void AddObject2dToList( AssetID assetId, DirectX::XMFLOAT2 topLeftCorner, DirectX::XMFLOAT2 widthHeight );
		void AddBoxToList( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max );
		void AddAnim3dToList( AssetID modelAssetId, AssetID animationAssetId, float* animationTime, DirectX::XMFLOAT3 position = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ), DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		void AddPlaneToList( AssetID assetId, DirectX::XMFLOAT3 topTriangle, DirectX::XMFLOAT3 bottomTriangle );
		HRESULT Update( float deltaTime );	//Currently clearing the arrays to standard values and reseting number of active objects variable for them in update
		HRESULT Render();	
		HRESULT Initialize();
		void Release();
		static RenderManager* GetInstance();


};

#endif