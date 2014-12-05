#ifndef ANIMATIONASSET_H
#define ANIMATIONASSET_H

#include <vector>
#include <string>
#include <Windows.h>
#include <DirectXMath.h>
#include <iostream>
using namespace std;

struct JointAnimation
{
	string						jointName;
	string						parentName;
	int							parentIndex;
	vector<int>					keys;
	vector<DirectX::XMFLOAT4X4>	matricies;
};
struct AnimationData
{
	int							nrOfJoints;
	string						animationName;
	vector<JointAnimation>		joints;
};

struct Joint
{
	string				jointName;
	string				parentName;
	int					parentIndex;
	int					lastFrame;
	DirectX::XMFLOAT4X4	originalMatrix;
	DirectX::XMFLOAT4X4 previousMatrix;
};
struct Skeleton
{
	int				nrOfJoints;
	string			skeletonName;
	vector<Joint>	joints;
};

struct AnimationAsset
{
private:
protected:
public:
	Skeleton			mSkeleton;
	AnimationData		mAnimationData;

	DirectX::XMFLOAT4X4	mCurrentBoneTransforms[16];
	
	int					mCurrentFrame;

private:
protected:
public:
	void		ParentIndexer();
	void		ResetAnimation();
	void		UpdateAnimation( float deltaTime );

	HRESULT		Initialize();
	void		Release();
				AnimationAsset();
	virtual		~AnimationAsset();
};
#endif