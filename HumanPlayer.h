#ifndef HUMANPLAYER_H_INCLUDED
#define HUMANPLAYER_H_INCLUDED

#include "Player.h"
#include "Card.h"
#include <vector>

class HumanPlayer : public Player {
public:
    HumanPlayer();
    Card* play(std::vector<Card*>, Game*, bool&);  //takes input from player to play cards
};

#endif // HUMANPLAYER_H_INCLUDED
