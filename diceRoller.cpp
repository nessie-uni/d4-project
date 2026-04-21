#include "splashkit.h"

struct die_history
{
    int previous_rolls[5];
    int length = 0;

    void add(int new_roll)
    {

        for (int i = length - 1; i > 0; i++)
        {
            if (i == 1)
            {
                previous_rolls[0] = new_roll;
                continue;
            }
            previous_rolls[i] = previous_rolls[i - 1];
        }
        if (length < 5)
        {
            length++;
        }
    }
};

class Die
{
protected:
    int _maximum;
    die_history _history;

public:
    Die()
    {
        Die(6);
    }

    Die(int size)
    {
        if (size <= 1)
        {
            throw "Die size must be 2 or greater";
        }

        _maximum = size;
    }

    int roll()
    {
        long random_value = random();
        int value = (int)(random_value * _maximum) + 1;
        _history.add(value);
        return value;
    }

    die_history get_history()
    {
        return _history;
    }

    int get_past_value(int history_counter)
    {
        if (history_counter >= _history.length || history_counter < 0)
        {
            throw "Index out of bounds";
        }
        return _history.previous_rolls[history_counter];
    }
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
    CustomDie(int options[6]) {}
    int roll() {}
};

class Program
{
    int main()
    {
        // Set up:
        // d4, d6, d8, d10, d12, d20, d100, custom.

        // Begin app loop
        // Choose option: roll die, edit custom, see history
        // Choose die to roll, roll it
        // Return its roll
        // OR
        // edit the custom die
        // OR
        // print history of the chosen die

        // When a specific die is accessed to roll, it will be stored in a pointer.
        return 0;
    }
};