#ifndef MAPNODEMANAGER_H
#define MAPNODEMANAGER_H
#include "MapNode.h"
#include <unordered_map>
#include "EventManager.h"
#include "Events.h"
struct JMatrix
{
	char name[50];
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
};

typedef std::unordered_map< std::string, std::vector<MapNode*> > NodeMap;

class MapNodeManager
{
	private:
		NodeMap mNodeMap;
	protected:
	public:

	private:
		MapNode* createNode( const char* filePath );
		void writeToLog( const std::string &test );
	protected:
	public:
		static		MapNodeManager* instance;

		NodeMap GetNodes();
		void LoadLevel( std::string filePath );
		static MapNodeManager* GetInstance();
		HRESULT		Initialize();
		void		Release();
					MapNodeManager();
		virtual		~MapNodeManager();
};
#endif
