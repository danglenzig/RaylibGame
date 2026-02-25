///////////////////
// EnemySystem.h //
///////////////////

#pragma once
#include "Player.h"
#include "raymath.h"
#include "UpdateRelay.h"
#include "RenderSystem.h"
#include "Enemy.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <thread>
#include <memory>

enum EnumEnemy
{
	SKEETER,
	ZOMBIE
};



struct EnemyAnimation
{
	EnumEnemy enemyName;
	size_t numberOfFrames = 1;
	float fps = 1.0f;
	float interval = 1.0f;
	size_t currentFrame = 0;
	float timer = 0.0f;
	bool loop = true;
};
struct EnemyAnimationConfig
{
	EnumEnemy enemyName;
	std::vector<std::string> framePaths;
	float fps = 1.0f;
};



class EnemySystem
{
	RenderSystem& renderSystem;
	Player& player;
public:
	EnemySystem(
		RenderSystem& _renderSystem,
		Player& player
	);
	~EnemySystem();

	void RenderEnemies();
	void SpawnEnemy(EnumEnemy enemyType, Vector2 spawnPos);
	void KillEnemy(Enemy* ptrToEnemy);
	size_t GetEnemyCount() const {
		return enemies.size();
	}
	

	Texture2D* GetPtrToTexture(EnumEnemy enemyType, size_t frame) {
		if (enemyFramesPool.find(enemyType) == enemyFramesPool.end()) {
			std::cerr << "Enemy type not found in frame pool: " << static_cast<int>(enemyType) << "\n";
			return nullptr;
		}
		if (frame >= enemyFramesPool[enemyType].size()) {
			std::cerr << "Frame index out of bounds for enemy type: " << static_cast<int>(enemyType) << ", frame: " << frame << "\n";
			return nullptr;
		}
		return &enemyFramesPool[enemyType][frame];
	}

private:
	

	std::unordered_map<EnumEnemy, std::vector<Texture2D>> enemyFramesPool;
	std::vector<EnemyAnimationConfig> animConfigs;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<Vector2> spawnPoints = {
		{0,0},
		{320,0},
		{640,0},
		{960,0},
		{1280,0},
		{0,720},
		{320,720},
		{640,720},
		{960,720},
		{1280,720},
		{0,240},
		{0,480},
		{0,720},
		{1280,240},
		{1280,480},
		{1280,720}
	};


	size_t handle;
	float spawnTimer = 0.0f;
	float spawnInterval = 0.05f;
	int toSpawn = 5000;
	bool beSpawning = false;

	float startWait = 3.0f;
	float startTimer = 0.0f;

	void Update(float dT);
	void PopulateFramePool();
};

EnemySystem::EnemySystem(
	RenderSystem& _renderSystem,
	Player& _player
) : renderSystem(_renderSystem),
	player(_player)
{
	PopulateFramePool();

	handle = UpdateRelay::AddListener(
		[this](const float& dT) {
			Update(dT);
		}
	);

	
}
EnemySystem::~EnemySystem()
{
	/*
	TODO: somehow disconnect from the update relay.
	*/
	enemies.clear();
}

void EnemySystem::PopulateFramePool()
{
	// create the configs lists
	EnemyAnimationConfig skeeterA = {
		EnumEnemy::SKEETER,
		{
			"Resources/EnemySprites/SKEETER/SKEETER_A00.png",
			"Resources/EnemySprites/SKEETER/SKEETER_A01.png"
		},
		4.0f
	}; animConfigs.push_back(skeeterA);

	// populate the frames pool
	for (const auto& config : animConfigs) {
		enemyFramesPool[config.enemyName] = {};
		for (const auto& path : config.framePaths) {
			Texture2D tex = LoadTexture(path.c_str());
			if (tex.id == 0) {
				std::cerr << "Failed to load texture: " << path << "\n";
			}
			enemyFramesPool[config.enemyName].push_back(tex);
		}
	}
	
}

void EnemySystem::RenderEnemies()
{
	for (const auto& enemy : enemies) {
		renderSystem.RenderGameObject(enemy.get());
	}
	
}

void EnemySystem::SpawnEnemy(EnumEnemy enemyType, Vector2 spawnPos)
{
	auto newEnemy = std::make_unique<Enemy>(
		spawnPos,
		Vector2{ 50,50 },
		100.0f,
		renderSystem,
		player,
		enemyFramesPool[enemyType].size(),
		12.0f
		//EnumEnemy::SKEETER
	);
	newEnemy->RandomizeSkew();
	//newEnemy->SetEnemyType(EnumEnemy::SKEETER);
	enemies.push_back(std::move(newEnemy));
	
}

void EnemySystem::Update(float dT)
{

	if (!beSpawning) {
		startTimer += dT;
		if (startTimer >= startWait) {
			beSpawning = true;
			player.SetBeAttacking(true);
		}
	}


	//startTimer += dT;
	//if (startTimer >= startWait) {
	//	beSpawning = true;
	//}

	if (!beSpawning) {
		return;
	}

	for (auto& enemy : enemies) {

		if (enemy == nullptr) continue;
		
		size_t currentFrame = enemy->GetCurrentFrame();
		enemy->setPtrToTexture(GetPtrToTexture(EnumEnemy::SKEETER, currentFrame));
		enemy->OnFrameUpdate(dT);
		
	}

	
	spawnTimer += dT;



	if (spawnTimer < spawnInterval) return; // <- every 2 seconds
	spawnTimer = 0.0f;
	

	if (enemies.size() < toSpawn) {

		size_t spawnIndex = GetRandomValue(0, spawnPoints.size() - 1);

		SpawnEnemy(EnumEnemy::SKEETER, { spawnPoints[spawnIndex]});
	}
	//for (const auto& enemy : enemies) {
	//	enemy->RandomizeSkew();
	//}

}

void EnemySystem::KillEnemy(Enemy* ptrToEnemy)
{
	enemies.erase(
		std::remove_if(
			enemies.begin(),
			enemies.end(),
			[ptrToEnemy](const auto& ptr){
				return ptr.get() == ptrToEnemy;
			}
		),
		enemies.end()
	);
}
