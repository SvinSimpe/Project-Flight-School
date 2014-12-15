#ifndef CBUFFERPEROBJECT_H
#define CBUFFERPEROBJECT_H

#include <DirectXMath.h>

#define NUM_SUPPORTED_JOINTS 32

struct CbufferPerObject
{
	DirectX::XMMATRIX worldMatrix;
};

struct CbufferPerObjectAnimated
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX boneTransforms[NUM_SUPPORTED_JOINTS];
};
#endif