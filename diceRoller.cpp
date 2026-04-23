#include "splashkit.h"
#include <cstdlib>
#include <ctime>
#include <climits>
#include <typeinfo>
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
 * @brief The options for the custom die's operation.
 *
 */
enum custom_modes
{
    MIN_TO_MAX = 0,
    MIN_TO_MAX_STEP,
    SIX_UNIQUE
};

/**
 * @brief The menu options within the edit custom die menu.
 *
 */
enum edit_custom_menu_options
{
    ONE_TO_X = 1,
    X_TO_Y,
    X_TO_Y_STEP_Z,
    UNIQUE_FACES,
};

/**
 * @brief A structure to hold the history of a die. Maximum length: 5.
 *
 */
struct die_history
{
public:
    /**
     * @brief A list of the historical rolls of the die. At most five.
     *
     */
    int previous_rolls[5] = {-1, -1, -1, -1, -1};

    /**
     * @brief The length of the list of historical values of the die. At most five.
     *
     */
    int length = 0;

    /**
     * @brief Adds a new roll to the history of the die, moving older rolls as necessary.
     *
     * @param new_roll The new value to be added to the history.
     */
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
 * @brief Designed to hold the dice used in the program. Holds a d4, d6, d8, d10, d12, d20, d100, and the custom die. Not intended for use by anything else.
 *
 */
struct die_cup
{
    /**
     * @brief A list of regular dice held in the die cup.
     *
     */
    Die dice[7] = {Die(4), Die(6), Die(8), Die(10), Die(12), Die(20), Die(100)};

    /**
     * @brief The number of REGULAR dice in the die cup.
     *
     */
    int length = 7;

    /**
     * @brief A custom die in the die cup. By default, it is a d1000.
     *
     */
    CustomDie custom = CustomDie(1000, 1, 1);
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
     * @brief Construct a new Die object.
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
    virtual int roll()
    {
        int value = (rand() % _maximum) + 1;
        _history.add(value);
        return value;
    }

    /**
     * @brief Get the history of the die.
     *
     * @return The history of the die in a die_history struct.
     */
    die_history get_history()
    {
        return _history;
    }

    /**
     * @brief Get an historical roll of the die.
     *
     * @param history_counter The number of values to go into the history. Throws an exception (string) if value does not exist. Minimum value: 1. Maximum value: min(5, _history.length).
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
     * @brief Returns the type of the die (e.g. d4, d6, custom)
     *
     * @return The die type.
     */
    virtual string die_type()
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
    int _minimum;
    int _step;
    int _options[6]{-1, -1, -1, -1, -1, -1};
    int _mode;

public:
    /**
     * @brief Construct a new Custom Die object.
     *
     */
    CustomDie()
    {
        _minimum = 1;
        _maximum = 6;
        _step = 1;
        _mode = MIN_TO_MAX;
    }

    /**
     * @brief Construct a new Custom Die object.
     * 
     * @param maximum The maximum value on the die (inclusive).
     */
    CustomDie(int maximum) : CustomDie(maximum, 1, 1) {}

    /**
     * @brief Construct a new Custom Die object.
     * 
     * @param maximum The maximum value on the die (inclusive).
     * @param minimum The minimum value on the die (inclusive). Cannot equal maximum.
     */
    CustomDie(int maximum, int minimum) : CustomDie(maximum, minimum, 1) {}

    /**
     * @brief Construct a new Custom Die object.
     * 
     * @param maximum The maximum value on the die (inclusive).
     * @param minimum The minimum value on the die (inclusive). Cannot equal maximum.
     * @param step The difference between values on the die. Must be greater than 0.
     */
    CustomDie(int maximum, int minimum, int step)
    {
        if (minimum == maximum)
        {
            throw "Minimum cannot equal maximum";
        }

        if (step < 1)
        {
            throw "Step must be greater than 0";
        }

        if (minimum > maximum)
        {
            // Swap so that max > min
            int temp = maximum;
            maximum = minimum;
            minimum = temp;
        }

        _minimum = minimum;
        _maximum = maximum;
        _step = step;

        if (_step > 1)
        {
            _mode = MIN_TO_MAX_STEP;
        }
        else
        {
            _mode = MIN_TO_MAX;
        }
    }

    /**
     * @brief Construct a new Custom Die object.
     * 
     * @param options A list of six custom faces for the die.
     */
    CustomDie(int options[6])
    {
        // Copy options to _options (surprisingly difficult task in C++)
        for (int i = 0; i < 6; i++)
        {
            write_line(options[i]);
            _options[i] = options[i];
        }

        _mode = SIX_UNIQUE;
    }
    
    /**
     * @brief Rolls the die.
     *
     * @return The value that the die rolled.
     */
    int roll() override
    {
        int value;
        switch (_mode)
        {
        case MIN_TO_MAX:
            value = (rand() % (_maximum - _minimum + 1)) + _minimum;
            break;
        case MIN_TO_MAX_STEP:
        {
            int num_options = 0;
            while (_minimum + (_step * num_options) <= _maximum)
            {
                num_options++;
            }

            int index = rand() % num_options;
            value = _minimum + _step * index;
        }
        break;
        case SIX_UNIQUE:
            value = _options[(rand() % 6)];
            break;

        default:
            break;
        }
        _history.add(value);

        return value;
    }

