#ifndef PATHS_H
#define PATHS_H

#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>

#define MAX_PATH_LENGTH 20

class SubPath
{
	private:
	protected:
	public:
		DirectX::XMFLOAT3 mStart;
		DirectX::XMFLOAT3 mEnd;

		int mNavMeshIndex;

		bool mIsCalculated;
		std::vector<DirectX::XMFLOAT2> mPoints;



	private:
	protected:
	public:
		HRESULT Initialize( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, int navMeshIndex );

		SubPath();
		~SubPath();

		bool operator==( const SubPath& origOb )const;
};

class Path
{
	private:
	protected:
	public:	
		int mNrOfSubPaths;
		int mCurrentSubPath;
		SubPath mSubPaths [100];

	private:
	protected:
	public:
		std::vector<DirectX::XMFLOAT2> TotalPath();
		bool AddSubPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, int navMeshIndex );
		SubPath* GetNextSubPath();
		Path();
		~Path();
};
#endif

