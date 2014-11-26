#include "ImporterAnim.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
ImportAnimation::ImportAnimation()
{

}

ImportAnimation::~ImportAnimation()
{

}

animationData ImportAnimation::ImportBinaryAnimData(string path)
{
	streampos size;
	char * memblock;

	ifstream file;

	//file.open(path, ios::in | ios::binary | ios::ate);
	string fullPath = "myHeartIsInPieces.PaMan";

	file.open("myHeartIsInPieces.PaMan", ios::in | ios::binary | ios::ate);
	animationData tempAnim;

	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[size];
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		file.close();

		cout << "hela filen inläst" << endl;



		int padding = 0;
		tempAnim.nrOfJoints = memblock[padding];
		//nr of joints
		for (int j = 0; j < memblock[padding]; j++)
		{
			jointAnimation tempJoint;

			if (j == 0)
				padding++;

			//jointName
			string tempName;
			for (int i = 0; i < memblock[padding]; i++)
			{
				if (i == 0)
				{
					padding++;
				}
				tempName.push_back(memblock[padding]);
				padding++;
			}
			tempJoint.jointName = tempName;

			//parent
			string tempParentName;
			for (int i = 0; i < memblock[padding]; i++)
			{
				if (i == 0)
				{
					padding++;
				}
				tempParentName.push_back(memblock[padding]);
				padding++;
			}
			tempJoint.parentName = tempParentName;

			int Keys = memblock[padding];
			padding++;
			for (int k = 0; k < Keys; k++)
			{
				tempJoint.keys.push_back(memblock[padding]);
				padding++;

				animationMatrix tempMatrix;
				for (int m = 0; m < 4; m++)
				{
					for (int n = 0; n < 4; n++)
					{
						int tempCounterValue = (int)memblock[padding];
						string tempDouble;
						for (int n = 0; n < tempCounterValue; n++)
						{
							if (n == 0)
							{
								padding++;
							}
							tempDouble.push_back(memblock[padding]);
							padding++;

						}
						tempMatrix.matrixData[m][n] = stod(tempDouble);
					}
				}
				tempJoint.matricies.push_back(tempMatrix);
			}
			tempAnim.joints.push_back(tempJoint);
		}

		/*	for (int i = 0; i < size; i++)
		{
		cout << memblock[i];
		}*/
		cout << endl;
		delete[] memblock;
	}
	else cout << "kunde ej öppna fil" << endl;
	return tempAnim;
}