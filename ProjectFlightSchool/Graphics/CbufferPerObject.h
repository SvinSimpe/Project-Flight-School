#ifndef CBUFFERPEROBJECT_H
#define CBUFFERPEROBJECT_H

#include <DirectXMath.h>

struct CbufferPerObject
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMFLOAT4 strangeFloat;		//Alpha channel not currently used.
};
#endif