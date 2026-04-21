#include "splashkit.h"
#include <cstdlib>
#include <ctime>

/**
 * @brief The menu options within the Roller class.
 * 
 */
enum menu_options
{
    ROLL_DIE = 1,
    EDIT_CUSTOM,
    SEE_HISTORY,
    QUIT_APP
};

/**
 * @brief A structure to hold the history of a die. Maximum length: 5.
 * 
 */
struct die_history
{
    int previous_rolls[5] = {-1, -1, -1, -1, -1};
    int length = 0;

    void add(int new_roll)
    {
        if (length == 0)
        {
            previous_rolls[0] = new_roll;
            length = 1;
            return;
        }

        for (int i = length; i >= 0; i--)
        {
            if (i == 5)
            {
                continue;
            }
            if (i == 0)
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

/**
 * @brief A rollable die with history of the past (up to) 5 values. Maximum value can be set in the constructor.
 * 
 */
class Die
{
protected:
    int _maximum;
    die_history _history;

public:
    /**
     * @brief Construct a new Die object. Size will be 6.
     * 
     */
    Die()
    {
        Die(6);
    }

    /**
     * @brief Construct a new Die object.
     * 
     * @param size The maximum value on the die (6 gives a classic die).
     */
    Die(int size)
    {
        if (size <= 1)
        {
            throw "Die size must be 2 or greater";
        }

        _maximum = size;
    }

    /**
     * @brief Rolls the die.
     * 
     * @return The value that the die rolled.
     */
    int roll()
    {
        int value = (rand() % _maximum) + 1;
        _history.add(value);
        return value;
    }

    /**
     * @brief Get the history of the die.
     * 
     * @return The history of the die (up to 5 rolls).
     */
    die_history get_history()
    {
        return _history;
    }

    /**
     * @brief Get an historical roll of the die. 
     * 
     * @param history_counter The number of values to go into the history. Throws an exception (string) if value does not exist. Minimum value: 1. Maimum value: min(5, _history.length).
     * @return The historical roll of the die.
     */
    int get_past_value(int history_counter)
    {
        if (history_counter > _history.length || history_counter <= 0)
        {
            throw "Index out of bounds";
        }
        return _history.previous_rolls[history_counter - 1];
    }

    /**
     * @brief Returns the type of the die (e.g. d4, d6)
     * 
     * @return The die type. 
     */
    string die_type()
    {
        return "d" + to_string(_maximum);
    }
};

/**
 * @brief A die with customisable faces. It can be set to have a custom minimum and maximum, and/or a custom step between faces, or six unique faces.
 * 
 */
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
    int roll()
    {
        // PLACEHOLDER
        return 1;
    }
};

/**
 * @brief Designed to hold the dice used in the program. Holds a d4, d6, d8, d10, d12, and a d20.
 * 
 */
struct die_cup
{
    Die dice[6] = {Die(4), Die(6), Die(8), Die(10), Die(12), Die(20)};
    int length = 6;
};

/**
 * @brief The class that operates the dice roller. One public method: int main().
 *
 */
class Roller
{
private:
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

    /**
     * @brief Prints the main menu of the dice roller app. Options are roll a die, edit the custom die, see the history of a die, and quit.
     *
     */
    void print_main_menu_options()
    {
        write_line();
        write_line("----------Main Menu----------");
        write_line("1: Roll a die");
        write_line("2: Edit the Custom Die (WIP)");
        write_line("3: See history");
        write_line("4: Quit");
    }

    /**
     * @brief Prints the dice in the given dice cup, including a numeric count.
     *
     * @param dice The dice to print.
     */
    void print_dice(die_cup dice)
    {
        for (int i = 0; i < dice.length; i++)
        {
            write_line("Die " + to_string(i + 1) + ": " + dice.dice[i].die_type());
        }
    }

    /**
     * @brief Rolls a user-chosen die from the die cup. Returns the value that the die rolls.
     *
     * @param dice The dice that the user can choose from to roll.
     * @return What the die rolled.
     */
    int roll_a_die(die_cup &dice)
    {
        print_dice(dice);
        int chosen_die_index = input_int("Choose a die to roll: ", 1, dice.length);
        Die *chosen_die = &dice.dice[chosen_die_index - 1];
        (*chosen_die).roll();
        // Future: If you ever need to do something to a die after rolling it, it can be done below.
        // Techincally the line before this comment could be returned, but I'm doing it separately.
        int past_value = (*chosen_die).get_past_value(1);

        return past_value;
    }

    /**
     * @brief Prints the history of a user-chosen die from the die cup. Due to how the history struct is modified, shows a maximum of 5 values.
     *
     * @param dice The dice that the user can choose from to print the history of.
     */
    void print_history(die_cup dice)
    {
        print_dice(dice);
        int chosen_die_index = input_int("Choose a die to see the history of: ", 1, dice.length);
        Die *chosen_die = &dice.dice[chosen_die_index - 1];
        die_history history = (*chosen_die).get_history();
        string position_honorific[5] = {"st", "nd", "rd", "th", "th"};

        write_line("History of the " + (*chosen_die).die_type() + ":");

        for (int i = 0; i < history.length; i++)
        {
            write_line(to_string(i + 1) + position_honorific[i] + ": " + to_string(history.previous_rolls[i]));
        }
        write_line("End history (max 5 values).");
    }

public:
    /**
     * @brief Runs the dice roller.
     *
     * @return Status code.
     */
    int main()
    {
        // Set up:
        // d4, d6, d8, d10, d12, d20, d100, custom.
        die_cup dice;
        int user_selection;
        // Begin app loop
        do
        {
            print_main_menu_options();
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
            {
                write_line("------Roll a Die------");
                int rolled = roll_a_die(dice);
                write_line("You rolled a " + to_string(rolled));
                break;
            }
            case EDIT_CUSTOM:
                // Edit the custom die (WIP)
                write_line("------Edit the Custom Die------");
                write_line("Not yet implemented; no custom die yet");
                break;
            case SEE_HISTORY:
                write_line("------See History------");
                print_history(dice);
                // See history of a die
                break;
            case QUIT_APP:
                // Do nothing, user wants to quit
                break;
            default:
                break;
            }
        } while (user_selection != 4);

        return 0;
    }
};

/**
 * @brief The entry point of the program.
 *
 * @return Status code.
 */
int main()
{
    srand(time(0));
    Roller roller = Roller();
    return roller.main();
}