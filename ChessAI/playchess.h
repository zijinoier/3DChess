//#include <mcheck.h>
#include <cstdlib>
#include <cstdio>
#include <list>
#include "chessboard.h"
#include "humanplayer.h"
#include "aiplayer.h"

using namespace std;

int getAIMove(char userInput[5], bool isInit) {

	static ChessBoard board;
	static list<Move> regulars, nulls;
	static int turn = WHITE;
	static Move move;
	static bool found;
    static int step = 0;

	// Initialize players
	static AIPlayer black(BLACK, 3);
    static HumanPlayer white(WHITE);

    if(isInit == true){
        step = 0;
        return 0;
    }
    
	// setup board
    if(step == 0)
        board.initDefaultSetup();
    step++;

    for(int i=0; i<2; i++) {
		// show board
        board.print();

		// query player's choice
		if(turn)
			found = black.getMove(board, move);
		else
            found = white.getMove(board, move, userInput);

		if(!found)
			break;

		// if player has a move get all moves
		regulars.clear();
		nulls.clear();
		board.getMoves(turn, regulars, regulars, nulls);

		// execute maintenance moves
		for(list<Move>::iterator it = nulls.begin(); it != nulls.end(); ++it)
			board.move(*it);

		// execute move
		board.move(move);
		move.print();
        board.print();

		// opponents turn
		turn = TOGGLE_COLOR(turn);
	}
    int result;
    result = (int)move.from*100 + (int)move.to;
    return result;
}
