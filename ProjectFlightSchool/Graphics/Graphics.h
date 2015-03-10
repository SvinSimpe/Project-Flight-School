#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include "AssetManager.h"
#include "Effect.h"
#include "CbufferPerFrame.h"
#include "CbufferPerObject.h"
#include "LightStructure.h"
#include "Camera.h"
#include "Gbuffer.h"
#include "..\ProjectFlightSchool\RenderStructs.h"

#ifdef COMPILE_LIBRARY
	#define LIBRARY_EXPORT __declspec( dllexport )
#else
	#define LIBRARY_EXPORT __declspec( dllimport )
#endif

enum BlendStates
{
	BLEND_NORMAL,
	BLEND_ADD,

	//New states added above this comment
	BLEND_STATES_AMOUNT
};

enum Buffers
{
	BUFFERS_CBUFFER_PER_FRAME,
	BUFFERS_CBUFFER_PER_FRAME_SHADOW,
	BUFFERS_CBUFFER_PER_OBJECT,
	BUFFERS_CBUFFER_PER_OBJECT_2D,
	BUFFERS_CBUFFER_PER_OBJECT_ANIMATED,
	BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED,
	BUFFERS_STATIC3D_PER_INSTANCED_OBJECT,
	BUFFERS_2D,
	BUFFERS_BILLBOARD,
	BUFFERS_LIGHT,
	BUFFERS_SINGLE_VERTEX,
	BUFFERS_PARTICLE,
	BUFFERS_SINGLE_STATIC_VERTEX,
	BUFFERS_CIRCLE_VERTEX,

	BUFFERS_DEBUG_BOX,
	BUFFERS_DEBUG_BOX_INDICES,
	//New buffers added above this comment
	BUFFERS_AMOUNT
};

enum DepthStencils
{
	DEPTHSTENCILS_DISABLED,
	DEPTHSTENCILS_ENABLED,
	DEPTHSTENCILS_MASK_DISABLED,

	DEPTHSTENCILS_AMOUNT
};

enum RasterizerStates
{
	CULL_NONE,
	CULL_BACK,
	WIREFRAME,

	RASTERIZER_STATES_AMOUNT
};

enum Effects
{
	//Particle Effects
	EFFECTS_SNIPER_TRAIL,
	EFFECTS_SHELL,
	EFFECTS_DEBRIS,
	EFFECTS_FIRE,	
	EFFECTS_SPARK,
	EFFECTS_SPARK_ROBOT,
	EFFECTS_SPARK_ELECTRIC,
	EFFECTS_BLOOD,	
	EFFECTS_MUZZLEFLASH,
	EFFECTS_SMOKE_MINIGUN,
	EFFECTS_SPORES,
	EFFECTS_FIRE_SMOKE,
	EFFECTS_BLOWTORCH_FIRE,
	EFFECTS_BLOWTORCH_IDLE,
	EFFECTS_LEVEL_UP,
	EFFECTS_LEVEL_INNER,
	EFFECTS_EXPLOSION,
	EFFECTS_EXPLOSION_SMOKE,
	EFFECTS_NORMAL_SMOKE,
	EFFECTS_FIRE_FLIES,
	EFFECTS_HAMMER_EFFECT,

	EFFECTS_STATIC_VERTEX,
	EFFECTS_STATIC_INSTANCED,
	EFFECTS_STATIC_INSTANCED_SHADOW,
	EFFECTS_2D,
	EFFECTS_ANIMATED,
	EFFECTS_ANIMATED_INSTANCED,
	EFFECTS_ANIMATED_INSTANCED_SHADOW,
	EFFECTS_DEFERRED,
	EFFECTS_BILLBOARD,
	EFFECTS_NODEGRID,

	EFFECTS_DEBUG_BOX,
	EFFECTS_CIRCLE,
	//New effects added above this comment
	EFFECTS_AMOUNT
};

enum Samplers
{
	SAMPLERS_POINT,
	SAMPLERS_LINEAR,

	SAMPLERS_AMOUNT
};

enum Cameras
{
	CAMERAS_MAIN,
	CAMERAS_DEV,
	CAMERAS_SHADOWMAP,
	//New cameras added above this comment
	CAMERAS_AMOUNT
};

#define ANIMATION_PLAY_LOOPED	0
#define ANIMATION_PLAY_ONCE		1

#define NUM_GBUFFERS				3
#define MAX_ANIM_INSTANCE_BATCH		32
#define MAX_STATIC3D_INSTANCE_BATCH 512
#define MAX_BILLBOARD_BATCH			5000 // Used to be 1024
#define MAX_PARTICLE_BATCH			5000
#define MAX_SINGLE_STATIC_VERTICES	20000
#define MAX_CIRCLE_POINTS			1000

#define SHADOW_MAP_WIDTH	512
#define SHADOW_MAP_HEIGHT	512

#define SAFE_RELEASE_DELETE( x ) if( x ) { ( x )->Release(); delete x; ( x ) = nullptr; }

class LIBRARY_EXPORT Graphics
{
	//Class members
	private:
		HWND						mHWnd;
		UINT						mScreenWidth;
		UINT						mScreenHeight;
		bool						mFullscreen;
		float						mTimeVariable;

		IDXGISwapChain*				mSwapChain;
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;

