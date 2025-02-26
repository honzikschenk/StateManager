/**
 * @brief A library for managing various states (a state machine).
 * @author Honzik Schenk
 *
 * StateManager is a library that provides easy and efficient management of
 * different states for various applications (ex: robotics). It allows you
 * to add, remove, and configurate various states and manage how they transition
 * between each other. A more common name for this is a state machine.
 */

#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <string>
#include <vector>

using namespace std;

class StateManager
{
private:
    struct State
    {
        string stateName;

        bool (*stateFunction)();
        bool (*transitionToState)(string activeState);

        bool operator==(const State &s)
        {
            return stateName == s.stateName;
        }

        bool operator!=(const State &s)
        {
            return stateName != s.stateName;
        }
    };

    State *getStateByName(const string stateName);

    State dummyState;

    static bool dummyStateFunction();

    static bool dummyTransitionToState(string activeState);

public:
    vector<State> states;

    State &activeState;

    StateManager();

    /**
     * @brief Run the state manager running the active state without transitioning to the proper next state.
     * @return True if the state manager executed the active state succesfully.
     *
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     */
    bool run();

    /**
     * @brief Run the state manager including running the active state and (if flagged true) transitioning to the proper next state.
     * @param transitionToo If true, the state manager will also transition to the next state.
     * @return True if the state manager executed the active state succesfully.
     *
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     */
    bool run(bool transitionToo);

    /**
     * @brief Transition to the state that wants to become active.
     * @return True if a state was transitioned to successfully, false if no state needed to be transitioned to.
     *
     * @warning If there are no states in the state manager, this function will always return false.
     * @warning If two states want to become active at the same time, the state manager will choose the first one in the list.
     *
     * @note This function is called automatically when using the run() function with the transitionToo flag set to true.
     */
    bool transition();

    /**
     * @brief Transition to a specific state.
     * @param stateName The name of the state to transition to.
     * @return True if the state was found and transitioned to successfully, false if the state was not found.
     */
    bool transition(string stateName);

    /**
     * @brief Add a state to the state manager.
     * @param stateName The name of the new state.
     * @return True if the state was added successfully, false if the state already exists.
     */
    bool addState(string stateName);

    /**
     * @brief Remove a state from the state manager.
     * @param stateName The name of the state to remove.
     * @return True if the state was removed successfully, false if not found.
     *
     * @warning Removing the active state will cause the state manager to return false when run until you switch to another state.
     */
    bool removeState(string stateName);

    /**
     * @brief Set the function that will be called when the state is active.
     * @param stateName The name of the state to set the function for.
     * @param stateFunction The function to call when the state is active.
     * @return True if the function was set successfully, false if the state was not found.
     */
    bool setStateFunction(string stateName, bool (*stateFunction)());

    /**
     * @brief Set the function that will be called when the state is transitioning to.
     * @param stateName The name of the state to set the function for.
     * @param transitionToState The function to call when the state is transitioning to.
     * @return True if the function was set successfully, false if the state was not found.
     */
    bool setTransitionToState(string stateName, bool (*transitionToState)(string activeState));

    /**
     * @brief Get the name of the active state.
     * @return The name of the active state.
     */
    string getActiveStateName();
};

#endif // STATEMANAGER_HPP
