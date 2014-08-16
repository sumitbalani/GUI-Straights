#include "HumanPlayer.h"
#include "Command.h"
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer() {
    
}

Card* HumanPlayer::play(std::vector<Card*> legalPlays, Game* game, bool& discarded) {
    game->printTable();
    game->printHand( this );
    
    cout << "Legal plays:";
    cout << legalPlays;
    game->setState("waiting");

    return NULL;
}
