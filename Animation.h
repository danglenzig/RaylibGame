/////////////////
// Animation.h //
/////////////////

#pragma once
#include <iostream>
#include <vector>
#include "raylib.h"

enum EnumAnimEvent
{
	STARTED,
	FINISHED,
	LOOPED,
	FRAME_UPDATED
};
struct AnimEvent
{
	std::string animName;
	EnumAnimEvent event;
};

struct AnimationConfig
{
	std::string name;
	std::vector<std::string> framePaths;
	float fps = 1.0f;
};
struct Animation
{
	std::string name;
	std::vector<Texture2D> frames;
	float fps = 1.0f;
	float interval = 1.0f;
	size_t currentFrame = 0;
	float timer = 0.0f;
	bool loop = true;
};