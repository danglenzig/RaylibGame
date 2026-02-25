#include "raylib.h"
#include "raymath.h"
#include "InputHandler.h"
#include "StateMachine.h"
#include "UpdateRelay.h"
#include "TestUpdateListener.h"
#include "Player.h"
#include "GameObject.h"
#include "RenderSystem.h"
#include "EnemySystem.h"
#include <iostream>
//#include "rlgl.h"

int main()
{
	///////////////////////
	// 1. INITIALIZATION //
	///////////////////////
	const int screenWidth = 1280;
	const int screenHeight = 720;
	InitWindow(screenWidth, screenHeight, "My First Raylib Game");
	SetTargetFPS(60);

	//ChangeDirectory(GetApplicationDirectory());
	// ^^for release

	// Load resources here (textures, sounds, etc. here)

	// Player
	Player player({640, 360}, 1000, 1, 2.0f);
	player.Init();
	RenderSystem renderSystem;
	EnemySystem enemySystem(renderSystem, player);

	//////////////////
	// 2. GAME LOOP //
	//////////////////
	while (!WindowShouldClose())
	{
		/////////////////////////////////////////////
		// 2a. UPDATE - Game logic, input, physics //
		/////////////////////////////////////////////
		float deltaTime = GetFrameTime();
		Vector2 moveInput = InputHandler::GetMoveInput();

		UpdateRelay::Dispatch(deltaTime);
		// Everything that inherits from GameObject will
		// hear ^^this

		// for testing only, remove later...
		//if (player.GetPlayerData().currentStateName == "WALKING") {
		//	std::cout << player.GetDrawData().position.x << ", " << player.GetDrawData().position.y << "\n";
		//}
		

		//////////////////////////////////
		// 2b. DRAW - Render everything //
		//////////////////////////////////
		BeginDrawing();
		ClearBackground({101,115,58,1});

		// Draw calls here
		renderSystem.RenderGameObject(&player);
		enemySystem.RenderEnemies();
		


		size_t count = enemySystem.GetEnemyCount();
		DrawText(("Enemy Count: " + std::to_string(count)).c_str(), 10, 10, 20, BLACK);

		EndDrawing();
	}

	// 3. CLEANUP
	CloseWindow();
	
	return 0;
}



