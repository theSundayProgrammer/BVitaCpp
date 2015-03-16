// Brainvita.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
typedef std::tuple<int, int, int> moveAction;
const moveAction invalid(-1, -1, -1);
enum Direction
{
	East,
	South,
	West,
	North
};
//const std::vector<Direction> dirs =  { North,West,South,East };
Direction dirs[] = {  East, South, West,North };
class BoardGame
{
	bool board[7][7];
	bool IsValid(int i, int j) const
	{
		return (i >= 2 && i <= 4 && j >= 0 && j < 7) ||
			(j >= 2 && j <= 4 && i >= 0 && i < 7);
	}
	void initboard()
	{
		for (int i = 0; i < 7; ++i)
			for (int j = 0; j < 7; ++j)
			{
				board[i][j] = IsValid(i, j) && !(i == 3 && j == 3);
			}
	}

	moveAction incr(moveAction const& act) const
	{
		int d, i, j;
		std::tie(d, i, j) = act;
		if (d < 3)
			return std::make_tuple(d + 1, i, j);
		else if (i < 6)
			return std::make_tuple(0, i + 1, j);
		else if ( j<6 )
			return std::make_tuple(0, 0, j + 1);
		else 
			return invalid;

	}
	void move(Direction d, int x, int y)
	{
		switch (d)
		{
		case East:  moveX(1, x, y); return;
		case West:  moveX(-1, x, y);  return;
		case South:  moveY(1, x, y); return;
		case North:  moveY(-1, x, y);
		}

	}

	bool movableX(int i, int x, int y) const
	{
		return board[x][y] && board[x + i][y] && !board[x + 2 * i][y];
	}

	bool movableY(int j, int x, int y) const
	{
		return	board[x][y] && board[x][y + j] && !board[x][y + 2 * j];
	}

	bool moveX(int i, int x, int y)
	{
		board[x][y] = false;
		board[x + i][y] = false;
		board[x + 2 * i][y] = true;
		return true;
	}


	bool  moveY(int j, int  x, int y)
	{
		board[x][y] = false;
		board[x][y + j] = false;
		board[x][y + 2 * j] = true;
		return	true;
	}

	bool isMovable(Direction d, int  x, int y) const
	{
		if (IsValid(x, y))
		{
			switch (d)
			{
			case East: return IsValid(x + 2, y) && movableX(1, x, y);
			case West: return IsValid(x - 2, y) && movableX(-1, x, y);
			case South: return IsValid(x, y + 2) && movableY(1, x, y);
			case North: return IsValid(x, y - 2) && movableY(-1, x, y);
			default: return false;
			}
		}
		else
			return false;
	}

public:
	BoardGame()
	{
		initboard();
	}


	moveAction findFirstMove() const
	{
		return findNextMove(moveAction(0, 0, 0));
	}
	moveAction findNextMove(moveAction const& act) const
	{
		moveAction next = incr(act);
		while (next != invalid)
		{
			int d, i, j;
			std::tie(d, i, j) = next;
			if (isMovable(dirs[d], i, j))
				return  next;
			next = incr(next);	
		}
		return next;

	}
	bool isBoardCountOne() const
	{
		int k = 0;
		for (int i = 0; i < 7; ++i)
			for (int j = 0; j < 7; ++j)
				if (board[i][j])
					if (++k > 1) 
						return false;
		return board[3][3];
	}
	BoardGame move(moveAction const& act) const
	{
		int d, i, j;
		std::tie(d, i, j) = act;
		BoardGame newBoard(*this);
		newBoard.move(dirs[d], i, j);
		return newBoard;
	}
};



std::stack<moveAction> moves;
bool  Solver(BoardGame const& game)
{
	if (game.isBoardCountOne())
	{
		return true;
	}
	moveAction next = game.findFirstMove();

	while (next != invalid)
	{
		if (Solver(game.move(next)))
		{
			moves.push(next);
			return true;
		}
		next = game.findNextMove(next);
	}

	return false;
} 
const char* DirStr(int d)
{
	switch (dirs[d])
	{
	case North:return "North";
	case South:return "South";
	case East:return "East";
	case West:return "West";
	default: return "unknown";
	}
}
void printOutput()
{
	auto result = moves._Get_container();
	auto it = result.rbegin();
	auto fin = result.rend();
	for (; it != fin;++it)
	{
		int d, i, j;
		std::tie(d, i, j) = *it;
		std::cout << i << "," << j << "," << DirStr(d) << std::endl;
	}
}
int main(int argc, char* argv[])
{
	BoardGame game;
	Solver (game);
	printOutput();
	return 0;
}

