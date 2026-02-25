#pragma once
#include<iostream>
#include "StateMachine.h"


void SmokeTestFSM(); // see comment in function implementation
void SmokeTestFSM()
{

	/*
	This kind of stuff will be moved to a separate console
	project in the same VS solution, linked to this one,
	which will be used for more rigorous testing.
	I haven't set that up yet, so I'm putting it here for now
	*/

	std::cout << "TESTING\n";


	const std::string IDLE = "IDLE";
	const std::string WALKING = "WALKING";
	const std::string RUNNING = "RUNNING";
	const std::string ATTACKING = "ATTACKING";

	FSM testMachine(IDLE, 3);

	testMachine.AddState(WALKING);
	testMachine.AddState(RUNNING);
	testMachine.AddState(ATTACKING);

	testMachine.AddTransition(IDLE, WALKING);
	testMachine.AddTransition(IDLE, ATTACKING);

	testMachine.AddTransition(WALKING, RUNNING);
	testMachine.AddTransition(WALKING, ATTACKING);
	testMachine.AddTransition(WALKING, IDLE);

	testMachine.AddTransition(RUNNING, ATTACKING);
	testMachine.AddTransition(RUNNING, WALKING);

	testMachine.AddTransition(ATTACKING, IDLE);


	testMachine.AddListener(
		[](const StateEvent& e) {

			switch (e.eventType) {
			case EnumStateEventType::STATE_ENTERED:
				std::cout << "Entered state: " << e.stateName << ".\n";
				break;
			case EnumStateEventType::STATE_EXITED:
				std::cout << "Exited state: " << e.stateName << ".\n";
				break;
			}
		}
	);
	testMachine.TryTransition(WALKING);
}

/*
* Drawing a rectangle
*
	Vector2 newPos = playerPosition + (moveInput * playerSpeed * deltaTime);
	if (newPos.x < 0.0f) newPos.x = 0.0f;
	if (newPos.x > rightScreenExtent) newPos.x = rightScreenExtent;
	if (newPos.y < 0.0f) newPos.y = 0.0f;
	if (newPos.y > bottomScreenExtent) newPos.y = bottomScreenExtent;
	playerPosition = newPos;


	DrawRectangleV(playerPosition, { playerSize, playerSize }, BLUE);
	DrawText("Use WASD to move", 10, 10, 20, DARKGRAY);

*/