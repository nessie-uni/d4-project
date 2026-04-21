#include "splashkit.h"

struct die_history
{
    int previous_rolls[5];
    int length;
};

class Die
{
protected:
    int _maximum;
    die_history _history;

public:
    Die() {}
    Die(int size) {}
    int roll() {}
    die_history get_history() {}
    int get_past_value(int history_counter) {}
};

class CustomDie : public Die
{
private:
    int _minimum = 1;
    int _step = 1;
    int _options[6];
    int _mode = 0;

public:
    CustomDie() {}
    CustomDie(int maximum) {}
    CustomDie(int maximum, int minimun) {}
    CustomDie(int maximun, int minimum, int step) {}
    CustomDie(int options[6]){}
};

class Program
{
    int main()
    {
        // Set up:
        // d4, d6, d8, d10, d12, d20, d100, custom.

        // Begin app loop
        // Choose option: roll die, edit custom
        // Choose die to roll, roll it
        // Return its roll
        // OR
        // edit the custom die

        // When a specific die is accessed to roll, it will be stored in a pointer.
        return 0;
    }
};