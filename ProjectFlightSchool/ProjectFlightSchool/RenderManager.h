#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <Graphics.h>
#include "RenderStructs.h"

static const UINT MAX_AMOUNT_OF_OBJECT3D = 500;
static const UINT MAX_AMOUNT_OF_ANIM3D = 100;

class RenderManager
{
	private:
		Object3dInfo	mObject3dArray[MAX_AMOUNT_OF_OBJECT3D];
		Anim3dInfo		mAnim3dArray[MAX_AMOUNT_OF_ANIM3D];

	protected:
	public:
		UINT mNrOfObject3d;
		UINT mNrOfAnim3d;

	private:
		RenderManager();
		virtual~RenderManager();
	protected:
	public:
		void AddObject3dToList( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		void AddAnim3dToList( AssetID modelAssetId, AssetID animationAssetId, float* animationTime );
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );	//Currently clearing the arrays to standard values and reseting number of active objects variable for them in update
		HRESULT Initialize();
		void Release();
		static RenderManager* GetInstance();


};

#endif