    /**
     * @brief Returns the type of the die (e.g. d4, d6, custom)
     *
     * @return The die type.
     */
    string die_type() override
    {
        switch (_mode)
        {
        case MIN_TO_MAX:
            if (_minimum == 1)
            {
                return "d" + to_string(_maximum);
            }
            return "Die with faces between " +
                   to_string(_minimum) + " and " +
                   to_string(_maximum);
            break;

        case MIN_TO_MAX_STEP:
            return "Die with faces between " +
                   to_string(_minimum) + " and " +
                   to_string(_maximum) + " increasing by " +
                   to_string(_step);
            break;

        case SIX_UNIQUE:
            return "Die with six custom faces";
            break;
        default:
            break;
        }
        return "Custom die";
    }
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
            input = read_line();
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
        Die *die;
        for (int i = 0; i < dice.length + 1; i++)
        {
            if (i == dice.length)
            {
                die = &dice.custom;
            }
            else
            {
                die = &dice.dice[i];
            }

            write_line("Die " + to_string(i + 1) + ": " + die->die_type());
        }
    }

    /**
     * @brief Prints the custom edit menu.
     *
     */
    void print_edit_custom_menu()
    {
        write_line("Choose a type of custom die:");
        write_line("1: Faces from 1-x (dx)");
        write_line("2: Faces from x-y");
        write_line("3: Faces from x-y increasing by z");
        write_line("4: Six custom faces");
    }

    /**
     * @brief Prints the history of a user-chosen die from the die cup. Due to how the history struct is modified, shows a maximum of 5 values.
     *
     * @param dice The dice that the user can choose from to print the history of.
     */
    void print_history(die_cup dice)
    {
        print_dice(dice);
        int chosen_die_index = input_int("Choose a die to see the history of: ", 1, dice.length + 1);
        Die *chosen_die;
        if (chosen_die_index == dice.length + 1)
        {
            chosen_die = &dice.custom;
        }
        else
        {
            chosen_die = &dice.dice[chosen_die_index - 1];
        }

        die_history history = (*chosen_die).get_history();
        string position_honorific[5] = {"st", "nd", "rd", "th", "th"};

        write_line("History of the " + (*chosen_die).die_type() + ":");

        for (int i = 0; i < history.length; i++)
        {
            write_line(to_string(i + 1) + position_honorific[i] + ": " + to_string(history.previous_rolls[i]));
        }
        write_line("End history (max 5 values).");
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
        int chosen_die_index = input_int("Choose a die to roll: ", 1, dice.length + 1);
        if (chosen_die_index == dice.length + 1)
        {
            return dice.custom.roll();
        }
        Die *chosen_die = &dice.dice[chosen_die_index - 1];
        chosen_die->roll();
        // Future: If you ever need to do something to a die after rolling it, it can be done below.
        // Techincally the line before this comment could be returned, but I'm doing it separately.
        int past_value = (*chosen_die).get_past_value(1);

        return past_value;
    }

    /**
     * @brief Prompts the user to edit the custom die.
     *
     * @return The updated (or previous) custom die.
     */
    CustomDie edit_custom_die()
    {
        print_edit_custom_menu();
        int choice = input_int("Menu option: ", 1, 4);
        switch (choice)
        {
        case ONE_TO_X:
        {
            int upper_bound = input_int("Choose an upper bound: ", 2, INT_MAX);
            return CustomDie(upper_bound);
            break;
        }
        case X_TO_Y:
        {
            int lower_bound = input_int("Choose a lower bound: ", INT_MIN, INT_MAX - 1);
            int upper_bound = input_int("Choose an upper bound: ", lower_bound + 1, INT_MAX);
            return CustomDie(upper_bound, lower_bound);
        }
        case X_TO_Y_STEP_Z:
        {
            int lower_bound = input_int("Choose a lower bound: ", INT_MIN, INT_MAX - 1);
            int upper_bound = input_int("Choose an upper bound: ", lower_bound + 1, INT_MAX);
            int step = input_int("Choose a step: ", 1, upper_bound - lower_bound);
            return CustomDie(upper_bound, lower_bound, step);
        }
        case UNIQUE_FACES:
        {
            int faces[6];
            string position_honorific[6] = {"st", "nd", "rd", "th", "th", "th"};

            for (int i = 0; i < 6; i++)
            {
                faces[i] = input_int("Input the " + to_string(i + 1) + position_honorific[i] + " value: ");
            }
            return CustomDie(faces);
            break;
        }

        default:
            break;
        }
        return CustomDie(6);
    }

public:
    /**
     * @brief Test modules for the Custom Die.
     *
     */
    void test_custom_die()
    {
        write_line("1 to 5, step 2 (i.e. 1, 3, 5)");
        CustomDie d = CustomDie(5, 1, 2);
        int result = 0;
        do
        {
            result = d.roll();
            write_line(result);
        } while (result != 5);

        write_line("1 to 5");
        CustomDie d2 = CustomDie(5, 1);
        int result2 = 0;
        do
        {
            result2 = d2.roll();
            write_line(result2);
        } while (result2 != 5);

        write_line("6 custom options");
        int options[6] = {1, 7, 10, 3, 11, 5};
        CustomDie d3 = CustomDie(options);
        int result3 = 0;
        do
        {
            result3 = d3.roll();
            write_line(result3);
        } while (result3 != 5);
    }
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
            write_line();
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
                dice.custom = edit_custom_die();
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