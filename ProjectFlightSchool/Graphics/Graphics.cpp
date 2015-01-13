#include "Graphics.h"

Graphics::Graphics()
{
	mHWnd			= 0;
	mScreenWidth	= 0;
	mScreenHeight	= 0;	
	mVertexBuffer2d = nullptr;

	mSwapChain		= nullptr;
	mDevice			= nullptr;
	mDeviceContext	= nullptr;

	mRenderTargetView	= nullptr;
	mDepthStencilView	= nullptr;
	mCbufferPerFrame	= nullptr;

	mAssetManager				= nullptr;
	mStaticEffect				= nullptr;
	mAnimatedEffect				= nullptr;
	mCamera						= nullptr;
	mDeveloperCamera			= nullptr;
	mIsDeveloperCameraActive	= false;
}

Graphics::~Graphics()
{

}


//Map buffer
HRESULT Graphics::MapBuffer( ID3D11Buffer* buffer, void* data, int size )
{
	D3D11_MAPPED_SUBRESOURCE mapRes;
	mDeviceContext->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes );
	memcpy( mapRes.pData, data, size );
	mDeviceContext->Unmap( buffer, 0 );

	return S_OK;
}

HRESULT Graphics::LoadStatic2dAsset( std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadStatic2dAsset( mDevice, mDeviceContext, fileName, assetId ); 
}

//Load a static 3d asset to the AssetManager.
HRESULT Graphics::LoadStatic3dAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadStatic3dAsset( mDevice, mDeviceContext, filePath, fileName, assetId );
}

HRESULT Graphics::LoadStatic3dAssetIndexed( Indexed3DAssetInfo &info, AssetID &assetId )
{
	return mAssetManager->LoadStatic3dAssetIndexed( mDevice, info, assetId );
}

HRESULT Graphics::LoadAnimated3dAsset( std::string filePath, std::string fileName, AssetID skeletonId, AssetID &assetId )
{
	return mAssetManager->LoadAnimated3dAsset( mDevice, mDeviceContext, filePath, fileName, skeletonId, assetId );
}

HRESULT Graphics::LoadSkeletonAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadSkeletonAsset( filePath, fileName, assetId );
}

HRESULT Graphics::LoadAnimationAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadAnimationAsset( filePath, fileName, assetId );
}

void Graphics::Render2dAsset( AssetID assetId, float x, float y, float width, float height )
{
	mDeviceContext->OMSetDepthStencilState( mDepthDisabledStencilState, 1 );

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	float left		= ( ( x / (float)mScreenWidth ) * 2.0f ) - 1.0f;
	float right		= ( ( ( x + width ) / (float)mScreenWidth ) * 2.0f ) - 1.0f;
	float bottom	= ( ( ( y + height ) / (float)mScreenHeight ) * -2.0f ) + 1.0f;
	float top		= ( ( y / (float)mScreenHeight ) * -2.0f ) + 1.0f;

	StaticVertex bottomleft		= { left, bottom, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f };
	StaticVertex topleft		= { left, top, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f };
	StaticVertex bottomright	= { right, bottom, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f };
	StaticVertex topright		= { right, top, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f };

	StaticVertex vertices[4] = { bottomleft, topleft, bottomright, topright };
	MapBuffer( mVertexBuffer2d, &vertices, sizeof(StaticVertex) * 4 );
	mDeviceContext->IASetVertexBuffers( 0, 1, &mVertexBuffer2d, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( m2dEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( m2dEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( m2dEffect->GetPixelShader(), nullptr, 0 );

	mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[assetId] )->mSRV );
	mDeviceContext->Draw( 4, 0 );
	mDeviceContext->OMSetDepthStencilState( mDepthEnabledStencilState, 1 );
}

void Graphics::RenderPlane2dAsset( AssetID assetId, DirectX::XMFLOAT3 x, DirectX::XMFLOAT3 y )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	StaticVertex bottomleft		= { x.x, x.y, y.z,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f };
	StaticVertex topleft		= { x.x, x.y, x.z,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f };
	StaticVertex bottomright	= { y.x, x.y, y.z,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f };
	StaticVertex topright		= { y.x, x.y, x.z,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f };

	StaticVertex vertices[4]	= { bottomleft, topleft, bottomright, topright };
	MapBuffer( mVertexBuffer2d, &vertices, sizeof(StaticVertex) * 4 );
	mDeviceContext->IASetVertexBuffers( 0, 1, &mVertexBuffer2d, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

		//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[assetId] )->mSRV );
	mDeviceContext->PSSetShaderResources( 1, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[SPECULAR_PLACEHOLDER] )->mSRV );
	mDeviceContext->PSSetShaderResources( 2, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[NORMAL_PLACEHOLDER] )->mSRV );
	mDeviceContext->Draw( 4, 0 );
}

