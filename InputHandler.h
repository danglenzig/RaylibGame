#pragma once
#include "raylib.h"
#include "raymath.h"

class InputHandler
{
	//static InputEvents events;
public:
	static Vector2 GetMoveInput();
};
Vector2 InputHandler::GetMoveInput()
{
	float moveX = 0.0f;
	float moveY = 0.0f;
	if (IsKeyDown(KEY_W)) moveY -= 1.0f;
	if (IsKeyDown(KEY_S)) moveY += 1.0f;
	if (IsKeyDown(KEY_A)) moveX -= 1.0f;
	if (IsKeyDown(KEY_D)) moveX += 1.0f;
	Vector2 moveInput = Vector2Normalize({ moveX, moveY });
	return moveInput;
}