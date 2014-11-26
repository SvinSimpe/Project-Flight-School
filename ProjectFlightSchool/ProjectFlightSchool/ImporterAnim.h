#ifndef IMPORTANIMATION_H
#define IMPORTANIMATION_H

#include <vector>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;

struct animationMatrix
{
	double matrixData[4][4];
};
struct jointAnimation
{
	string							jointName;
	string							parentName;
	std::vector<int>				keys;
	std::vector<animationMatrix>	matricies;
};
struct animationData
{
	int								nrOfJoints;
	string							animationName;
	std::vector<jointAnimation>		joints;
};



class ImportAnimation
{
	//Class members
private:
	animationData mTempAnimData;

protected:
public:

	//Class functions
private:
protected:
public:
	ImportAnimation();
	virtual			~ImportAnimation();
	animationData	ImportBinaryAnimData(string path);
};

#endif