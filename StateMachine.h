////////////////////
// StateMachine.h //
////////////////////

#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <deque>
#include <functional>

enum EnumStateEventType
{
	STATE_ENTERED,
	STATE_EXITED
};
struct StateEvent
{
	EnumStateEventType eventType;
	std::string stateName;
};


class FSM_State
{
public:
	std::string StateName;
	std::vector<std::string> Transitions;
	FSM_State(const std::string& stateName) : StateName(stateName) {}
	void AddTransition(const std::string& toStateName);
private:
	bool TransitionWithNameExists(const std::string& transName)
	{
		return std::find(Transitions.begin(), Transitions.end(), transName) != Transitions.end();
	}
};
void FSM_State::AddTransition(const std::string& toStateName)
{
	if (TransitionWithNameExists(toStateName)) {
		std::string errMsg = "State " + StateName + " already has transition to " + toStateName + "\n";
		std::cerr << errMsg;
		return;
	}
	Transitions.emplace_back(toStateName);
}

/////////////////////////////////////////////////////////

class FSM_Events
{
	std::vector<std::function<void(const StateEvent&)>> listeners;
public:
	void AddListener(std::function<void(const StateEvent&)> cBack) {
		listeners.push_back(cBack);
	}
	void Dispatch(const StateEvent& event) {
		for (auto& cBack : listeners) {
			cBack(event);
		}
	}
};

/////////////////////////////////////////////////////////


class FSM
{
	FSM_Events events;
public:
	std::vector<FSM_State> States;
	std::deque<std::string> StateHistory;
	std::string currentStateName;
	size_t historySize = 10;

	// constructors
	FSM(const std::string& initialStateName, size_t _historySize = 10) : historySize(_historySize), currentStateName(initialStateName)
	{
		States.emplace_back(initialStateName);
		StateHistory.emplace_back("NONE");
		currentStateName = initialStateName;
		NotifyStateEntered(currentStateName);
	}
	FSM() {}

	// public methods
	void AddState(const std::string& stateName);
	void AddTransition(const std::string& fromState, const std::string& toState);
	bool TryTransition(const std::string& toStateName);
	void Initialize(const std::string& initialStateName, size_t _historySize = 10);
	void AddListener(std::function <void(const StateEvent&)> cBack) {
		events.AddListener(cBack);
	}

private :
	FSM_State* GetPointerToStatewithName(const std::string& name);
	bool StateWithNameExists(const std::string& name);
	void UpdateStateHistory(const std::string& name);
	void NotifyStateExited(const std::string& name) {
		events.Dispatch({ EnumStateEventType::STATE_EXITED, name });
	}
	void NotifyStateEntered(const std::string& name) {
		events.Dispatch({ EnumStateEventType::STATE_ENTERED, name });
	}

};

void FSM::Initialize(const std::string& initialStateName, size_t _historySize)
{
	States.emplace_back(initialStateName);
	StateHistory.emplace_back("NONE");
	currentStateName = initialStateName;
	NotifyStateEntered(currentStateName);
}

FSM_State* FSM::GetPointerToStatewithName(const std::string& name)
{
	auto iterator = std::find_if(
		States.begin(), States.end(), [&](const FSM_State& s) {
			return s.StateName == name;
		}
	);
	return (iterator == States.end()) ? nullptr : &(*iterator);
}

bool FSM::StateWithNameExists(const std::string& name)
{

	bool exists = std::find_if(States.begin(), States.end(), [&](const FSM_State& s) {
		return s.StateName == name;
		}) != States.end();
	return exists;
}

void FSM::UpdateStateHistory(const std::string& name)
{
	if (StateHistory.size() >= historySize) {
		StateHistory.pop_front();
	}
	StateHistory.emplace_back(name);
}

void FSM::AddState(const std::string& stateName)
{
	if (StateWithNameExists(stateName)) {
		std::string err = "State " + stateName + " already exists\n";
		std::cerr << err;
		return;
	}
	States.emplace_back(FSM_State(stateName));
}

void FSM::AddTransition(const std::string& fromStateName, const std::string& toStateName)
{
	if (!StateWithNameExists(fromStateName) || !StateWithNameExists(toStateName)) {
		std::string err = "Invalid transition to and/or from state does not exist\n";
		std::cerr << err;
		return;
	}
	FSM_State* ptrToFromState = GetPointerToStatewithName(fromStateName);
	ptrToFromState->AddTransition(toStateName);
}

bool FSM::TryTransition(const std::string& toStateName)
{
	FSM_State* ptrToCurrentState = GetPointerToStatewithName(currentStateName);
	if (ptrToCurrentState == nullptr) {
		std::cerr << "No current state\n";
		return false;
	}
	const FSM_State& currentState = *ptrToCurrentState;
	for (auto& trans : currentState.Transitions) {
		if (trans == toStateName) {
			// update the history
			UpdateStateHistory(currentStateName);
			// pop off STATE_EXITED event
			NotifyStateExited(currentStateName);

			// switcheroo
			currentStateName = trans;
			// pop off STATE_ENTERED event
			NotifyStateEntered(currentStateName);
			return true;
		}
	}
	std::string err = currentStateName + " has no transition to " + toStateName + "\n";
}