#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Card.h"
#include "model.h"
#include <vector>

class Game;

class Player {
public:
    Player();                           //constructor
    virtual Card* play(std::vector<Card*>, Game*, bool&) = 0;   //the way its meant to be played
    void addToHand(Card*);              //adds new card to player hand
    void removeFromHand(Card*);         //removes card from player hand
    void discardFromHand(Card*);        //updates discard list of player with cards
    std::vector<int> getScore();        //gets the score of player
    std::vector<Card*> getHand();
    std::vector<Card*> getDiscards();
    void resetDiscards();
	void resetPlayer();
protected:
    std::vector<int> score_;
    std::vector<Card*> cardsinhand_;
    std::vector<Card*> discardslist_;
};

#endif // PLAYER_H_INCLUDED
