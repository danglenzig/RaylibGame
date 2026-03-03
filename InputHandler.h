#pragma once
#include "raylib.h"
#include "raymath.h"
#include <functional>

class InputHandler
{
	//static InputEvents events;
	inline static std::vector<std::function<void()>> listeners;
public:
	static Vector2 GetMoveInput();
	static void GetAttackInput();
	static size_t AddAttackListener(std::function<void()> cBack);
	static void RemoveListener(size_t handle) {
		// fuck
		// it
	}

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
void InputHandler::GetAttackInput()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (auto& cBack : listeners) {
			if (cBack) { cBack(); }
		}
	}
}
size_t InputHandler::AddAttackListener(std::function<void()> cBack)
{
	listeners.push_back(cBack);
	return listeners.size() - 1;
}

