/**
 * @brief A library for managing various states (a state machine).
 * Author: Honzik Schenk
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
 
 StateManager::State *StateManager::getStateByName(const string stateName)
 {
     for (auto &s : states)
     {
         if (s.stateName == stateName)
         {
             return &s;
         }
     }
 
     return nullptr;
 }
 
 bool StateManager::dummyStateFunction()
 {
     return false;
 }
 
 bool StateManager::dummyTransitionToState(string activeState)
 {
     return false;
 }
 
 StateManager::StateManager() : activeState(dummyState)
 {
     dummyState.stateName = "dummyState";
     dummyState.stateFunction = dummyStateFunction;
     dummyState.transitionToState = dummyTransitionToState;
 
     activeState = dummyState;
 
     states = vector<State>();
 }
 
 bool StateManager::run()
 {
     return activeState.stateFunction();
 }
 
 bool StateManager::run(bool transitionToo)
 {
     bool stateRan = activeState.stateFunction();
 
     if (!transitionToo)
     {
         return stateRan;
     }
 
     for (auto &s : states)
     {
         if (activeState != s && s.transitionToState(activeState.stateName))
         {
             activeState = s;
             return stateRan;
         }
     }
 
     return stateRan;
 }
 
 bool StateManager::transition()
 {
     for (auto &s : states)
     {
         if (activeState != s && s.transitionToState(activeState.stateName))
         {
             activeState = s;
             return true;
         }
     }
 
     return false;
 }
 
 bool StateManager::transition(string stateName)
 {
     State *state = getStateByName(stateName);
 
     if (state == nullptr)
     {
         return false;
     }
 
     activeState = *state;
 
     return true;
 }
 
 bool StateManager::addState(string stateName)
 {
     State *checkIfExists = getStateByName(stateName);
 
     if (checkIfExists != nullptr)
     {
         return false;
     }
 
     State state;
     state.stateName = stateName;
 
     states.push_back(state);
 
     return true;
 }
 
 bool StateManager::removeState(string stateName)
 {
     State *state = getStateByName(stateName);
 
     if (state == nullptr)
     {
         return false;
     }
 
     if (activeState.stateName == stateName)
     {
         activeState = dummyState;
     }
 
     states.erase(find(states.begin(), states.end(), *state));
 
     if (states.size() == 0)
     {
         activeState = dummyState;
 
         states.push_back(dummyState);
     }
 
     return true;
 }
 
 bool StateManager::setStateFunction(string stateName, bool (*stateFunction)())
 {
     State *state = getStateByName(stateName);
 
     if (state == nullptr)
     {
         return false;
     }
 
     state->stateFunction = stateFunction;
 
     return true;
 }
 
 bool StateManager::setTransitionToState(string stateName, bool (*transitionToState)(string activeState))
 {
     State *state = getStateByName(stateName);
 
     if (state == nullptr)
     {
         return false;
     }
 
     state->transitionToState = transitionToState;
 
     return true;
 }
 
 string StateManager::getActiveStateName()
 {
     return activeState.stateName;
 }
 