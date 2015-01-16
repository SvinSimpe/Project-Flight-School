#ifndef MAPNODEMANAGER_H
#define MAPNODEMANAGER_H
#include "MapNode.h"

struct JMatrix
{
	char name[50];
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
};
class MapNodeManager
{
	private:
		MapNode*	mNodes;
		int			mNrOfNodes;
	protected:
	public:

	private:
		HRESULT createNodes( char* filePath,  int nrOfNodes );
	protected:
	public:
		static		MapNodeManager* instance;

		MapNode* GetNodes();

		static MapNodeManager* GetInstance();
		HRESULT		Initialize( char* fileName );
		void		Release();
					MapNodeManager();
		virtual		~MapNodeManager();
};
#endif
