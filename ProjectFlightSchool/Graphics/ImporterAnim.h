#ifndef IMPORTANIMATION_H
#define IMPORTANIMATION_H

#include <vector>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;

struct AnimationMatrix
{
	double matrixData[4][4];
};
struct JointAnimation
{
	string							jointName;
	string							parentName;
	std::vector<int>				keys;
	std::vector<AnimationMatrix>	matricies;
};
struct AnimationData
{
	int								nrOfJoints;
	string							animationName;
	std::vector<JointAnimation>		joints;
};



class ImporterAnim
{
	//Class members
private:
protected:
public:

	//Class functions
private:
protected:
public:
	ImporterAnim();
	virtual			~ImporterAnim();
	AnimationData	ImportBinaryAnimData( string path );
};

#endif