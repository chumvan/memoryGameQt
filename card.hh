/* Class: Card
 * -----------
 * Represents a single card in pairs (memory) game.
 *
 * COMP.CS.110 K2021
 * */

#ifndef CARD_HH
#define CARD_HH

enum Visibility_type {OPEN, HIDDEN, EMPTY};
const char HIDDEN_CHAR = '#';
const char EMPTY_CHAR = '.';

class Card
{
public:
    // Default constructor: creates an empty card.
    Card();

    // Constructor: creates a hidden card with the given character.
    Card(const char c);

    // Obvious setter and getter methods.
    void set_letter(const char c);
    void set_visibility(const Visibility_type visibility);
    char get_letter();
    Visibility_type get_visibility() const;

    // Turns a card: changes the visibility from open to hidden and vice versa.
    void turn();

    // Prints a card based on its current state (visibility).
    void print() const;

    // Removes a card from the game board: changes the visibility to empty.
    void remove_from_game_board();

private:
    char letter_;
    Visibility_type visibility_;
};

#endif // CARD_HH