//Render a static 3d asset given the assetId
void Graphics::RenderStatic3dAsset( AssetID assetId )
{
	Static3dAsset* object = (Static3dAsset*)mAssetManager->mAssetContainer[assetId];

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize = sizeof( StaticVertex );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	for( int i = 0; i < (int)object->mMeshes.size(); i++ )
	{
		UINT32 offset					= 0;
		ID3D11Buffer* buffersToSet[]	= { object->mMeshes[i].mVertexBuffer };
		mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

		ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
													};

		mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

		mDeviceContext->Draw( object->mMeshes[i].mVertexCount, 0 );
	}
}

void Graphics::RenderStatic3dAsset( AssetID assetId, float x, float y, float z )
{
	Static3dAsset* object = (Static3dAsset*)mAssetManager->mAssetContainer[assetId];

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize = sizeof( StaticVertex );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMMatrixTranslation( x, y, z ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	for( int i = 0; i < (int)object->mMeshes.size(); i++ )
	{
		UINT32 offset					= 0;
		ID3D11Buffer* buffersToSet[]	= { object->mMeshes[i].mVertexBuffer };
		mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

		ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
													};

		mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

		mDeviceContext->Draw( object->mMeshes[i].mVertexCount, 0 );
	}
}

void Graphics::RenderStatic3dAsset( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
	Static3dAsset* object = (Static3dAsset*)mAssetManager->mAssetContainer[assetId];

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize = sizeof( StaticVertex );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( 
		DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
		DirectX::XMMatrixTranslation( position.x, position.y, position.z ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	for( int i = 0; i < (int)object->mMeshes.size(); i++ )
	{
		UINT32 offset					= 0;
		ID3D11Buffer* buffersToSet[]	= { object->mMeshes[i].mVertexBuffer };
		mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

		ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
													};

		mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

		mDeviceContext->Draw( object->mMeshes[i].mVertexCount, 0 );
	}
}

void Graphics::RenderStatic3dAsset( AssetID assetId, DirectX::XMFLOAT4X4* world )
{
	Static3dAsset* object = (Static3dAsset*)mAssetManager->mAssetContainer[assetId];

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize = sizeof( StaticVertex );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( world ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	for( int i = 0; i < (int)object->mMeshes.size(); i++ )
	{
		UINT32 offset					= 0;
		ID3D11Buffer* buffersToSet[]	= { object->mMeshes[i].mVertexBuffer };
		mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

		ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
													};

		mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

		mDeviceContext->Draw( object->mMeshes[i].mVertexCount, 0 );
	}
}

void Graphics::RenderStatic3dAssetIndexed( AssetID assetId, UINT indexCount, UINT startIndex )
{
	Static3dAssetIndexed* object = (Static3dAssetIndexed*)mAssetManager->mAssetContainer[assetId];

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize = sizeof( StaticVertex );

	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMMatrixIdentity() );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	for( int i = 0; i < (int)object->mMeshes.size(); i++ )
	{
		UINT32 offset					= 0;
		ID3D11Buffer* buffersToSet[]	= { object->mMeshes[i].mVertexBuffer };
		mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );
		mDeviceContext->IASetIndexBuffer( object->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
														( (Static2dAsset*)mAssetManager->mAssetContainer[object->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
													};

		mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

		mDeviceContext->DrawIndexed( indexCount, 0, 0 );
	}
}

void Graphics::RenderAnimated3dAsset( AssetID modelAssetId, AssetID animationAssetId, float &animationTime )
{
	Animated3dAsset*	model		= (Animated3dAsset*)mAssetManager->mAssetContainer[modelAssetId];
	Skeleton*			skeleton	= &( (SkeletonAsset*)mAssetManager->mAssetContainer[model->mSkeletonId] )->mSkeleton;
	AnimationData*		animation	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animationAssetId] )->mAnimationData;

	int	framesJumped	 = 0;
	bool animationLooped = false;

	if( animationTime > (float)animation->AnimLength / 60.0f )
	{
		animationTime	-= ( (float)animation->AnimLength / 60.0f - 1.0f / 60.0f );
		animationLooped  = true;
	}

	float calcTime = animationTime * 60.0f;

	while( calcTime > 1.0f )
	{
		calcTime -= 1.0f;
		framesJumped++;
	}

	DirectX::XMMATRIX currentBoneTransforms[NUM_SUPPORTED_JOINTS];
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
	{
		currentBoneTransforms[i] = DirectX::XMMatrixIdentity();
	}

	for( int i = 0; i < (int)skeleton->joints.size(); i++ )
	{
		int					lastFrame = 1;
		DirectX::XMFLOAT4X4	previousMatrix = skeleton->joints.at(i).originalMatrix;


		for( int j = 0; j < (int)animation->joints.at(i).keys.size(); j++ )
		{
			//If no keyframes exist apply static pose
			if( animation->joints.at(i).keys.size() == 1 && animation->joints.at(i).keys.at(j) == 0 )
			{
				if( skeleton->joints.at(i).parentIndex == -1 )
				{
					currentBoneTransforms[i] = DirectX::XMLoadFloat4x4( &animation->joints.at(i).matricies.at( 0 ) );
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &animation->joints.at(i).matricies.at( 0 ) );
					DirectX::XMMATRIX parent	= currentBoneTransforms[animation->joints.at(i).parentIndex];

					currentBoneTransforms[i] = child * parent;
				}
			}
			//Find next keyframe and interpolate previousMatrix with next matrix in animation based on key.
			else
			{
				if( animationLooped )
					for( int frames = 0; frames < animation->AnimLength; frames++ )
					{
						if( animation->joints.at(i).keys.at(j) == frames )
						{
							previousMatrix	= animation->joints.at(i).matricies.at(j);
						}
					}
				else
					for( int frames = 0; frames < framesJumped; frames++ )
					{
						if( animation->joints.at(i).keys.at(j) == frames )
						{
							previousMatrix	= animation->joints.at(i).matricies.at(j);
							lastFrame		= frames;
						}
					}

				if( animation->joints.at(i).keys.at(j) == framesJumped )
				{
					previousMatrix	= animation->joints.at(i).matricies.at(j);
					lastFrame		= framesJumped;

					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );
					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													currentBoneTransforms[animation->joints.at(i).parentIndex];

					currentBoneTransforms[i] = child * parent;
					break;
				}
				else if( animation->joints.at(i).keys.at(j) > framesJumped )
				{
					float interpolation					=	(float)( framesJumped - lastFrame ) /
															(float)( animation->joints.at(i).keys.at(j) - lastFrame );

					DirectX::XMMATRIX targetMatrix		= DirectX::XMLoadFloat4x4( &animation->joints.at(i).matricies.at(j) );
					DirectX::XMMATRIX child				= DirectX::XMLoadFloat4x4( &previousMatrix );
					
					DirectX::XMVECTOR targetComp[3];
					DirectX::XMVECTOR childComp[3];

					DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], targetMatrix );
					DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

					child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
																	DirectX::XMVectorZero(),
																	DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
																	DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );			

					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													currentBoneTransforms[animation->joints.at(i).parentIndex];

					currentBoneTransforms[i] = child * parent;
					break;
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );
					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													currentBoneTransforms[animation->joints.at(i).parentIndex];

					currentBoneTransforms[i] = child * parent;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( AnimatedVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { model->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mAnimatedEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mAnimatedEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mAnimatedEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObjectAnimated data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixIdentity();
	for( int i = 0; i < skeleton->nrOfJoints; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixTranspose( DirectX::XMMatrixMultiply(  DirectX::XMLoadFloat4x4( &model->mBoneOffsets[i] ), currentBoneTransforms[i] ) );
	MapBuffer( mCbufferPerObjectAnimated, &data, sizeof( CbufferPerObjectAnimated ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObjectAnimated );

	ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_DIFFUSE]] )->mSRV,
													( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_NORMAL]] )->mSRV,
													( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_SPECULAR]] )->mSRV,
												};

	mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

	mDeviceContext->Draw( model->mVertexCount, 0 );
}

