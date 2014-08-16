/*
 * MVC example of GTKmm program
 *
 * Controller class.  Is responsible for translating UI events (from the View)
 * into method calls to the Model.
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */


#include "controller.h"
#include "model.h"

using namespace std;

Controller::Controller( Game *m) {
	game_ = m;
}

void Controller::startButtonClicked(int seed, string p1, string p2, string p3, string p4) {
	//game started
	if (p1=="") {
		game_->setState("setup");
	}
	else {
	int playertype[4];
	playertype[0] = p1=="Human" ? 1 : 0;
	playertype[1] = p2=="Human" ? 1 : 0;
	playertype[2] = p3=="Human" ? 1 : 0;
	playertype[3] = p4=="Human" ? 1 : 0;
	game_->fillPlayers(playertype);
	game_->startGame(seed);
	}
} 

//signal fcns
void Controller::endButtonClicked() {
	exit(0); 
}

void Controller::PlayerButtonClicked(int player) {
	game_->playerQuit(player);
}

void Controller::CardClicked(int cardloc) {
	game_->cardPlayed(cardloc);	
} 
