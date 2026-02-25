//////////////////////////
// TestUpdateListener.h //
//////////////////////////

#pragma once
#include <iostream>
#include "UpdateRelay.h"
class TestUpdateListener
{
	/*
	This is a throwaway class, just to test a concept.
	In real life, maybe it's a frame animator, or anything
	that has "do this N times per second" behavior
	*/

public:
	TestUpdateListener(const float& _fps);
	~TestUpdateListener();
private:
	float fps;
	float interval;
	float _ta; // time accumulator
	size_t listenerHandle;
	void OnFrameUpdate(const float& dT);
};

// constructor
TestUpdateListener::TestUpdateListener(const float& _fps)
{
	fps = abs(_fps);
	interval = 1.0f / fps;
	_ta = 0.0f;

	// Bind and register
	listenerHandle = UpdateRelay::AddListener(
		std::bind(&TestUpdateListener::OnFrameUpdate, this, std::placeholders::_1)
	);
}

//destructor
TestUpdateListener::~TestUpdateListener()
{
	//UpdateRelay::RemoveListener(listenerHandle);
}


// Update behavior
void TestUpdateListener::OnFrameUpdate(const float& dT)
{
	_ta += dT;
	if (_ta < interval) return;
	_ta = 0.0f;

	std::cout << "Tick!\n";

}