void Graphics::RenderAnimated3dAsset( AssetID modelAssetId, AssetID animationAssetId, float &animationTime, float x, float y, float z )
{
	Animated3dAsset*	model		= (Animated3dAsset*)mAssetManager->mAssetContainer[modelAssetId];
	Skeleton*			skeleton	= &( (SkeletonAsset*)mAssetManager->mAssetContainer[model->mSkeletonId] )->mSkeleton;
	AnimationData*		animation	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animationAssetId] )->mAnimationData;

	if( animationTime > (float)animation->AnimLength / 60.0f )
	{
		animationTime	-= ( (float)animation->AnimLength / 60.0f - 1.0f / 60.0f );
	}

	float calcTime = animationTime * 60.0f;

	DirectX::XMMATRIX currentBoneTransforms[NUM_SUPPORTED_JOINTS];
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
	{
		currentBoneTransforms[i] = DirectX::XMMatrixIdentity();
	}

	for( int i = 0; i < (int)skeleton->joints.size(); i++ )
	{
		float prevTime		= 1.0f;
		float targetTime	= 1.0f;

		DirectX::XMFLOAT4X4	previousMatrix;
		DirectX::XMFLOAT4X4 targetMatrix = animation->joints.at(i).matricies.at(0);

		for( int j = 0; j < (int)animation->joints.at(i).keys.size() - 1; j++ )
		{
			if( (float)animation->joints.at(i).keys.at(j) <= calcTime )
			{
				prevTime		= targetTime;
				previousMatrix	= targetMatrix;
				targetTime		= (float)animation->joints.at(i).keys.at(j+1);
				targetMatrix	= animation->joints.at(i).matricies.at(j+1);
			}
		}

		float interpolation = ( calcTime - prevTime ) / ( targetTime - prevTime );

		DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );

		DirectX::XMVECTOR targetComp[3];
		DirectX::XMVECTOR childComp[3];

		DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], DirectX::XMLoadFloat4x4( &targetMatrix ) );
		DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

		child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
															DirectX::XMVectorZero(),
															DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
															DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );		

		DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
											currentBoneTransforms[animation->joints.at(i).parentIndex];

		currentBoneTransforms[i] = child * parent;
	}

	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( AnimatedVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { model->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mAnimatedEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mAnimatedEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mAnimatedEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObjectAnimated data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMMatrixTranslation( x, y, z ) );
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixIdentity();
	for( int i = 0; i < skeleton->nrOfJoints; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixTranspose( DirectX::XMMatrixMultiply(  DirectX::XMLoadFloat4x4( &model->mBoneOffsets[i] ), currentBoneTransforms[i] ) );
	MapBuffer( mCbufferPerObjectAnimated, &data, sizeof( CbufferPerObjectAnimated ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObjectAnimated );

	ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_DIFFUSE]] )->mSRV,
													( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_NORMAL]] )->mSRV,
													( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_SPECULAR]] )->mSRV,
												};

	mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

	mDeviceContext->Draw( model->mVertexCount, 0 );
}

