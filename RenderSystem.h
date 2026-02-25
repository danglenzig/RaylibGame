#pragma once
#include "GameObject.h"
#include "raylib.h"
#include <iostream>

class RenderSystem
{
public:
	RenderSystem() = default;
	~RenderSystem() = default;
	void RenderGameObject(GameObject* ptrToGameObject);
	void Foo()
	{
		std::cout << "BAR" << std::endl;
	}
private:
	// as needed
};

void RenderSystem::RenderGameObject(GameObject* ptrToGameObject)
{
	if (ptrToGameObject == nullptr) return;

	GameObjectDrawData drawData = ptrToGameObject->GetDrawData();

	if (drawData.ptrToTexture == nullptr) {
		// red rectangle placeholder
		/*
		DrawRectanglePro(
			Rectangle{ drawData.position.x - 16, drawData.position.y - 16, 32, 32 },
			{ 16,16 },
			0.0f,
			RED
		);
		*/
		return;
	}
	else {
		DrawTexturePro(
			*drawData.ptrToTexture,
			{ 0,0, (float)drawData.ptrToTexture->width, (float)drawData.ptrToTexture->height },
			{ drawData.position.x, drawData.position.y, drawData.size.x, drawData.size.y },
			{ drawData.size.x / 2, drawData.size.y / 2 },
			drawData.lookAngle * RAD2DEG,
			WHITE
		);
	}
}