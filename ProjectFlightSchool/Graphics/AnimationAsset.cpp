#include "AnimationAsset.h"

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

void AnimationAsset::ParentIndexer()
{
	for (int i = 0; i < (int)mAnimationData.joints.size(); i++)
	{
		if( strcmp( (char*)mAnimationData.joints.at( i ).parentName.c_str() , "root" ) == 0 )
		{
			mAnimationData.joints.at( i ).parentIndex = -1;
		}
		else
		{
			for( int j = 0; j < (int)mAnimationData.joints.size(); j++ )
			{
				if( strcmp( (char*)mAnimationData.joints.at( i ).parentName.c_str(), (char*)mAnimationData.joints.at( j ).jointName.c_str() ) == 0 )
				{
					mAnimationData.joints.at( i ).parentIndex = j;
					break;
				}
			}
		}
	}

	for (int i = 0; i < (int)mSkeleton.joints.size(); i++)
	{
		if( strcmp( (char*)mSkeleton.joints.at( i ).parentName.c_str() , "root" ) == 0 )
		{
			mSkeleton.joints.at( i ).parentIndex = -1;
		}
		else
		{
			for( int j = 0; j < (int)mSkeleton.joints.size(); j++ )
			{
				if( strcmp( (char*)mSkeleton.joints.at( i ).parentName.c_str(), (char*)mSkeleton.joints.at( j ).jointName.c_str() ) == 0 )
				{
					mSkeleton.joints.at( i ).parentIndex = j;
					break;
				}
			}
		}
	}
}

void AnimationAsset::ResetAnimation()
{
	for( int i = 0; i < (int)mSkeleton.joints.size(); i++ )
	{
		if( mSkeleton.joints.at( i ).parentIndex == -1 )
		{
			mCurrentBoneTransforms[i]					= mSkeleton.joints.at( i ).originalMatrix;
			mSkeleton.joints.at( i ).previousMatrix		= mSkeleton.joints.at( i ).originalMatrix;
		}
		else
		{
			DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).originalMatrix );
			DirectX::XMMATRIX parent	= DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mSkeleton.joints.at( i ).parentIndex] );

			DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i], child * parent );
			DirectX::XMStoreFloat4x4( &mSkeleton.joints.at( i ).previousMatrix, child );
		}
		mSkeleton.joints.at( i ).lastFrame = 1;
	}

	mCurrentFrame = 1;
}

void AnimationAsset::UpdateAnimation( float deltaTime )
{
	mCurrentFrame++;
	for( int i = 0; i < (int)mSkeleton.joints.size(); i++ )
	{
		for( int j = 0; j < (int)mAnimationData.joints.at( i ).keys.size(); j++ )
		{
			//If no keyframes exist apply static pose
			if( mAnimationData.joints.at( i ).keys.size() == 1 && mAnimationData.joints.at( i ).keys.at( j ) == 0 )
			{
				if( mSkeleton.joints.at( i ).parentIndex == -1 )
				{
					mCurrentBoneTransforms[i] = mAnimationData.joints.at( i ).matricies.at( 0 );
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mAnimationData.joints.at( i ).matricies.at( 0 ) );
					DirectX::XMMATRIX parent	= DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i], child * parent );
				}
			}
			//Find next keyframe and interpolate previousMatrix with next matrix in animation based on key.
			else
			{
				if( mAnimationData.joints.at( i ).keys.at( j ) == mCurrentFrame )
				{
					mSkeleton.joints.at( i ).previousMatrix		= mAnimationData.joints.at( i ).matricies.at( j );
					mSkeleton.joints.at( i ).lastFrame			= mCurrentFrame;

					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).previousMatrix );
					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i], child * parent );
					break;
				}
				else if( mAnimationData.joints.at( i ).keys.at( j ) > mCurrentFrame )
				{
					int key = mAnimationData.joints.at( i ).keys.at( j );

					float interpolation					=	(float)( mCurrentFrame - mSkeleton.joints.at( i ).lastFrame ) /
															(float)( mAnimationData.joints.at( i ).keys.at( j ) - mSkeleton.joints.at( i ).lastFrame );
					DirectX::XMMATRIX targetMatrix		= DirectX::XMLoadFloat4x4( &mAnimationData.joints.at( i ).matricies.at( j ) );
					DirectX::XMMATRIX child				= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).previousMatrix );
					
					DirectX::XMVECTOR targetComp[3];
					DirectX::XMVECTOR childComp[3];

					DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], targetMatrix );
					DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

					child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
																	DirectX::XMVectorZero(),
																	DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
																	DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );			

					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i], child * parent );
					break;
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).previousMatrix );
					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i], child * parent );
				}
			}
		}
	}

	if( mCurrentFrame > 60 )
		ResetAnimation();
}

HRESULT AnimationAsset::Initialize()
{
	return S_OK;
}

void AnimationAsset::Release()
{

}

AnimationAsset::AnimationAsset()
{

}

AnimationAsset::~AnimationAsset()
{

}