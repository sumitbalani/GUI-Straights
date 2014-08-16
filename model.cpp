/*
 * MVC example of GTKmm program
 *
 * Model class.  Is responsible keeping track of the deck of cards.  
 * It knows nothing about views or controllers.
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */


#include "model.h"

#include "Command.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
using namespace std;

ostream &operator<<(ostream &out, const vector<Card*> cardlist)
{
    if (cardlist.size() == 0) {
        cout << " ";
    }
    for (int i = 0; i < cardlist.size(); i++) {
        cout << " " << *cardlist[i];
    }
    out << endl;
    
    return out;
}

//converts number into string
string itos( int num )
{
     ostringstream ss;
     ss << num;
     return ss.str();
}

Game::Game() {
    deck = new Deck;
	state = "setup";
}

void Game::printDeck() const {
    deck->printDeck();
}

void Game::dealCards() {
    for (int i = 0; i < 4; i++) {
        int begin = i*13;
        for (int j = begin; j < begin+13 ; j++) {
            playersingame[i]->addToHand(deck->dealDeck(j));
        }
    }
}
// simple implementation that traverses through list and prints in order specified in design rubric
void Game::printTable() const{
    cout << "Cards on the table: " << endl;
    
    for (int i = 0; i < SUIT_COUNT; i++) {
        switch (i) {
            case 0:
                cout << "Clubs: ";
                break;
            case 1:
                cout << "Diamonds: ";
                break;
            case 2:
                cout << "Hearts: ";
                break;
            case 3:
                cout << "Spades: ";
                break;
        }
        bool first = true;
        for (int j = 0; j < RANK_COUNT; j++ ) {
            for (int k =0; k < cardsontable.size(); k++) {
                if (cardsontable[k]->getRank() == j && cardsontable[k]->getSuit() == i){
                    int value = j+1;
                    if (first){
                        first = false;
                    }
                    else {
                        cout << " ";
                    }
                    if (value == 10) cout << "J";
                    else if (value == 11) cout << "Q";
                    else if (value == 12) cout << "K";
                    else if (value == 1) cout << "A";
                    else cout << value;
                }
            }
        }
        cout << endl;
    }
}

void Game::startGame(int seed) {
	//init seed
	srand48(seed);
	cout << "Starting game with seed: " << seed << "!"  << endl;
    deck->resetDeck();
	cardsontable.clear();
	turnspassed = 0;
    for (int i=0; i<4; i++) {
        oldScore[i] = 0;
    }
	state = "newround";
	lastAction = "New game!";
	gameLoop();
}

void Game::resetDiscards(){
    //resets every player's discards list
    for (int i = 0; i < playersingame.size(); i++) {
        playersingame[i]->resetDiscards();
    }
}

void Game::playerQuit(int playernum) {
    //player ragequit, replace him with AI
    Player* curPlayer = playersingame[playernum];
	
    cout << "Player " << playernum+1 << " ragequits. A computer will now take over." << endl;
	ComputerPlayer* playerptr = new ComputerPlayer( curPlayer->getScore(), curPlayer->getHand(), curPlayer->getDiscards() );
	playersingame[playernum] = playerptr;
	//delete curPlayer;

    //let computer take players turn
	setState("computer");
	gameLoop();
    //no needed for part 2 code
    //bool discarded;
    //Card* played = playersingame[playernum]->play( getLegalPlaysofPlayer(playersingame[playernum]) , this, discarded);
    //cout << "Player " << currentPlayer+1 << (discarded ? " discards " : " plays ") << *played << "." << endl;
    //currentPlayer = ++currentPlayer % 4;
}

