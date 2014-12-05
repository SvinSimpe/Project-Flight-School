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
	string						jointName;
	string						parentName;
	vector<int>					keys;
	vector<AnimationMatrix>		matricies;
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
	AnimationMatrix		originalMatrix;
};
struct Skeleton
{
	int				nrOfJoints;
	string			skeletonName;
	vector<Joint>	joints;
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
	AnimationData	ImportBinaryAnimData( string directoryPath, string fileName );
	Skeleton		ImportBinarySkelData( string directoryPath, string fileName );
};

#endif