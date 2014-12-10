#ifndef ASSETBASE_H
#define ASSETBASE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; ( x ) = nullptr; }

typedef UINT AssetID;

struct JointAnimation
{
	std::string							jointName;
	std::string							parentName;
	int									parentIndex;
	std::vector<int>					keys;
	std::vector<DirectX::XMFLOAT4X4>	matricies;
};

struct AnimationData
{
	int								nrOfJoints;
	int								AnimLength;
	std::string						animationName;
	std::vector<JointAnimation>		joints;
};

struct Joint
{
	std::string				jointName;
	std::string				parentName;
	int						parentIndex;
	DirectX::XMFLOAT4X4		originalMatrix;
};

struct Skeleton
{
	int					nrOfJoints;
	std::string			skeletonName;
	std::vector<Joint>	joints;
};

struct AssetBase
{
	private:
	protected:
	public:
		AssetID				mAssetId;
		std::string			mFileName;

	private:
	protected:
	public:
		std::string			GetFileName();
		virtual	HRESULT		Initialize()	= 0;
		virtual void		Release()		= 0;
							AssetBase();
		virtual				~AssetBase();

};
#endif