Camera* Graphics::GetCamera() const
{
	return mCamera;
}

Camera* Graphics::GetDeveloperCamera() const
{
	return mDeveloperCamera;
}

void Graphics::ChangeCamera()
{
	if( mIsDeveloperCameraActive )
		mIsDeveloperCameraActive = false;
	else
		mIsDeveloperCameraActive = true;
}

void Graphics::ZoomInDeveloperCamera()
{
	mDeveloperCamera->ZoomIn();
}

void Graphics::ZoomOutDeveloperCamera()
{
	mDeveloperCamera->ZoomOut();
}

void Graphics::SetNDCSpaceCoordinates( float &mousePositionX, float &mousePositionY )
{
	//Calculate mouse position in NDC space
	mousePositionX	= ( ( 2.0f *  mousePositionX ) / mScreenWidth  - 1.0f );
	mousePositionY	= ( ( 2.0f * -mousePositionY ) / mScreenHeight + 1.0f );
}

void Graphics::SetInverseViewMatrix( DirectX::XMMATRIX &inverseViewMatrix )
{
	inverseViewMatrix = mCamera->GetInverseViewMatrix();
}

void Graphics::SetInverseProjectionMatrix( DirectX::XMMATRIX &projectionViewMatrix )
{
	projectionViewMatrix = mCamera->GetInverseProjectionMatrix();
}

