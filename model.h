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


#ifndef MVC_MODEL_H
#define MVC_MODEL_H

#include "subject.h"
#include "Player.h"
#include "Deck.h"
#include "Card.h"
#include <vector>
#include <iostream>
#include <string>
class Player;

const int numFaces = 6;
const int numSuits = 4;
const int numCards = numFaces * numSuits;

class Game : public Subject{
public:
    Game();                         //constructor
    void addCardToTable(Card*);      //updates a card played on the table
    void dealCards();               //deals cards to players
    void printTable() const;        //prints cards on the table
    void printDeck() const;         //prints out deck as it appears
    void startGame(int);
    void printHand(Player*) const;
    std::vector<Card*> getLegalPlaysofPlayer(Player*) const; //returns legal plays of player
    std::vector<Card*> getLegalPlaysonTable() const;
    std::vector<Card*> getPlayerHand(int);
    void resetDiscards();		
    bool hasStarted();		    //checks if game has started	
    int getPlayerScore(int);        // gets players score
    int getPlayerDiscard(int);      
    void fillPlayers(int[4]);
    void cardPlayed(int);
    void playerQuit(int);
    void setState(std::string);
    std::string getState();
    void gameLoop();
    int getCurrentPlayer();
    std::vector<Card*> getCardsonTable();
    std::string getLastAction();
private:
    //manage data members as private
    std::vector<Player*> playersingame;
    std::vector<Card*> cardsontable;
    int currentPlayer;
    Deck* deck;
    std::string state;
    int turnspassed;
    int oldScore[4];
    int startPlayer;
    std::string lastAction;
}; //Model

    std::ostream &operator<<(std::ostream &out, const std::vector<Card*> cardlist);

#endif
