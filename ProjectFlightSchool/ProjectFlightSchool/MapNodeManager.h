#ifndef MAPNODEMANAGER_H
#define MAPNODEMANAGER_H
#include "MapNode.h"
#include <unordered_map>
#include "EventManager.h"
#include "Events.h"
struct JMatrix
{
	char name[64];
	//-------------- Quaternion
	//XMFLOAT3 pos;
	//XMFLOAT4 rot;
	//XMFLOAT3 scale;
	//-------------- Quaternion
	double transformation[16];
};

typedef std::unordered_map< std::string, std::vector<MapNode*> > NodeMap;

class MapNodeManager
{
	private:
		NodeMap mNodeMap;
	protected:
	public:

	private:
		void ConvertToFloat( XMFLOAT4X4& dest, double* source );
		void writeToLog( const std::string &test );
	protected:
	public:
		static		MapNodeManager* instance;
		MapNode* CreateNode( const char* filePath );
		NodeMap GetNodes();
		void LoadLevel( std::string filePath );
		static MapNodeManager* GetInstance();
		HRESULT		Initialize();
		void		Release();
					MapNodeManager();
		virtual		~MapNodeManager();
};
#endif