		ID3D11RenderTargetView*		mRenderTargetView;
		ID3D11DepthStencilView*		mDepthStencilView;
		ID3D11ShaderResourceView*	mLightStructuredBuffer;
		D3D11_VIEWPORT				mStandardView;
		ID3D11Buffer*				mBuffers[BUFFERS_AMOUNT];
		ID3D11BlendState*			mBlendStates[BLEND_STATES_AMOUNT];
		ID3D11DepthStencilState*	mDepthStencils[DEPTHSTENCILS_AMOUNT];
		ID3D11SamplerState*			mSamplerStates[SAMPLERS_AMOUNT];
		ID3D11RasterizerState*		mRasterizerState[RASTERIZER_STATES_AMOUNT];

		AssetManager*				mAssetManager;
		Effect*						mEffects[EFFECTS_AMOUNT];
		Camera*						mCamera[CAMERAS_AMOUNT];
		Cameras						mCurrentCamera;
		bool						mIsDeveloperCameraActive;
		Gbuffer*					mGbuffers[NUM_GBUFFERS];

		ID3D11DepthStencilView*		mShadowMapDSV;
		ID3D11ShaderResourceView*	mShadowMapSRV;
		D3D11_VIEWPORT				mShadowView;

		int							mNumPointLights;

		StaticInstance				mStatic3dInstanced[MAX_STATIC3D_INSTANCE_BATCH];
		AnimatedInstance			mAnimInstanced[MAX_ANIM_INSTANCE_BATCH];
		CbufferPerObjectAnimated	mAnimCbufferInstanced[MAX_ANIM_INSTANCE_BATCH];
		BillboardInstanced			mBillboardInstanced[MAX_BILLBOARD_BATCH];
		ParticleVertex16			mParticleInstanced[MAX_PARTICLE_BATCH];

	protected:
	public:

	//Class functions
	private:
		Graphics();
		virtual	~Graphics();

		HRESULT InitializeDepthStencilStates();
		HRESULT InitializeSamplerStates();
		HRESULT InitializeBuffers();
		HRESULT InitializeEffects();
		HRESULT MapBuffer( ID3D11Buffer* buffer, void* data, int size );

	protected:
	public:
		void Update( float deltaTime );

		HRESULT LoadStatic2dAsset	( std::string fileName, AssetID &assetId );
		HRESULT LoadStatic3dAsset	( std::string filePath, std::string fileName, AssetID &assetId );
		HRESULT LoadAnimated3dAsset	( std::string filePath, std::string fileName, AssetID skeletonId, AssetID &assetId ); 
		HRESULT LoadSkeletonAsset	( std::string filePath, std::string fileName, AssetID &assetId );
		HRESULT LoadAnimationAsset	( std::string filePath, std::string fileName, AssetID &assetId );

		void Render2dAsset			( Object2dInfo* info, UINT sizeOfList );
		void RenderPlane2dAsset		( AssetID assetId, DirectX::XMFLOAT3 x, DirectX::XMFLOAT3 y );
		void RenderStatic3dAsset	( Object3dInfo* info, UINT sizeOfList );
		void RenderAnimated3dAsset	( Anim3dInfo* info, UINT sizeOfList );
		void RenderBillboard		( BillboardInfo* info, UINT sizeOfList );
		void RenderParticleSystems	( ParticleInfo* info, UINT sizeOfList );
		void RenderNodeGrid			( NodeGridInfo* info, UINT sizeOfList );
		void RenderDebugBox			( BoxInfo* info, UINT sizeOfList );
		void RenderDebugBox			( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max, DirectX::XMFLOAT4X4 world );
		void RenderLine				( LineInfo* info, UINT sizeOfList );
		void RenderCircle			( CircleInfo* info, UINT sizeOfList );

		DirectX::XMFLOAT4X4	GetRootMatrix( AnimationTrack animTrack );
		bool				GetAnimationMatrices( AnimationTrack &animTrack, int playType, Anim3dInfo &info );
		OctTree*			GetOctTreeFromStatic3DAsset( AssetID assetID );

		void ChangeCamera();
		void ZoomInDeveloperCamera();
		void ZoomOutDeveloperCamera();

		void GetViewMatrix				( DirectX::XMMATRIX &view );
		void GetInverseViewMatrix		( DirectX::XMMATRIX &inverseViewMatrix );
		void GetProjectionMatrix		( DirectX::XMMATRIX &proj );
		void GetInverseProjectionMatrix	( DirectX::XMMATRIX &projectionViewMatrix );

		void ChangeRasterizerState		( RasterizerStates rasterState );
		void MapLightStructuredBuffer	( LightStructure* lightStructure, int numPointLights );
		void SetNDCSpaceCoordinates		( float &mousePositionX, float &mousePositionY );
		void SetEyePosition				( Cameras camera, DirectX::XMFLOAT3 &eyePosition );
		void SetFocus					( Cameras camera, DirectX::XMFLOAT3 &focusPoint );

		void BeginScene();
		void GbufferPass();
		void DeferredPass();
		void ScreenSpacePass();
		void EndScene();

		void ToggleFullscreen();

		static	Graphics* GetInstance();
		HRESULT Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight, bool fullscreen );
		void	Release();
};
#endif