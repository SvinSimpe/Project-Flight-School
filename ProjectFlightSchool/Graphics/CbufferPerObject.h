#ifndef CBUFFERPEROBJECT_H
#define CBUFFERPEROBJECT_H

#include <DirectXMath.h>

#define NUM_SUPPORTED_JOINTS 32
#define NUM_SUPPORTED_ANIM_INSTANCES 32

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