#ifndef IMPORTANIMATION_H
#define IMPORTANIMATION_H

#include <vector>

struct animationData
{
	int nrOfJoints;
	char animationName[128];
	std::vector<jointAnimation> joints;
};
struct jointAnimation
{
	char jointName[128];
	char parentName[128];
	std::vector<int> keys;
	std::vector<animationMatrix> matricies;
};
struct animationMatrix
{
	double matrixData[4][4];
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
		animationData ImportBinaryAnimData(char* path);
};

#endif