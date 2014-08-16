XX= g++
CXXFLAGS = -Wall -g `pkg-config gtkmm-2.4 --cflags` -O
LDFLAGS=`pkg-config gtkmm-2.4 --libs`
OBJS = subject.o model.o controller.o DeckGUI.o view.o main.o Card.o Command.o ComputerPlayer.o HumanPlayer.o Deck.o Player.o
EXEC = straights

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm $(EXEC) $(OBJS)

DeckGUI.o: DeckGUI.h DeckGUI.cpp
model.o: subject.h model.h model.cpp
subject.o: subject.h subject.cpp
controller.o: controller.h model.h controller.cpp
view.o: view.h observer.h controller.h model.h subject.h DeckGUI.h view.cpp
main.o: view.h controller.h model.h main.cpp
Card.o: Card.h Card.cpp
Command.o: Command.h Command.cpp
ComputerPlayer.o: ComputerPlayer.h ComputerPlayer.cpp
HumanPlayer.o: HumanPlayer.h HumanPlayer.cpp
Deck.o: Deck.h Deck.cpp
Player.o: Player.h Player.cpp