void Graphics::SetEyePosition( DirectX::XMFLOAT3 &eyePosition )
{
	mCamera->SetEyePosition( eyePosition );
}

void Graphics::SetFocus( DirectX::XMFLOAT3 &focusPoint )
{
	mCamera->SetFocus( focusPoint );
}

//Clear canvas and prepare for rendering.
void Graphics::BeginScene()
{
	mDeviceContext->ClearState();

	if( mIsDeveloperCameraActive )
		mDeveloperCamera->Update();
	else
		mCamera->Update();

	/////////////////////////////////
	// Clear errything
	/////////////////////////////////
	static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//ID3D11ShaderResourceView* nullSRV[TEXTURES_AMOUNT] = { nullptr, nullptr, nullptr };
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clearColor );
	for( int i = 0; i < NUM_GBUFFERS; i++)
		mDeviceContext->ClearRenderTargetView( mGbuffers[i]->mRenderTargetView, clearColor );
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	/////////////////////////////////
	// Gbuffer generation pass
	/////////////////////////////////

	ID3D11RenderTargetView* rtvsToSet[NUM_GBUFFERS];
	for( int i = 0; i < NUM_GBUFFERS; i++)
		rtvsToSet[i] = mGbuffers[i]->mRenderTargetView;
	mDeviceContext->OMSetRenderTargets( 3, rtvsToSet, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );
	
	//mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, nullSRV );

	//Map CbufferPerFrame
	CbufferPerFrame data;

	if( mIsDeveloperCameraActive )
	{
		data.viewMatrix			= mDeveloperCamera->GetViewMatrix();
		data.projectionMatrix	= mDeveloperCamera->GetProjMatrix();
		data.cameraPosition		= mDeveloperCamera->GetPos();
	}
	else
	{
		data.viewMatrix			= mCamera->GetViewMatrix();
		data.projectionMatrix	= mCamera->GetProjMatrix();
		data.cameraPosition		= mCamera->GetPos();
	}
	MapBuffer( mCbufferPerFrame, &data, sizeof( CbufferPerFrame ) );

	mDeviceContext->VSSetConstantBuffers( 0, 1, &mCbufferPerFrame );
	mDeviceContext->PSSetSamplers( 0, 1, &mPointSamplerState );
	mDeviceContext->PSSetSamplers( 1, 1, &mLinearSamplerState );
}

