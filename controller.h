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


#ifndef MVC_CONTROLLER_H
#define MVC_CONTROLLER_H


#include <gtkmm.h>
#include <string>
#include "model.h"

class Game;

class Controller {
public:
   Controller(Game* );
   void startButtonClicked(int, std::string, std::string, std::string, std::string);
   void endButtonClicked();
   void PlayerButtonClicked(int);
   void CardClicked(int);
private:
   Game *game_;
}; // Controller

#endif
