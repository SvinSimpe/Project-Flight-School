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
	protected:
	public:

	//Class functions
	private:
	protected:
	public:
					MapPathImportHandler();
	virtual			~MapPathImportHandler();
	HRESULT			HandlePath( string folderPath );
};
#endif