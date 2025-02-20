// NOTE: This is an example of how to use the StateManager library.
// To run with gcc, use the following command: g++ -std=c++11 -o StateManagerTest Test.cpp StateManager.cpp && ./StateManagerTest
#include <iostream>
#include <string>

#include "StateManager.hpp"

int q = 0;

bool cond(int i)
{
    if (i == 1)
    {
        return true;
    }

    return false;
}

bool transition(string activeState)
{
    return true;
}

int main()
{
    StateManager stateManager;

    cout << stateManager.run(true) << endl;

    stateManager.addState("state1");

    stateManager.setStateFunction("state1", []() { return cond(q); });
    stateManager.setTransitionToState("state1", transition);

    cout << stateManager.run(true) << endl;

    q = 1;

    cout << stateManager.run(true) << endl;
}
