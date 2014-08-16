#ifndef COMPUTERPLAYER_H_INCLUDED
#define COMPUTERPLAYER_H_INCLUDED

#include "Player.h"
#include "Card.h"
#include "Command.h"
#include <vector>

class ComputerPlayer : public Player {
public:
    ComputerPlayer();
    ComputerPlayer(std::vector<int>, std::vector<Card*>, std::vector<Card*>);
    Card* play(std::vector<Card*>, Game*, bool&);  //automatically plays given legal plays
};


#endif // COMPUTERPLAYER_H_INCLUDED