void Game::printHand(Player* playerptr) const {
    //prints out hand
    vector<Card*> playerCards = playerptr->getHand();
    
    cout << "Your hand:";
    cout << playerCards;
}
// determines legal plays for player given possible plays and his hand
vector<Card*> Game::getLegalPlaysofPlayer(Player* playerptr) const {
    vector<Card*> playersHand = playerptr->getHand();
    vector<Card*> legalTablePlays = getLegalPlaysonTable();
    vector<Card*> legalPlayerPlays;
    
    Card* SevenSpade = new Card(SPADE, SEVEN);
    if (match( SevenSpade, playersHand)) {
        legalPlayerPlays.push_back(SevenSpade);
        return legalPlayerPlays;
    }
    
    for (int i = 0; i < playersHand.size() ; i++) {
        if ( match(playersHand[i], legalTablePlays)) {
            legalPlayerPlays.push_back(playersHand[i]);
        }
    }
    return legalPlayerPlays;
}
// populates vectors with possible plays
vector<Card*> Game::getLegalPlaysonTable() const {
    vector<Card*> possiblePlays;
    
    if (cardsontable.size() == 0) {
        possiblePlays.push_back( new Card(SPADE, SEVEN) );
        return possiblePlays;
    }
    //fills deck with 7's as possible plays
    possiblePlays.push_back( new Card(HEART, SEVEN) );
    possiblePlays.push_back( new Card(CLUB, SEVEN) );
    possiblePlays.push_back( new Card(DIAMOND, SEVEN) );
    
    
    //takes care of case of A,K not being adjacent ranks
    //loops through to find every card on table
    for(int j =0; j<cardsontable.size(); j++){
    Card* lastCard = cardsontable[j];
    if (lastCard->getRank() != 0 && lastCard->getRank() != 12 ) {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()+1)) );
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()-1)) );
    }
    else if (lastCard->getRank() == 0) {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()+1)) );
    }
    else {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()-1)) );
    }
    }
    return possiblePlays;
}

void Game::addCardToTable(Card* card) {      //updates a card played on the table
    cardsontable.push_back(card);
}

bool Game::hasStarted() {
	if ( state  == "finished" || state=="setup" ) {
		return false;
	}
	else {
		return true;
	}			
}
//return score of player
int Game::getPlayerScore(int playernum) {
    int totalscore = 0;
    vector<int> playerscore = playersingame[playernum]->getScore();
    for (int j = 0; j < playerscore.size(); j++) {
	    totalscore += playerscore[j];		
	}
	return totalscore;
}

int Game::getPlayerDiscard(int playernum) {
	return playersingame[playernum]->getDiscards().size();
}
//fill in players
void Game::fillPlayers(int p[4]) {
	playersingame.clear();
	for (int i=0; i<4; i++) {
		if (p[i]==1) {
            HumanPlayer* playerptr = new HumanPlayer;
            playersingame.push_back(playerptr);
        }
        else {
            ComputerPlayer* playerptr = new ComputerPlayer;
            playersingame.push_back(playerptr);
        }
	}
}
//return a players hand
vector<Card*> Game::getPlayerHand(int playernum) {
	return playersingame[playernum]->getHand();
}

void Game::cardPlayed(int cardloc) {
	Card* cardclicked = getPlayerHand(currentPlayer).at(cardloc);
		
	bool discard = !getLegalPlaysofPlayer(playersingame[currentPlayer]).size();
	if (discard) {
		//play your turn
		string action = "Player " + itos(currentPlayer+1) + " discards " + cardToString(*cardclicked) + ".";
		cout << action;
		lastAction = action;
        	playersingame[currentPlayer]->discardFromHand(cardclicked);
		setState("playing");
	}
	else {
		if ( match(cardclicked, getLegalPlaysofPlayer(playersingame[currentPlayer])) ) {

			string action = "Player " + itos(currentPlayer+1) + " plays " + cardToString(*cardclicked) + ".";
        		cout << action;//cout << "Player " << currentPlayer+1 << " discards " << *cardclicked << "." << endl;
			lastAction = action;
			playersingame[currentPlayer]->removeFromHand(cardclicked);
			addCardToTable(cardclicked);
	        	setState("playing");
		}
		else {
			cout << "This is not a legal play." << endl;
			return;
		}
	}

	bool haveWinner = false;
    if (turnspassed == 13 && (currentPlayer+1)%4==startPlayer ) {
        turnspassed = 0;
        state = "newround";
        //print out player discards and score
        for (int i = 0; i < playersingame.size(); i++ ) {
            cout << "Player " << i+1 << "'s discards:" << playersingame[i]->getDiscards();
            int totalscore = 0;
            vector<int> playerscore = playersingame[i]->getScore();
            for (int j = 0; j < playerscore.size(); j++) {
                totalscore += playerscore[j];
            }
            cout << "Player " << i+1 << "'s score: " << oldScore[i] << " + " << totalscore-oldScore[i] << " = " << totalscore << endl;
            oldScore[i] = totalscore;
            if (oldScore[i] >= 80) {
                haveWinner = true;
            }
        }
    }


    //TO DO PUT INTO FCN
    //only inc when player plays a card (line 293)
    if (currentPlayer !=3) {
        currentPlayer++;
    }
    else {
        currentPlayer = 0;
        turnspassed++;
   	}
	 
   	if (haveWinner) {
        //find lowest score
        int lowestscore = oldScore[0];
        if (haveWinner) {
            for(int i = 0; i<4; i++) {
                if (oldScore[i] < lowestscore) {
                    lowestscore = oldScore[i];
                }
            }
        }

        //print all people with lowest score as winners
        for (int i = 0; i<playersingame.size(); i++) {
            if(oldScore[i]==lowestscore){
                cout<<"Player "<< i+1<<" wins!" << endl;
            }
        }
		setState("finished");
   	}

	//notify observer (view) of changes
	notify();
	if (state != "finished") gameLoop(); //re-enter game
}

