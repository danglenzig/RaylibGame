/////////////////////////////
// GameObjectBoilerplate.h //
/////////////////////////////

#pragma once

#include "GameObject.h"

class ExampleGameObject : public GameObject
{
public:
	void OnFrameUpdate(const float& dT) override;
};

void ExampleGameObject::OnFrameUpdate(const float& dT)
{
	// per-frame behavior here
}