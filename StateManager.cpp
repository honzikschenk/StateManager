/**
 * @brief A library for managing various states (a state machine).
 * @author Honzik Schenk
 *
 * StateManager is a library that provides easy and efficient management of
 * different states for various applications (ex: robotics). It allows you
 * to add, remove, and configurate various states and manage how they transition
 * between each other. A more common name for this is a state machine.
 */

#include <string>
#include <vector>
#include <iostream>

#include "StateManager.hpp"

using namespace std;

class StateManager {
private:
    struct State {
        bool (*stateFunction)();
        bool (*transitionToState)(string activeState);
        string stateName;

        bool operator==(const State &s) {
            return stateName == s.stateName;
        }

        bool operator!=(const State &s) {
            return stateName != s.stateName;
        }
    };

    State* getStateByName(const string stateName) {
        for (auto &s : states) {
            if (s.stateName == stateName) {
                return &s;
            }
        }
        
        return nullptr;
    }

    bool noStates;
    State dummyState;

    static bool dummyStateFunction() {
        return false;
    }

    static bool dummyTransitionToState(string activeState) {
        return false;
    }

public:
    vector<State> states;

    State &activeState;

    StateManager() : activeState(dummyState) {
        dummyState.stateName = "dummyState";
        dummyState.stateFunction = dummyStateFunction;
        dummyState.transitionToState = dummyTransitionToState;

        noStates = true;

        activeState = dummyState;

        states = vector<State>();
    }

    /**
     * @brief Run the state manager running the active state without transitioning to the proper next state.
     * @return True if the state manager executed the active state succesfully.
     * 
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     */
    bool run() {
        return activeState.stateFunction();
    }

    /**
     * @brief Run the state manager including running the active state and (if flagged true) transitioning to the proper next state.
     * @param transitionToo If true, the state manager will also transition to the next state.
     * @return True if the state manager executed the active state succesfully.
     * 
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     */
    bool run(bool transitionToo) {
        bool stateRan = activeState.stateFunction();

        if(!transitionToo) {
            return stateRan;
        }

        for(auto &s : states) {
            if(s.transitionToState(activeState.stateName) && activeState.stateName != s.stateName) {
                activeState = s;
                return stateRan;
            }
        }

        return stateRan;
    }

    /**
     * @brief Transition to the state that wants to become active.
     * @return True if a state was transitioned to successfully, false if no state needed to be transitioned to.
     * 
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     * 
     * @note This function is called automatically when using the run() function with the transitionToo flag set to true.
     */
    bool transition() {
        for(auto &s : states) {
            if(s.transitionToState(activeState.stateName) && activeState.stateName != s.stateName) {
                activeState = s;
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Transition to a specific state.
     * @param stateName The name of the state to transition to.
     * @return True if the state was found and transitioned to successfully, false if the state was not found.
     */
    bool transition(string stateName) {
        State *state = getStateByName(stateName);

        if (state == nullptr) {
            return false;
        }

        activeState = *state;

        return true;
    }

    /**
     * @brief Add a state to the state manager.
     * @param stateName The name of the new state.
     * @return True if the state was added successfully, false if the state already exists.
     */
    bool addState(string stateName) {
        State *checkIfExists = getStateByName(stateName);

        if (checkIfExists != nullptr) {
            return false;
        }

        State state;
        state.stateName = stateName;
        
        states.push_back(state);

        return true;
    }

    /**
     * @brief Remove a state from the state manager.
     * @param stateName The name of the state to remove.
     * @return True if the state was removed successfully, false if not found.
     * 
     * @warning Removing the active state will cause the state manager to return false when run until you switch to another state.
     */
    bool removeState(string stateName) {
        State *state = getStateByName(stateName);

        if (state == nullptr) {
            return false;
        }

        if (activeState.stateName == stateName) {
            activeState = dummyState;
        }

        states.erase(find(states.begin(), states.end(), *state));

        if (states.size() == 0) {
            noStates = true;

            activeState = dummyState;

            states.push_back(dummyState);
        }

        return true;
    }

    /**
     * @brief Set the function that will be called when the state is active.
     * @param stateName The name of the state to set the function for.
     * @param stateFunction The function to call when the state is active.
     * @return True if the function was set successfully, false if the state was not found.
     */
    bool setStateFunction(string stateName, bool (*stateFunction)()) {
        State *state = getStateByName(stateName);

        if (state == nullptr) {
            return false;
        }

        state->stateFunction = stateFunction;

        return true;
    }

    /**
     * @brief Set the function that will be called when the state is transitioning to.
     * @param stateName The name of the state to set the function for.
     * @param transitionToState The function to call when the state is transitioning to.
     * @return True if the function was set successfully, false if the state was not found.
     */
    bool setTransitionToState(string stateName, bool (*transitionToState)(string activeState)) {
        State *state = getStateByName(stateName);

        if (state == nullptr) {
            return false;
        }

        state->transitionToState = transitionToState;

        return true;
    }
};





// NOTE: This is an example of how to use the StateManager library.
// To run with gcc, use the following command: g++ -std=c++11 -o StateManager StateManager.cpp && ./StateManager
int i = 0;

bool cond() {
    if(i == 1) {
        return true;
    }

    return false;
}

bool transition(string activeState) {
    return true;
}

int main() {
    StateManager stateManager;

    cout << stateManager.run(true) << endl;

    stateManager.addState("state1");

    stateManager.setStateFunction("state1", cond);
    stateManager.setTransitionToState("state1", transition);

    cout << stateManager.run(true) << endl;

    i = 1;

    cout << stateManager.run(true) << endl;
}
