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

    bool run();

    bool run(bool transitionToo);

    bool transition();

    bool transition(string stateName);

    bool addState(string stateName);

    bool removeState(string stateName);

    bool setStateFunction(string stateName, bool (*stateFunction)());

    bool setTransitionToState(string stateName, bool (*transitionToState)(string activeState));

    string getActiveStateName();
};

#endif // STATEMANAGER_HPP
