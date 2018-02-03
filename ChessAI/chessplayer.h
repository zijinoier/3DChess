#ifndef CHESS_PLAYER_H_INCLUDED
#define CHESS_PLAYER_H_INCLUDED

class ChessBoard;
class Move;

class ChessPlayer
{
	public:

		enum Status { Normal, InCheck, Stalemate, Checkmate };

		ChessPlayer(int color)
		 : color(color)
		{};

		virtual ~ChessPlayer(){};
		

	protected:

		/*
		* Guess what...
		*/
		int color;
};

#endif
