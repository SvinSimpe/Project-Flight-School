#ifndef MAPNODEMANAGER_H
#define MAPNODEMANAGER_H
#include "MapNode.h"
#include <unordered_map>
#include "Events.h"
struct JMatrix
{
	char name[64];
	double transformation[16];
};

struct AssetInfo
{
	std::string		assetPath;
	CollisionType	collisionType;
	RenderType		renderType;

	AssetInfo()
	{
		collisionType	= FULL_COLLISION;
		renderType		= STATIC_RENDERTYPE;
	}
};

typedef std::unordered_map< std::string, std::vector<MapNode*> > NodeMap;

class MapNodeManager
{
	private:
		NodeMap mNodeMap;
	protected:
	public:

	private:
		void					ConvertToFloat( XMFLOAT4X4& dest, double* source );
		void					writeToLog( const std::string &test );
	protected:
	public:
		static MapNodeManager*	instance;
		MapNode*				CreateNode( const char* fileName, std::unordered_map<AssetID, AssetInfo>& assetMap );
		NodeMap					GetNodes();
		void					LoadLevel( std::string filePath );
		static MapNodeManager*	GetInstance();
		HRESULT					Initialize();
		void					Release();
								MapNodeManager();
		virtual					~MapNodeManager();
};
#endif
