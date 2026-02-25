/////////////
// Enemy.h //
/////////////

#pragma once
#include "GameObject.h"
#include "RenderSystem.h"
#include "Player.h"
#include "raylib.h"
#include "MiscTools.h"
#include <iostream>

class Enemy : public GameObject
{
	RenderSystem& renderSystem;
	Player& player; //
	

public:
	Enemy(
		Vector2 startPos,
		Vector2 _size,
		float _speed,
		RenderSystem& _renderSystem,
		Player& _player,
		size_t _frameCount,
		float _animFps
	);
	~Enemy();

	void OnFrameUpdate(const float& dT) override;
	void FixDrawData() override;
	void setPtrToTexture(Texture2D* ptr) {
		drawData.ptrToTexture = ptr;
	}
	void RandomizeSkew()
	{
		float denominator = GetRandomValue(2.0f , 8.0f);
		skew = PI / denominator;
	}
	size_t GetCurrentFrame() {
		return currentFrame;
	}

private:
	float speed;
	float skew = 0.0f;
	size_t currentFrame = 0;
	size_t frameCount = 1;

	float animFps = 4.0f;
	float animTimer = 0.0f;
	float animInterval = 1.0f / animFps;

	float skewTimer = 0.0f;
	float skewInterval = 2.0f;
	
};


Enemy::Enemy(
	Vector2 startPos,
	Vector2 _size,
	float _speed,
	RenderSystem& _renderSystem,
	Player& _player,
	size_t _frameCount,
	float _animFps
) : 
	speed(_speed),
	renderSystem(_renderSystem),
	player(_player),
	frameCount(_frameCount),
	animFps(_animFps)

{
	position = startPos;
	size = _size;
	drawData.size = size;

	if (animFps > 0) {
		animInterval = 1.0f / animFps;
	}

	skewInterval = GetRandomValue(1.0f, 3.0f);

	FixDrawData();
}



Enemy::~Enemy()
{
	// 
}

void Enemy::OnFrameUpdate(const float& dT)
{
	

	Vector2 targetPos = player.GetPosition();
	Vector2 direction = Vector2Subtract(targetPos, position);

	direction = Vector2Normalize(direction);
	direction = MiscTools::AdjustVector2ByAngle(direction, skew);

	float distanceThisFrame = speed * dT;
	lookAngle = Vector2Angle({ 1,0 }, direction);
	position = Vector2Add(position, Vector2Scale(direction, distanceThisFrame));
	FixDrawData();

	animTimer += dT;
	skewTimer += dT;

	if (animTimer >= animInterval) {
		animTimer = 0.0f;
		currentFrame = (currentFrame + 1) % frameCount;
	}

	if (skewTimer >= skewInterval) {
		skewTimer = 0.0f;
		RandomizeSkew();
	}

}
void Enemy::FixDrawData()
{
	drawData.position = position;
	drawData.lookAngle = lookAngle;

}
