/*
 * MVC example of GTKmm program
 *
 * View class.  Is responsible for buttons (that user clicks) and for displaying
 * the top card of the deck.
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */


#include "observer.h"
#include "view.h"
#include "controller.h"
#include "model.h"
#include "subject.h"
#include "Card.h"
#include "DeckGUI.h"
#include <iostream>
#include <string>
using namespace std;

// Creates buttons with labels. Sets butBox elements to have the same size, 
// with 10 pixels between widgets
View::View(Controller *c, Game *m) {

        game_ = m;
        controller_ = c;
	
	// Sets some properties of the window.
    	set_title( "CS247 Straights - kwdai sbalani" );
	set_border_width( 10 );

	//history
	

	//game controls
	add(gameGUI);
	gameGUI.add(controlArea);
	controlArea.add(start_button);
	start_button.set_label("Start new game with seed:");
	controlArea.add(seedField);
	seedField.set_text( "0" );	//TO DO add middle justify
	controlArea.add(end_button);
	end_button.set_label("Exit game");

	//initial table setup
	gameGUI.add(tableArea);
	Glib::RefPtr<Gdk::Pixbuf> nullPixbuf = deck.null();	
	tableArea.set_label("Cards on table:");
	tableArea.add(tableVBox);

	tableVBox.add(clubHBox);
	for(int i=0; i<13; i++) {
		tableCardPics[i] = new Gtk::Image ( nullPixbuf );
		clubHBox.add ( *tableCardPics[i] );
	}	
	tableVBox.add(diamondHBox);
        for(int i=13; i<26; i++) {
                tableCardPics[i] = new Gtk::Image ( nullPixbuf );
                diamondHBox.add ( *tableCardPics[i] );
        }
	tableVBox.add(heartHBox);
        for(int i=26; i<39; i++) {
                tableCardPics[i] = new Gtk::Image ( nullPixbuf );
                heartHBox.add ( *tableCardPics[i] );
        }
	tableVBox.add(spadeHBox);
        for(int i=39; i<52; i++) {
                tableCardPics[i] = new Gtk::Image ( nullPixbuf );
                spadeHBox.add ( *tableCardPics[i] );
        } 
	

	//initial player window
	gameGUI.add(playerArea);

	for (int i=0; i<4; i++) {
		playerArea.add(pArea[i]);
		ostringstream string;
		string << "Player " << (i+1);
		pArea[i].set_label( string.str() );
		pArea[i].add(pVBox[i]);
		pVBox[i].add(pButton[i]);
		pButton[i].set_label("Human");
		pVBox[i].add(pPoints[i]);
		pVBox[i].add(pDiscards[i]);	
	}

	//intial player hand
	gameGUI.add(handArea);
	handArea.add(handHBox);
	handArea.set_label("Your hand:");
	for(int i=0; i<13; i++){
		handHBox.add(handButtons[i]);
			handPics[i] = new Gtk::Image(nullPixbuf);
			handButtons[i].set_image(*handPics[i]);
	}

	gameGUI.add(historyArea);
	historyArea.add(history);
	history.set_editable(false);
	history.set_text("Welcome!");

	// Associate button "clicked" events with local onButtonClicked() method defined below.
	start_button.signal_clicked().connect( sigc::mem_fun( *this, &View::startButtonClicked ) );
	end_button.signal_clicked().connect( sigc::mem_fun( *this, &View::endButtonClicked ) );

	//Set up / ragequit buttons associations
	for (int i=0; i<4; i++) {
		pButton[i].signal_clicked().connect( sigc::bind (sigc::mem_fun(*this, &View::PlayerButtonClicked), i) );
	}

	//set up buttons for each card in hand
	for (int i=0; i<13; i++) {
		handButtons[i].signal_clicked().connect( sigc::bind (sigc::mem_fun(*this, &View::CardClicked), i) );
	}
	
	// The final step is to display the newly created window
	show_all();
	
	//initial
	resetHand(false);
	
	// Register view as observer of model
	game_->subscribe(this);

} // View::View

