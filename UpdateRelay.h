///////////////////
// UpdateRelay.h //
///////////////////




#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <deque>
#include <functional>

class UpdateRelay
{
	// Frame cycle...float payload is deltaTime
	inline static std::vector<std::function<void(const float&)>> listeners;
public:
	static size_t AddListener(std::function<void(const float&)> cBack)
	{
		listeners.push_back(cBack);
		return listeners.size() - 1;
	}


	static void RemoveListener(size_t handle)
	{
		// FUCK
		// It.
	}
	

	void static Dispatch(const float& event) {
		
		
		// main loop calls this every frame and
		// payloads it with delta time
		for (auto& cBack : listeners) {
			if (cBack) {
				cBack(event);
			}
		}
		
	}

};
