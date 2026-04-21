#include "splashkit.h"

enum menu_options
{
    ROLL_DIE = 1,
    EDIT_CUSTOM,
    SEE_HISTORY,
    QUIT
};
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

struct die_cup
{
    Die dice[6] = {Die(4), Die(6), Die(8), Die(10), Die(12), Die(20)};
    int length = 6;
};
class Program
{

    /**
     * @brief Uses SplashKit functions to repeatedly prompt the user for input until they provie a valid integer.
     *
     * @param prompt The string to prompt the user with.
     * @return The integer inputted by the user.
     */
    int input_int(string prompt)
    {
        string input = "";
        write(prompt);
        input = read_line();
        while (!is_integer(input))
        {
            write_line("Your input of \"" + input + "\" is not a valid integer.");
            write(prompt);
        }

        return to_integer(input);
    }

    /**
     * @brief Uses SplashKit functions to repeatedly prompt the user for input until they provide a valid integer.
     *
     * @param prompt The string to prompt the user with.
     * @param lower_bound The lowest value to be accepted (inclusive).
     * @param upper_bound The highest value to be accepted (inclusive).
     * @return The integer inputted by the user.
     */
    int input_int(string prompt, int lower_bound, int upper_bound)
    {
        if (lower_bound > upper_bound)
        {
            // Swap bounds as lower is greater than upper
            int temp = upper_bound;
            upper_bound = lower_bound;
            lower_bound = temp;
        }
        int candidate;

        do
        {
            candidate = input_int(prompt);
            if (candidate < lower_bound || candidate > upper_bound)
            {
                write_line("Please input an integer between " +
                           to_string(lower_bound) +
                           " and " +
                           to_string(upper_bound) +
                           ".");
            }
        } while (candidate < lower_bound || candidate > upper_bound);

        return candidate;
    }

    void print_main_menu_options()
    {
        write_line();
        write_line("----------Main Menu----------");
        write_line("1: Roll a die");
        write_line("2: Edit the Custom Die (WIP)");
        write_line("3: See history");
        write_line("4: Quit");
    }

    int roll_a_die(die_cup &dice)
    {
        int chosen_die_index = input_int("Choose a die to roll: ", 1, dice.length);
        Die *chosen_die = &dice.dice[chosen_die_index];

        (*chosen_die).roll();
        // Future: If you ever need to do something to a die after rolling it, it can be done below.
        // Techincally the last line could be returned, but I'm doing it separately.
        return (*chosen_die).get_past_value(0);
    }

    void print_history(die_cup dice)
    {
        int chosen_die_index = input_int("Choose a die to see the history of: ", 1, dice.length);
        Die *chosen_die = &dice.dice[chosen_die_index];
        die_history history = (*chosen_die).get_history();
        string position_honorific[5] = {"st", "nd", "rd", "th", "th"};

        write_line("History of the d" + to_string(chosen_die_index) + ":");
        
        for (int i = 0; i < history.length; i++)
        {
            write_line(to_string(i) + position_honorific[i] + ": " + to_string(history.previous_rolls[i]));
        }
        write_line("End history (max 5 values).");
    }

    int main()
    {
        // Set up:
        // d4, d6, d8, d10, d12, d20, d100, custom.
        die_cup dice;
        int user_selection;
        // Begin app loop
        do
        {
            user_selection = input_int("Menu option: ", 1, 4);

            // Choose option: roll die, edit custom, see history, quit
            // Choose die to roll, roll it
            // Return its roll
            // OR
            // edit the custom die
            // OR
            // print history of the chosen die
            // quit
            switch (user_selection)
            {
            case ROLL_DIE:
                write_line("------Roll a Die------");
                int rolled = roll_a_die(dice);
                write_line("You rolled a " + to_string(rolled));
                break;
            case EDIT_CUSTOM:
                // Edit the custom die (WIP)
                write_line("------Edit the Custom Die------");
                write_line("Not yet implemented; no custom die yet");
                break;
            case SEE_HISTORY:
                write_line("------See History------");
                // See history of a die
                break;
            case QUIT:
                // Do nothing, user wants to quit
                break;
            default:
                break;
            }
        } while (user_selection != 4);

        return 0;
    }
};