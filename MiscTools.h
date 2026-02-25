#pragma once
#include "raylib.h"
#include "raymath.h"
class MiscTools
{
public:
	MiscTools();
	~MiscTools();
	static Vector2 AdjustVector2ByAngle(const Vector2 v, const float& angle) {
		float cosA = cosf(angle);
		float sinA = sinf(angle);

		float newX = v.x * cosA - v.y * sinA;
		float newY = v.x * sinA + v.y * cosA;
		return Vector2Normalize({ newX, newY });
	}


private:

};

MiscTools::MiscTools()
{
}

MiscTools::~MiscTools()
{
}