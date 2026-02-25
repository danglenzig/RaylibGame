////////////////
// Animator.h //
////////////////

#pragma once
#include "Animation.h"
#include <unordered_map>
#include <functional>
#include <algorithm>


class Animator
{
	std::vector<std::function<void(const AnimEvent&)>> listeners;
public:
	Animator();
	~Animator();

	void LoadAnimations(const std::vector<AnimationConfig>& configs);
	void OnUpdate(const float& dT);
	void Play(const std::string& animName, bool loop = true);
	Texture2D* GetPtrToCurrentTexture();
	bool IsPlaying(const std::string& animName) const;
	std::string GetCurrentAnimName() const;
	size_t GetCurrentFrame() const;
	void AddListener(std::function<void(const AnimEvent&)> cBack);
	void Dispatch(const AnimEvent& animEvent);
	
private:
	std::vector<Animation> animations;
	Animation* ptrToCurrentAnim = nullptr;
	std::unordered_map<std::string, size_t> animIndexMap;
	Animation* GetPtrToAnimation(const std::string animName);
	
};

//////////////////////////////
// Constructor & Destructor //
//////////////////////////////
Animator::Animator()
{
}

Animator::~Animator()
{
	for (auto& anim : animations) {
		for (auto& frame : anim.frames) {
			UnloadTexture(frame);
		}
	}
}

////////////
// Public //
////////////
void Animator::LoadAnimations(const std::vector<AnimationConfig>& configs)
{
	animations.clear();
	animIndexMap.clear();

	for (const auto& config : configs) {
		Animation anim;
		anim.name = config.name;
		anim.fps = std::clamp(config.fps, 1.0f, 60.0f);
		anim.interval = 1.0f / anim.fps;
		animIndexMap[config.name] = animations.size();

		for (const auto& path : config.framePaths) {
			Texture2D tex = LoadTexture(path.c_str());
			if (tex.id == 0) {
				std::cerr << "Failed to load texture: " << path << "\n";
			}
			anim.frames.push_back(tex);
		}
		animations.push_back(anim);
	}
}

void Animator::OnUpdate(const float& dT)
{
	if (ptrToCurrentAnim == nullptr || ptrToCurrentAnim->frames.empty()) return;

	ptrToCurrentAnim->timer += dT;
	if (ptrToCurrentAnim->timer < ptrToCurrentAnim->interval) return;
	ptrToCurrentAnim->timer = 0.0f;
	ptrToCurrentAnim->currentFrame++;

	if (ptrToCurrentAnim->currentFrame >= ptrToCurrentAnim->frames.size()) {
		if (ptrToCurrentAnim->loop) {
			ptrToCurrentAnim->currentFrame = 0;
			Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::LOOPED });
		}
		else
		{
			Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::FINISHED });
		}
		return;
	}
	Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::FRAME_UPDATED });
}

void Animator::Play(const std::string& animName, bool loop)
{

	auto ptrToAnim = GetPtrToAnimation(animName);
	if (ptrToAnim == nullptr) {
		std::cerr << "Animation " << animName << " not found!\n";
		return;
	}
	ptrToCurrentAnim = ptrToAnim;
	ptrToCurrentAnim->loop = loop;
	ptrToCurrentAnim->currentFrame = 0;
	ptrToCurrentAnim->timer = 0.0f;
	Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::STARTED });
}

Texture2D* Animator::GetPtrToCurrentTexture()
{
	if (ptrToCurrentAnim != nullptr && !ptrToCurrentAnim->frames.empty()) {
		size_t idx = ptrToCurrentAnim->currentFrame;
		return &ptrToCurrentAnim->frames[idx];
	}
	return nullptr;
}

bool Animator::IsPlaying(const std::string& animName) const
{
	if (ptrToCurrentAnim == nullptr) return false;
	return ptrToCurrentAnim->name == animName;
}

std::string Animator::GetCurrentAnimName() const
{
	return (ptrToCurrentAnim != nullptr) ? ptrToCurrentAnim->name : "NONE";
}

size_t Animator::GetCurrentFrame() const
{
	// sentinel value incicates no current anim
	if (ptrToCurrentAnim == nullptr) return -1; 
	return ptrToCurrentAnim->currentFrame;
}

void Animator::AddListener(std::function<void(const AnimEvent&)> cBack)
{
	listeners.push_back(cBack);
}

void Animator::Dispatch(const AnimEvent& animEvent)
{
	for (auto& cBack : listeners) {
		if (cBack) cBack(animEvent);
	}
}

/////////////
// Private //
/////////////

Animation* Animator::GetPtrToAnimation(const std::string animName)
{
	auto iterator = animIndexMap.find(animName);
	if (iterator != animIndexMap.end()) {
		return &animations[iterator->second];
	}
}