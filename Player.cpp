#include "Player.h"
#include <iostream>//test remove

Player::Player() {
    
}
//mutator for cardsinhand
void Player::addToHand(Card* card) {
    cardsinhand_.push_back(card);
}
//accessor for cardsinhand
std::vector<Card*> Player::getHand() {
    return cardsinhand_;
}
//accessor for score
std::vector<int> Player::getScore() {
    return score_;
}
//accessor for discards list
std::vector<Card*> Player::getDiscards() {
    return discardslist_;
}
//clear discards
void Player::resetDiscards() {
    discardslist_.clear();
}
//removes card from hand once played
void Player::removeFromHand(Card* card) {
    for (int i = 0; i < cardsinhand_.size(); i++) {
        if (*card == *cardsinhand_[i]) {
            cardsinhand_.erase(cardsinhand_.begin() + i);
        }
    }
    score_.push_back(0); //score of 0 was added
}
//removes card from hand and places in discards list 
void Player::discardFromHand(Card* card) {
    removeFromHand(card);
    discardslist_.push_back(card);           //add to discards list
    score_.push_back( card->getRank()+1 );  //keep track of score
}

void Player::resetPlayer() {
	resetDiscards();
	score_.clear();
	cardsinhand_.clear();
}
