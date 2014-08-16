#include "ComputerPlayer.h"
#include <iostream>

using namespace std;

ComputerPlayer::ComputerPlayer() {
    
}
//computer constructor, takes care of ragequit case, copy over human information
ComputerPlayer::ComputerPlayer(std::vector<int> copyScore, std::vector<Card*> copyHand, std::vector<Card*> copyDiscards) {
    score_ = copyScore;
    cardsinhand_ = copyHand;
    discardslist_ = copyDiscards;
}

//computer play strategy, implement play with play first legal play
Card* ComputerPlayer::play(std::vector<Card*> legalPlays, Game* game, bool& discarded) {
    game->setState("computer");
	if (legalPlays.size() != 0) {
        //play first legal play in hand
        game->addCardToTable( legalPlays[0] );
        removeFromHand( legalPlays[0] );
        discarded = false;
        return legalPlays[0];
    }
    else {
        //discards first card in hand
        discarded = true;
        Card* toDiscard = cardsinhand_[0];
        discardFromHand( toDiscard );
        return toDiscard;
    }
}
