#ifndef IMPORTHANDLER_H
#define IMPORTHANDLER_H

#include <vector>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "ImporterAnim.h"

using namespace std;

class MapPathImportHandler
{
	//Class members
	private:
		vector<AnimationData>	mAnimationList;
		vector<Skeleton>		mSkeletonList;
	protected:
	public:

	//Class functions
	private:
	protected:
	public:
					MapPathImportHandler();
	virtual			~MapPathImportHandler();
	HRESULT			HandlePaManPath( string folderPath );
	HRESULT			HandleSkelPath( string folderPath );
};
#endif