#ifndef PATHFINDER_H
#define PATHFINDER_H

#pragma once
#include "Paths.h"
#include "Navmesh.h"
#include "NodeGraph.h"
#define MAX_PATHS 100

class Map;

class Pathfinder
{
	private:
		Navmesh** mNavmeshMap;
		NodeGraph* mNodeGraph;
		Map* mMap;
		Path mPaths[MAX_PATHS];

		int mNrOfPaths;
		UINT mMapWidth;
		UINT mMapHeight;

	protected:
	public:
	
	private:
		Pathfinder();
	protected:
	public:
		static Pathfinder* instance;
		HRESULT Initialize( Map* map );

		void RequestPath( Path* path, DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end );
		//If nrOfSteps left at 0, whole path will be calculated ( atm always leave this as 0 )
		bool CalculateSubPath( Path* path, int nrOfSteps = 0 );
		static Pathfinder* GetInstance();
		void Release();
		~Pathfinder();
};
#endif
