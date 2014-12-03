#include "ImporterAnim.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////


AnimationData ImporterAnim::ImportBinaryAnimData( string directoryPath,string fileName )
{
	streampos size;
	char * memblock;

	ifstream file;

	//this is how the final code should look!
	file.open( directoryPath + fileName, ios::in | ios::binary | ios::ate);

	//string fullPath = "myHeartIsInPieces.PaMan";

	//file.open( fullPath, ios::in | ios::binary | ios::ate );
	AnimationData tempAnim;

	int lastindex = fileName.find_last_of(".");
	string rawName = fileName.substr(0, lastindex);

	tempAnim.animationName = rawName;

	if ( file.is_open() )
	{
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

		cout << "File read" << endl;

		int padding = 0;
		tempAnim.nrOfJoints = memblock[padding];
		//memblock should contain nr of joints
		for ( int j = 0; j < memblock[padding]; j++ )
		{
			JointAnimation tempJoint;

			if ( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			for ( int i = 0; i < memblock[padding]; i++ )
			{
				if ( i == 0 )
				{
					padding++;
				}
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			for ( int i = 0; i < memblock[padding]; i++ )
			{
				if ( i == 0 )
				{
					padding++;
				}
				tempParentName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.parentName = tempParentName;

			int keys = memblock[padding];
			padding++;
			for ( int k = 0; k < keys; k++ )
			{
				tempJoint.keys.push_back( memblock[padding] );
				padding++;

				AnimationMatrix tempMatrix;
				for ( int m = 0; m < 4; m++ )
				{
					for ( int n = 0; n < 4; n++ )
					{
						int tempCounterValue = (int)memblock[padding];
						string tempDouble;
						for ( int w = 0; w < tempCounterValue; w++ )
						{
							if ( w == 0 )
							{
								padding++;
							}
							tempDouble.push_back( memblock[padding] );
							padding++;

						}
						tempMatrix.matrixData[m][n] = stod( tempDouble );
					}
				}
				tempJoint.matricies.push_back( tempMatrix );
			}
			tempAnim.joints.push_back( tempJoint );
		}
		delete[] memblock;
	}
	else cout << "Error opening file!" << endl;
	return tempAnim;
}

Skeleton ImporterAnim::ImportBinarySkelData( string directoryPath,string fileName )
{
	streampos size;
	char * memblock;

	ifstream file;

	//this is how the final code should look!
	file.open( directoryPath + fileName, ios::in | ios::binary | ios::ate );

	Skeleton tempSkel;

	int lastindex = fileName.find_last_of(".");
	string rawName = fileName.substr(0, lastindex);

	tempSkel.skeletonName = rawName;

	if ( file.is_open() )
	{
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

		cout << "File read" << endl;

		int padding = 0;
		tempSkel.nrOfJoints = memblock[padding];
		//memblock should contain nr of joints
		for ( int j = 0; j < memblock[padding]; j++ )
		{
			Joint tempJoint;

			if ( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			for ( int i = 0; i < memblock[padding]; i++ )
			{
				if ( i == 0 )
				{
					padding++;
				}
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			int parentFor = memblock[padding];
			for ( int i = 0; i < parentFor; i++ )
			{
				if ( i == 0 )
				{
					padding++;
				}
				tempParentName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.parentName = tempParentName;

			AnimationMatrix tempMatrix;
			for ( int m = 0; m < 4; m++ )
			{
				for ( int n = 0; n < 4; n++ )
				{
					int tempCounterValue = (int)memblock[padding];
					string tempDouble;
					for ( int w = 0; w < tempCounterValue; w++ )
					{
						if ( w == 0 )
						{
							padding++;
						}
						tempDouble.push_back( memblock[padding] );
						padding++;

					}
					tempMatrix.matrixData[m][n] = stod( tempDouble );
				}
			}
			tempJoint.originalMatrix = tempMatrix;
			tempSkel.joints.push_back( tempJoint );
		}
		delete[] memblock;
	}
	else cout << "Error opening file!" << endl;
	return tempSkel;
}


ImporterAnim::ImporterAnim()
{

}

ImporterAnim::~ImporterAnim()
{

}