View::~View() {}

void View::UpdateHistory(){
	history.set_text( game_->getLastAction() );
}


void View::update() {
  //update GUI after each turn 
  UpdateHandPics( game_->getPlayerHand( game_->getCurrentPlayer() ) );  
  UpdateTablePics();
  UpdateHistory();
  for (int i=0; i<4; i++) {
  	if ( game_->getCurrentPlayer() != i || game_->getState() == "finished" ){
		 pButton[i].set_sensitive(false);
	}
	else {
		 pButton[i].set_sensitive(true);
	}
  }
  
  UpdateScores();
  UpdateDiscards();
}

void View::CardClicked(int cardloc) {
  controller_->CardClicked(cardloc);
}
//toggle between human and comp intially
void View::PlayerButtonClicked(int playernum) {
  if ( !game_->hasStarted() ) {
	if (pButton[playernum].get_label() == "Human") {
		pButton[playernum].set_label("Computer");
	}
	else {
		pButton[playernum].set_label("Human");
	}
  }
  else {
    controller_->PlayerButtonClicked(playernum);
	
  }
}
//start new game
void View::startButtonClicked() {
	if ( game_->getState() == "setup" ) {
		start_button.set_label("Restart game");	
 		 int seed = atoi( seedField.get_text().c_str() );
 		 controller_->startButtonClicked(seed, pButton[0].get_label(), pButton[1].get_label(), pButton[2].get_label(), pButton[3].get_label() );
  		for (int i=0; i<4; i++) {	
    			pButton[i].set_label("Ragequit!");
 		 }
		
	}
	else {
		start_button.set_label("Start new game with seed");
		for (int i=0; i<4; i++) {
			pButton[i].set_label("Human");
			pButton[i].set_sensitive(true);
		}
		clearScoreandDiscard();
		controller_->startButtonClicked(0, "", "", "", "");
	}
} // View::nextButtonClicked

void View::endButtonClicked() {
  controller_->endButtonClicked();
} // View::resetButtonClicked

void View::clearScoreandDiscard() {
	for (int i=0; i<4; i++) {
		pPoints[i].set_text("");
		pDiscards[i].set_text("");
	}
}
//scores update 
void View::UpdateScores() {
	for (int i=0; i<4; i++) {
		ostringstream string;
		string << "Points: " << game_->getPlayerScore(i);
		pPoints[i].set_text( string.str() );
	}
}
//discards updates
void View::UpdateDiscards() {
    for (int i=0; i<4; i++) {
		ostringstream string;
		string << "Discards: " << game_->getPlayerDiscard(i);
	    pDiscards[i].set_text( string.str() );
    }
}
//update players hand
void View::UpdateHandPics( vector<Card*> playerHand) {
	resetHand(true);
	Gtk::Image* blank = new Gtk::Image( deck.null() );
	for (int i=0; i < 13; i++) {
		Gtk::Image* card;
		if ( i<playerHand.size() ) {
			 card = new Gtk::Image(deck.image(playerHand[i]->getRank(), playerHand[i]->getSuit() ));
		}
		else {
			 card = blank;
			 handButtons[i].set_sensitive(false);
		}
		handButtons[i].set_image( *card);
	}
}

void View::resetHand(bool sense) {
	for (int i=0; i < 13; i++) {
		handButtons[i].set_sensitive(sense);
	}
}
//update the table of cards
void View::UpdateTablePics() {
	vector<Card*> tableCards = game_->getCardsonTable();
	int k = 0;
	for (int j=0; j<4; j++) {
		for (int i=0; i<RANK_COUNT; i++) {
			Card* temp = new Card( Suit(j), Rank(i) );
			if ( match(temp ,tableCards) ) {
				tableCardPics[k]->set(deck.image(temp->getRank(), temp->getSuit() ));

			}
			else {
				tableCardPics[k]-> set(deck.null());
			}
		k++;
		}
	}
}
