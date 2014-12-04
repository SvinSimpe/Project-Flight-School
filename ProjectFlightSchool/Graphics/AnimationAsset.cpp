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
			mCurrentBoneTransforms[i] = mSkeleton.joints.at( i ).originalMatrix;
		}
		else
		{
			DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( i ).originalMatrix );
			DirectX::XMMATRIX parent	= DirectX::XMLoadFloat4x4( &mSkeleton.joints.at( mSkeleton.joints.at( i ).parentIndex ).originalMatrix );

			DirectX::XMStoreFloat4x4( &mCurrentBoneTransforms[i],  child * parent );
		}
	}
}

void AnimationAsset::UpdateAnimation( float deltaTime )
{

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