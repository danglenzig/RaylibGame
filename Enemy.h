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
#include <memory>

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
		float _animFps,
		std::string _uniqueID
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

	bool followPlayer = true;
	size_t playerAttackHandle;
	
	std::string uniqueID;

	std::vector<Vector2> gatherPositions = {
		{100,100},
		{1180,100},
		{100,620},
		{1180,620}
	};
	Vector2 gatherPos = {100,100};
	void HandlePlayerAttack(const DamageSectorData& data);
	
};


Enemy::Enemy(
	Vector2 startPos,
	Vector2 _size,
	float _speed,
	RenderSystem& _renderSystem,
	Player& _player,
	size_t _frameCount,
	float _animFps,
	std::string _uniqueID
) : 
	speed(_speed),
	renderSystem(_renderSystem),
	player(_player),
	frameCount(_frameCount),
	animFps(_animFps),
	uniqueID(_uniqueID)

{
	position = startPos;
	size = _size;
	drawData.size = size;

	if (animFps > 0) {
		animInterval = 1.0f / animFps;
	}
	skewInterval = GetRandomValue(1.0f, 3.0f);
	playerAttackHandle = player.AddAttackListener(
		[this](const DamageSectorData& data) {
			HandlePlayerAttack(data);
		}
	);
	uniqueID = _uniqueID;
	FixDrawData();
}



Enemy::~Enemy()
{
	player.RemoveAttackListener(playerAttackHandle);
}

void Enemy::OnFrameUpdate(const float& dT)
{
	Vector2 targetPos = gatherPos;
	
	if (followPlayer) {
		targetPos = player.GetPosition();
	}

	
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

		size_t randomInteger = GetRandomValue(0, 2);
		followPlayer = randomInteger < 2;
		gatherPos = gatherPositions[GetRandomValue(0, gatherPositions.size() - 1)];
	}

}
void Enemy::FixDrawData()
{
	drawData.position = position;
	drawData.lookAngle = lookAngle;

}
void Enemy::HandlePlayerAttack(const DamageSectorData& data)
{
	// Check if this enemy is within the damage sector defined by the player's attack
	// How do I know if my current is inside the area defined by the sector data?
	// 1. Check if the enemy is within the radius of the sector
	bool withinRadius = Vector2DistanceSqr(position, data.position) <= (data.radius * data.radius);
	bool withinAngle = false;
	// 2. Check if the enemy is within the angle of the sector
	// How?
	// Get the direction from the center of the sector to the enemy
	Vector2 directionToEnemy = Vector2Subtract(position, data.position);
	directionToEnemy = Vector2Normalize(directionToEnemy);
	// Now what? I have the direction from the center of the sector to the enemy,
	// and I have the angle range of the sector (data.angleStart to data.angleEnd)
	// I can get the angle of the directionToEnemy vector using atan2
	float angleToEnemy = atan2f(directionToEnemy.y, directionToEnemy.x);
	// Now what? I have the angle to the enemy, and I have the angle range of the sector
	// I need to check if angleToEnemy is between data.angleStart and data.angleEnd
	// How? I need to account for the fact that angles can wrap around at 2*PI,
	// so I can't just check if angleToEnemy is between data.angleStart and data.angleEnd
	// I need to check if the angle range of the sector wraps around the 0 angle
	bool angleRangeWraps = data.angleEnd < data.angleStart;
	// Now what? If the angle range wraps, then the enemy is within the sector if its angle
	// is greater than data.angleStart or less than data.angleEnd.
	// If the angle range does not wrap, then what?
	// 
	if (angleRangeWraps) {
		withinAngle = (angleToEnemy > data.angleStart || angleToEnemy < data.angleEnd);
	}
	else {
		withinAngle = (angleToEnemy >= data.angleStart && angleToEnemy <= data.angleEnd);
	}
	if (withinRadius && withinAngle) {
		// Take damage!

		// Later: tell the CombatSystem that the enemy with my uniqueID
		// has been killed.
		// It will emit an event that the EnemySystem will catch
		// and will clean up its list.

		std::cout << uniqueID << " says OUCH!\n";

	}
}