void Game::setState(string state) {
	this->state = state;
}

void Game::gameLoop() {
	do {
	bool haveWinner = false;
    //static oldScore[4] = {0, 0, 0, 0};
	//static int turnspassed = 0;
	if (state == "newround") {
		deck->shuffle();
        dealCards();
        resetDiscards();
		cardsontable.clear();

		//find 7S to decide who goes first and announce
        for (int i = 0; i < playersingame.size(); i++ ) {
            if (match( new Card(SPADE, SEVEN), playersingame[i]->getHand() )) {
                startPlayer = i;
                break;
            }
        }
        cout << "A new round begins. It's player " << startPlayer+1 << "'s turn to play." << endl;
       	currentPlayer = startPlayer;
		state == "playing";
	}
	
    //game logic of a players turn
	Player* curPlayer = playersingame[currentPlayer];
	vector<Card*> playerLegalPlays = getLegalPlaysofPlayer( curPlayer );
	notify();
	bool discarded;
	state = "playing";
	Card* played = curPlayer->play( playerLegalPlays , this, discarded);

	//if a play was made
	if (played != NULL) {
		if ( discarded ) {
			string action = "Player " + itos(currentPlayer+1) + " discards " + cardToString(*played) + ".";
        		cout << action;//cout << "Player " << currentPlayer+1 << " discards " << *cardclicked << "." << endl;
			lastAction = action;
      	} 
        else{
			string action = "Player " + itos(currentPlayer+1) + " plays " + cardToString(*played) + ".";
        		cout << action;//cout << "Player " << currentPlayer+1 << " discards " << *cardclicked << "." << endl;
			lastAction = action;
		}
    }
    else {
        //wait for play to be made
        return; 
    }

	notify();

	if (turnspassed == 13 && (currentPlayer+1)%4==startPlayer ) {
		turnspassed = 0;
		state = "newround";
		//print out player discards and score
		for (int i = 0; i < playersingame.size(); i++ ) {
			cout << "Player " << i+1 << "'s discards:" << playersingame[i]->getDiscards();
			int totalscore = 0;
			vector<int> playerscore = playersingame[i]->getScore();
			for (int j = 0; j < playerscore.size(); j++) {
				totalscore += playerscore[j];
			}
			cout << "Player " << i+1 << "'s score: " << oldScore[i] << " + " << totalscore-oldScore[i] << " = " << totalscore << endl;
			oldScore[i] = totalscore;
			if (oldScore[i] >= 80) {
				haveWinner = true;
			}
		}
	}


    //TO DO PUT INTO FCN
    //only inc when player plays a card (line 293)
    if (currentPlayer !=3) {
        currentPlayer++;
    }
    else {
        currentPlayer = 0;
        turnspassed++;
    }

	if (haveWinner) {
		//find lowest score
    	int lowestscore = oldScore[0];
    	if (haveWinner) {
        	for(int i = 0; i<4; i++) {
            	if (oldScore[i] < lowestscore) {
                	lowestscore = oldScore[i];
            	}
        	}
    	}
    
    	//print all people with lowest score as winners
    	for (int i = 0; i<playersingame.size(); i++) {
        	if(oldScore[i]==lowestscore){
            	cout<<"Player "<< i+1<<" wins!" << endl;
        	}
    	}
		state="finished";
	}
	notify();
	} while (state=="computer" || state=="newround" );
}

int Game::getCurrentPlayer() {
	return currentPlayer;
}

vector<Card*> Game::getCardsonTable() {
	return cardsontable;
}

string Game::getState() {
	return state;
}

string Game::getLastAction() {
	return lastAction;
}
