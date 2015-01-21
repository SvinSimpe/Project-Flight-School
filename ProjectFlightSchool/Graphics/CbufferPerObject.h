#ifndef CBUFFERPEROBJECT_H
#define CBUFFERPEROBJECT_H

#include <DirectXMath.h>

#define NUM_SUPPORTED_JOINTS 16
#define NUM_SUPPORTED_ANIM_INSTANCES 60

struct CbufferPerObject
{
	DirectX::XMMATRIX worldMatrix;
};

struct CbufferPerObjectAnimated
{
	DirectX::XMMATRIX boneTransforms[NUM_SUPPORTED_JOINTS];
};

struct CbufferPerInstancedAnimated
{
	CbufferPerObject perInstance[NUM_SUPPORTED_ANIM_INSTANCES];
};
#endif