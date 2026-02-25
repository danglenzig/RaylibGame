//////////////////
// GameObject.h //
//////////////////

#pragma once
#include "UpdateRelay.h"


struct GameObjectDrawData
{
	Vector2 position;
	Vector2 size;
	float lookAngle;
	Texture2D* ptrToTexture;
};

class GameObject
{
protected:
	size_t handle;
	Vector2 position;
	Vector2 size;
	float lookAngle;
	GameObjectDrawData drawData;
public:
	GameObject() {
		drawData.position = { 0,0 };
		drawData.size = { 0,0 };
		drawData.lookAngle = 0.0f;
		drawData.ptrToTexture = nullptr;
		//handle = UpdateRelay::AddListener(
		//	[this](const float& dt) { 
		//		OnFrameUpdate(dt);
		//	}
		//);
	}
	virtual ~GameObject() {
		// TODO: somehow disconnect from the update relay.
	}
	virtual void OnFrameUpdate(const float& dT) {}
	virtual void FixDrawData() {}
	GameObjectDrawData GetDrawData()
	{
		return drawData;
	}
private:
	
};