//Finalize rendering.
void Graphics::EndScene()
{
	/////////////////////////////////
	// Deferred rendering pass
	/////////////////////////////////
	mDeviceContext->ClearState();

	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	
	for( int i = 0; i < NUM_GBUFFERS; i++)
		mDeviceContext->PSSetShaderResources( i, 1, &mGbuffers[i]->mShaderResourceView );

		//mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mDeferredPassEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mDeferredPassEffect->GetPixelShader(), nullptr, 0 );

	mDeviceContext->PSSetSamplers( 0, 1, &mPointSamplerState );
	mDeviceContext->PSSetSamplers( 1, 1, &mLinearSamplerState );

	mDeviceContext->PSSetConstantBuffers( 0, 1, &mCbufferPerFrame );

	mDeviceContext->Draw( 4, 0 );

	mSwapChain->Present( 0, 0 );
}

UINT Graphics::QueryMemoryUsed()
{
	IDXGIDevice* dxgiDevice;
	HRESULT hr = mDevice->QueryInterface( __uuidof( IDXGIDevice ), (void**)&dxgiDevice );
	IDXGIAdapter* adapter;
	dxgiDevice->GetAdapter( &adapter );
	DXGI_ADAPTER_DESC adapterDesc;
	adapter->GetDesc( &adapterDesc );
	return (UINT)adapterDesc.DedicatedVideoMemory;
}

//Singleton for the Graphics dll.
Graphics* Graphics::GetInstance()
{
	static Graphics instance;
	return &instance;
}

