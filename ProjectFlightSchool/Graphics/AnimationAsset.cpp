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

			DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i],  child * parent );
			mSkeleton.joints.at( i ).previousMatrix = mCurrentBoneTransforms[i];
		}
	}

	mLastFrame = mCurrentFrame = 1;
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
					mCurrentBoneTransforms[i]					= mAnimationData.joints.at( i ).matricies.at( 0 );
					mSkeleton.joints.at( i ).previousMatrix		= mAnimationData.joints.at( i ).matricies.at( 0 );
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mAnimationData.joints.at( i ).matricies.at( 0 ) );
					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i],  child * parent );
					mSkeleton.joints.at( i ).previousMatrix = mCurrentBoneTransforms[i];
				}
			}
			//Find next keyframe and interpolate previousMatrix with next matrix in animation based on key.
			else
			{
				if( mAnimationData.joints.at( i ).keys.at( j ) == mCurrentFrame )
				{
					mSkeleton.joints.at( i ).previousMatrix		= mAnimationData.joints.at( i ).matricies.at( j );
					mLastFrame									= mCurrentFrame;

					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).previousMatrix );
					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );

					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i],  child * parent );
					break;
				}
				else if( mAnimationData.joints.at( i ).keys.at( j ) > mCurrentFrame )
				{
					float interpolation					= (float)( mCurrentFrame - mLastFrame ) / (float)( mAnimationData.joints.at( i ).keys.at( j ) - mLastFrame );
					DirectX::XMMATRIX targetMatrix		= DirectX::XMLoadFloat4x4( &mAnimationData.joints.at( i ).matricies.at( j ) );

					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).previousMatrix );
					child						= child * ( 1.0f - interpolation ) + targetMatrix * interpolation;

					DirectX::XMMATRIX parent	= mAnimationData.joints.at( i ).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &mCurrentBoneTransforms[mAnimationData.joints.at( i ).parentIndex] );



					DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i],  child * parent );
					cout << mCurrentBoneTransforms[i]._11 << endl;
					break;
				}
			}
		}
	}
	if( mCurrentFrame > 90 )
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