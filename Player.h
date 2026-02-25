//////////////
// Player.h //
//////////////

#pragma once

#include "GameObject.h"
#include "StateMachine.h"
#include "InputHandler.h"
#include "raymath.h"
#include "Animator.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum EnumPlayerState {
	IDLE_STATE,
	WALKING_STATE,
	ATTACKING_STATE
};

struct PlayerData
{
	std::string currentStateName;
	std::string previousStateName;
	int health;
	int level;
	float slapFrequency;
};


class Player : public GameObject
{
public:
	PlayerData GetPlayerData();
	Vector2 GetPosition() const;

	Player(Vector2 startPos, int startHealth, int startLevel, float startSlapFrequency) { // initialize with start position
		position = startPos;
		health = startHealth;
		level = startLevel;
		slapFrequency = startSlapFrequency;

		InitializeAnimator();
		ConfigureFSM();
		animator.Play("IDLE", true);
		FixDrawData();

	}
	void Init();

	~Player() = default;
	// any cleanup required?


	std::string GetCurrentState() const {
		return playerFSM.currentStateName;
	}

	void OnFrameUpdate(const float& dT) override;
	void FixDrawData() override;

private:
	const std::string IDLE = "IDLE";
	const std::string WALKING = "WALKING";
	const std::string ATTACKING = "ATTACKING";
	const std::unordered_map<std::string, EnumPlayerState> statesMap = {
		{"IDLE", IDLE_STATE},
		{"WALKING", WALKING_STATE},
		{"ATTACKING", ATTACKING_STATE}
	};

	float speed = 300.0f;
	FSM playerFSM;
	Animator animator;
	int health;
	int level;
	float slapFrequency;
	float slapTimer = 0.0f;
	bool beSlapping = false;

	void OnStateEvent(const StateEvent& e);
	void OnStateEntered(const std::string& stateName);
	void OnStateExited(const std::string& stateName);
	EnumPlayerState GetStateNameAsEnum(const std::string& stateName);
	void IdleUpdateBehavior(const float& dT, const Vector2& moveInput);
	void WalkingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void AttackingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void ConfigureFSM();
	void InitializeAnimator();
	void OnAnimationEvent(const AnimEvent&);
};

///////////////
// Overrides //
///////////////
void Player::OnFrameUpdate(const float& dT)
{
	animator.OnUpdate(dT);
	Vector2 moveInput = InputHandler::GetMoveInput();
	// ask the Animator for a pointer to the appropriate texture
	//   for this frame, and update drawData.ptrToTexture
	//   (remember drawData lives in the GameObject parent
	//   class now)

	switch (GetStateNameAsEnum(playerFSM.currentStateName))
	{
	case EnumPlayerState::IDLE_STATE:
		IdleUpdateBehavior(dT, moveInput);
		break;
	case EnumPlayerState::WALKING_STATE:
		WalkingUpdateBehavior(dT, moveInput);
		break;
	case EnumPlayerState::ATTACKING_STATE:
		AttackingUpdateBehavior(dT, moveInput);
		break;
	default:
		break;
	}


	if (beSlapping) {
		slapTimer += dT;
		if (slapTimer >= slapFrequency) {
			slapTimer = 0.0f;
			playerFSM.TryTransition(ATTACKING);
		}
	}
	

}


void Player::FixDrawData()
{
	drawData.position = position;
	drawData.size = { 270,270 };
	
	if (playerFSM.currentStateName == WALKING) {
		drawData.lookAngle = Vector2Angle({ 1,0 }, InputHandler::GetMoveInput());
	}
	drawData.ptrToTexture = animator.GetPtrToCurrentTexture();
}

//////////////////////
// Public functions //
//////////////////////

PlayerData Player::GetPlayerData()
{
	PlayerData playerData;
	playerData.currentStateName = playerFSM.currentStateName;
	playerData.previousStateName = playerFSM.StateHistory.back();
	playerData.health = health;
	playerData.level = level;
	playerData.slapFrequency = slapFrequency;
	return playerData;
}
Vector2 Player::GetPosition() const {
	return position;
}