//Initialize graphics interfaces.
HRESULT Graphics::Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight )
{
	mHWnd			= hWnd;
	mScreenWidth	= screenWidth;
	mScreenHeight	= screenHeight;

	HRESULT hr		= E_FAIL;



	/////////////////////////
	// CREATE SWAPCHAIN
	/////////////////////////
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_WARP,
										D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount							= 1;
	swapChainDesc.BufferDesc.Width						= screenWidth;
	swapChainDesc.BufferDesc.Height						= screenHeight;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow							= hWnd;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.Windowed								= true;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for(	UINT driverTypeIndex = 0;
			driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr );
			driverTypeIndex++ )
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverTypes[driverTypeIndex],
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevelsToTry,
			ARRAYSIZE( featureLevelsToTry ),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&mSwapChain,
			&mDevice,
			&initiatedFeatureLevel,
			&mDeviceContext );

	if( FAILED( hr ) )
		return hr;
	
	/////////////////////////////////////////////
	// CREATE RENDERTARGETVIEW & BackBufferUAV
	/////////////////////////////////////////////

	ID3D11Texture2D* backBuffer;
	if( SUCCEEDED( hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( backBuffer, nullptr, &mRenderTargetView );
		SAFE_RELEASE( backBuffer );
	}

	if( FAILED( hr ) )
		return hr;

	//////////////////////////////
	// CREATE DEPTHSTENCILVIEW
	//////////////////////////////
	D3D11_TEXTURE2D_DESC	textureDesc;
	ID3D11Texture2D*		depthStencil;

	textureDesc.Width				= screenWidth;
	textureDesc.Height				= screenHeight;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= 0;

	if( FAILED( hr = mDevice->CreateTexture2D( &textureDesc, nullptr, &depthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dDSV;
	ZeroMemory( &dDSV, sizeof( dDSV ) );
	dDSV.Format				= textureDesc.Format;
	dDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dDSV.Texture2D.MipSlice	= 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( depthStencil, &dDSV, &mDepthStencilView ) ) )
		return hr;

	SAFE_RELEASE( depthStencil );

	//////////////////////////////
	// CREATE DEPTHSTENCILSTATE DISABLE
	//////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory( &depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc) );

	depthDisabledStencilDesc.DepthEnable					= false;
	depthDisabledStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable					= false;
	depthDisabledStencilDesc.StencilReadMask				= 0xFF;
	depthDisabledStencilDesc.StencilWriteMask				= 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if ( FAILED( hr = mDevice->CreateDepthStencilState( &depthDisabledStencilDesc, &mDepthDisabledStencilState ) ) )
		return hr;

	//////////////////////////////
	// CREATE DEPTHSTENCILSTATE ENABLE
	//////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthEnabledStencilDesc;
	ZeroMemory( &depthEnabledStencilDesc, sizeof(depthEnabledStencilDesc) );

	depthDisabledStencilDesc.DepthEnable					= true;
	depthDisabledStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable					= false;
	depthDisabledStencilDesc.StencilReadMask				= 0xFF;
	depthDisabledStencilDesc.StencilWriteMask				= 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if ( FAILED( hr = mDevice->CreateDepthStencilState( &depthDisabledStencilDesc, &mDepthEnabledStencilState ) ) )
		return hr;

	//////////////////////////////
	// CREATE VIEWPORT
	//////////////////////////////
	mStandardView.Height	= (float)screenHeight;
	mStandardView.Width		= (float)screenWidth;
	mStandardView.MaxDepth	= 1.0f;
	mStandardView.MinDepth	= 0.0f;
	mStandardView.TopLeftX	= 0.0f;
	mStandardView.TopLeftY	= 0.0f;

	//////////////////////////////
	// CREATE POINT SAMPLER
	//////////////////////////////
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxAnisotropy	= 1;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD			= 0.0f;
	samplerDesc.MipLODBias		= 0.0f;

	if( FAILED( hr = mDevice->CreateSamplerState( &samplerDesc, &mPointSamplerState ) ) )
		return hr;

	//////////////////////////////
	// CREATE LINEAR SAMPLER
	//////////////////////////////
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	if( FAILED( hr = mDevice->CreateSamplerState( &samplerDesc, &mLinearSamplerState ) ) )
		return hr;

	///////////////////////////////
	// CREATE CBUFFERPERFRAME
	///////////////////////////////
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth		= sizeof( CbufferPerFrame );
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerFrame ) ) )
		return hr;

	///////////////////////////////
	// CREATE CBUFFERPEROBJECT
	///////////////////////////////
	bufferDesc.ByteWidth = sizeof( CbufferPerObject );

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerObject ) ) )
		return hr;

	///////////////////////////////////////
	// CREATE CBUFFERPEROBJECTANIMATED
	///////////////////////////////////////
	bufferDesc.ByteWidth				= sizeof( CbufferPerObjectAnimated );

	hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerObjectAnimated );

	//AssetManager
	mAssetManager = new AssetManager;
	mAssetManager->Initialize( mDevice, mDeviceContext );

	//Effect
	mStaticEffect	= new Effect;

	EffectInfo effectInfo;
	ZeroMemory( &effectInfo, sizeof( EffectInfo ) );
	effectInfo.fileName					= "../Content/Effects/Static3dEffect.hlsl";
	effectInfo.vertexType				= STATIC_VERTEX_TYPE;
	effectInfo.isVertexShaderIncluded	= true;
	effectInfo.isPixelShaderIncluded	= true;

	if( FAILED( hr = mStaticEffect->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	m2dEffect			= new Effect;
	effectInfo.fileName = "../Content/Effects/2dEffect.hlsl";

	if( FAILED( hr = m2dEffect->Intialize( mDevice, &effectInfo ) ) )
		return hr;


	mAnimatedEffect			= new Effect;
	effectInfo.fileName		= "../Content/Effects/Animated3dEffect.hlsl";
	effectInfo.vertexType	= ANIMATED_VERTEX_TYPE;

	if( FAILED( hr = mAnimatedEffect->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	
	mDeferredPassEffect = new Effect;
	effectInfo.fileName		= "../Content/Effects/DeferredPassEffect.hlsl";
	effectInfo.vertexType	= STATIC_VERTEX_TYPE;

	if( FAILED( hr = mDeferredPassEffect->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//Gbuffers
	for( int i = 0; i < NUM_GBUFFERS; i++ )
	{
		mGbuffers[i] = new Gbuffer;
		mGbuffers[i]->Initialize( mDevice, mScreenWidth, mScreenHeight );
	}
	//Camera
	mCamera = new Camera;

	CameraInfo cameraInfo;
	ZeroMemory( &cameraInfo, sizeof( cameraInfo ) );
	cameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 27.0f, -22.25f, 0.0f );
	cameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	cameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	cameraInfo.width		= (float)screenWidth;
	cameraInfo.height		= (float)screenHeight;
	cameraInfo.foVY			= 0.75f;
	cameraInfo.nearZ		= 0.1f;
	cameraInfo.farZ			= 1000.0f;

	if( FAILED( hr = mCamera->Initialize( &cameraInfo ) ) )
		return hr;

	//Developer Camera
	mDeveloperCamera = new Camera;

	CameraInfo developerCameraInfo;
	ZeroMemory( &cameraInfo, sizeof( developerCameraInfo ) );
	developerCameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 50.0f, -50.0f, 0.0f );
	developerCameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	developerCameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	developerCameraInfo.width		= (float)screenWidth;
	developerCameraInfo.height		= (float)screenHeight;
	developerCameraInfo.foVY		= 0.75f;
	developerCameraInfo.nearZ		= 0.1f;
	developerCameraInfo.farZ		= 1000.0f;

	hr = mDeveloperCamera->Initialize( &developerCameraInfo );


	/////////////////////////
	// INITIATE VERTEXBUFFER FOR 2D
	/////////////////////////
	float position[3]	= { 1.0f, 1.0f, 0.0f };
	float normal[3]		= { 0.0f, 0.0f, 0.0f };
	float tangent[3]	= { 0.0f, 0.0f, 0.0f };
	float uv[2]			= { 1.0f, 1.0f };

	StaticVertex vert[4];

	for ( int k = 0; k < 4; k++ )
	{
		for ( int i = 0; i < 3; i++ )
		{
			vert[k].position[i] = position[i];
			vert[k].normal[i]	= normal[i];
			vert[k].tangent[i]	= tangent[i];
		}
		for ( int i = 0; i < 2; i++ )
		{
			vert[k].uv[i] = uv[i];
		}
	}


	StaticVertex vertices[4] = { vert[0], vert[1], vert[2], vert[3] }; //Kanske måste fyllas med data
	D3D11_BUFFER_DESC bufferDesc2d;
	ZeroMemory( &bufferDesc2d, sizeof(bufferDesc2d) );
	bufferDesc2d.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc2d.ByteWidth		= sizeof(StaticVertex) * 4;
	bufferDesc2d.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc2d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vertices;

	hr = mDevice->CreateBuffer( &bufferDesc2d, &subData, &mVertexBuffer2d );
	if ( FAILED( hr ) )
	{
		//Failed to create vertex buffer
		return hr;
	}

	return hr;
}

//Release all the stuff.
void Graphics::Release()
{
	SAFE_RELEASE( mVertexBuffer2d );
	SAFE_RELEASE( mSwapChain );
	SAFE_RELEASE( mDevice );
	SAFE_RELEASE( mDeviceContext );

	SAFE_RELEASE( mRenderTargetView );
	SAFE_RELEASE( mDepthStencilView );
	SAFE_RELEASE( mDepthDisabledStencilState );
	SAFE_RELEASE( mDepthEnabledStencilState );
	SAFE_RELEASE( mCbufferPerFrame );
	SAFE_RELEASE( mCbufferPerObject );
	SAFE_RELEASE( mCbufferPerObjectAnimated );

	SAFE_RELEASE( mPointSamplerState );
	SAFE_RELEASE( mLinearSamplerState );

	mAssetManager->Release();
	mStaticEffect->Release();
	mAnimatedEffect->Release();
	m2dEffect->Release();
	mDeferredPassEffect->Release();
	mCamera->Release();
	mDeveloperCamera->Release();
	for( int i = 0; i < NUM_GBUFFERS; i++ )
	{
		mGbuffers[i]->Release();
		SAFE_DELETE( mGbuffers[i] );
	}

	SAFE_DELETE( mAssetManager );
	SAFE_DELETE( mStaticEffect );
	SAFE_DELETE( mAnimatedEffect );
	SAFE_DELETE( m2dEffect );
	SAFE_DELETE( mDeferredPassEffect );
	SAFE_DELETE( mCamera );
	SAFE_DELETE( mDeveloperCamera );
}