#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#define CARD_COUNT 52

#include <vector>
#include "Card.h"
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

class Deck {
public:
    Deck();             //default constructor
    Card* dealDeck(int);//deals out card based on index
    void printDeck();   //prints out deck as it appears
    void resetDeck();   //resets deck to sorted state
    void shuffle();     //shuffles the deck randomly
private:
    std::vector<Card*> listofcards_;
};

#endif // DECK_H_INCLUDED
