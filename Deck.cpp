#include "Deck.h"
#include <cstdlib>

using namespace std;

Deck::Deck() {
    resetDeck();
}
//print deck funtion, 13 on each row
void Deck::printDeck() {
    for (int i=1; i<listofcards_.size()+1; i++) {
        cout << *listofcards_[i-1];
        if (i % 13 == 0 && i != 0) {
            cout << endl;
        }
        else {
            cout << " ";
        }
    }
    cout << endl;
}
//reset deck to create a new deck of cards in order outlined in output spec
void Deck::resetDeck() {
    listofcards_.clear();
    for (int i=0; i<SUIT_COUNT; i++) {
        for (int j=0; j<RANK_COUNT; j++) {
            Card *cardptr = new Card( Suit(i), Rank(j) );
            listofcards_.push_back( cardptr );
        }
    }
}
//shuffle function (provided code)
void Deck::shuffle(){
	int n = CARD_COUNT;
    
	while ( n > 1 ) {
		int k = (int) (lrand48() % n); //lrand48
		--n;
		Card *c = listofcards_[n];
		listofcards_[n] = listofcards_[k];
		listofcards_[k] = c;
	}
}
//deal one card at the time
Card* Deck::dealDeck(int i) {
    return listofcards_[i];
}
