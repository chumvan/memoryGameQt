#include "player.hh"
#include <string>
// Add class implementation here.
// It should be enough to write only 1-2 code lines in each public method.

Player::Player():name_("Unknown"){}

Player::Player(const std::string& name):name_(name){}

std::string Player::get_name() const{
    return name_;
}

void Player::add_card(Card &card){
    pairs_.push_back(card.get_letter());
}

unsigned int Player::number_of_pairs() const { return pairs_.size();}

void Player::print() const{
    std::cout <<"*** " << get_name() <<" has " << number_of_pairs() << " pair(s)."<<std::endl;
}

std::string Player::get_correct_pairs()
{
    std::string pairs_string ="";
    for(auto p : pairs_)
    {
        pairs_string.push_back(p);
        pairs_string += " | ";
    }
    return pairs_string;
}
