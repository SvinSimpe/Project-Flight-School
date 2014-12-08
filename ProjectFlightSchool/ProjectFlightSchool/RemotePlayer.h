#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "Input.h"
#include "Player.h"

//struct BoundingBox
//{
//	XMFLOAT3	position;
//	float		width;
//	float		height;
//
//	bool Intersect(BoundingBox inBox) const
//	{
//		return ((position.x < inBox.position.x + inBox.width) && (position.x + width  > inBox.position.x) &&
//			(position.z < inBox.position.z + inBox.height) && (position.z + height > inBox.position.z));
//	}
//};
//
//struct BoundingCircle
//{
//	XMFLOAT3	center;
//	float		radius;
//
//	bool Intersect(BoundingCircle inCircle) const
//	{
//		return (pow(center.x - inCircle.center.x, 2) + pow(center.y - inCircle.center.y, 2)) < pow(radius + inCircle.radius, 2);
//	}
//};
//
//
//struct UpperBody
//{
//	UINT		playerModel;
//	XMFLOAT3	direction;
//	XMFLOAT3	position;
//};
//
//struct LowerBody
//{
//	UINT		playerModel;
//	XMFLOAT3	direction;
//	XMFLOAT3	position;
//	float		speed;
//};

class RemotePlayer
{
private:
	UpperBody	mUpperBody;
	LowerBody	mLowerBody;

private:
	void RemoteUpdate(IEventPtr newEvent);
	void LookAt(float rotation);

public:
	HRESULT Render(float deltaTime);

	HRESULT Initialize();
	void Release();
	RemotePlayer();
	~RemotePlayer();
};

#endif