void Player::Init()
{
	handle = UpdateRelay::AddListener(
		[this](const float& dt) {
			OnFrameUpdate(dt);
		}
	);
}

///////////////////////
// Private functions //
///////////////////////
void Player::IdleUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2Length(moveInput) > FLT_EPSILON) {
		playerFSM.TryTransition(WALKING);
		return;
	}
	FixDrawData();
}

void Player::WalkingUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2Length(moveInput) <= FLT_EPSILON) {
		playerFSM.TryTransition(IDLE);
		return;
	}
	Vector2 newPos = position + (moveInput * speed * dT);
	position = newPos;
	FixDrawData();
}

void Player::AttackingUpdateBehavior(const float& dT, const Vector2& moveInput)
{

	FixDrawData();
}

void Player::ConfigureFSM()
{
	playerFSM.Initialize(IDLE, 4);

	playerFSM.AddState(WALKING);
	playerFSM.AddState(ATTACKING);

	playerFSM.AddTransition(IDLE, WALKING);
	playerFSM.AddTransition(IDLE, ATTACKING);

	playerFSM.AddTransition(WALKING, IDLE);
	playerFSM.AddTransition(WALKING, ATTACKING);

	playerFSM.AddTransition(ATTACKING, IDLE);

	playerFSM.AddListener(
		[this](const StateEvent& event) {
			OnStateEvent(event);
		}
	);
}

void Player::OnStateEvent(const StateEvent& e)
{
	switch (e.eventType) {
	case EnumStateEventType::STATE_ENTERED:
		OnStateEntered(e.stateName);
		break;
	case EnumStateEventType::STATE_EXITED:
		OnStateExited(e.stateName);
		break;
	default: break;
	}
}

void Player::OnStateEntered(const std::string& stateName)
{
	switch (GetStateNameAsEnum(stateName)) {
	case EnumPlayerState::IDLE_STATE:
		// tell the animator to be playing the IDLE anim
		animator.Play(IDLE, true);
		break;
	case EnumPlayerState::WALKING_STATE:
		// tell the animator to be playing the WALKING anim
		animator.Play(WALKING, true);
		break;
	case EnumPlayerState::ATTACKING_STATE:
		// tell the animator to be playing the ATTACKING anim
		animator.Play(ATTACKING, false);
		break;
	}

}

void Player::OnStateExited(const std::string& stateName)
{
	switch (GetStateNameAsEnum(stateName)) {
		// ...
	}
}

EnumPlayerState Player::GetStateNameAsEnum(const std::string& stateName)
{
	auto iterator = statesMap.find(stateName);
	if (iterator != statesMap.end()) {
		return iterator->second;
	}
	std::cerr << "Error: '" << stateName << "' is not a valid Player State.\n";
}



void Player::InitializeAnimator()
{
	std::vector<AnimationConfig> configs = {
		{IDLE, {"Resources/PlayerSprites/Posey/IDLE/POSEY_IDLE_01.png"}, 1.0f},
		{WALKING,
			{
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_00.png",
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_01.png",
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_02.png",
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_03.png",
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_04.png",
				"Resources/PlayerSprites/Posey/WALKING/POSEY_RUN_05.png"
			},
			12.0f
		},
		{ATTACKING,
			{
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_01.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_02.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_03.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_04.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_05.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_06.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_07.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_08.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_09.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_10.png",
				"Resources/PlayerSprites/Posey/ATTACKING/POSEY_SLAP_11.png",
			},
			24.0f
		}
	
	};
	animator.LoadAnimations(configs);

	animator.AddListener(
		[this](const AnimEvent& event) {
			OnAnimationEvent(event);
		}
	);

}

void Player::OnAnimationEvent(const AnimEvent& event)
{
	if (event.animName == ATTACKING) {
		switch (event.event) {
		case EnumAnimEvent::FINISHED:
			playerFSM.TryTransition(IDLE);
			break;
		}
	}
}