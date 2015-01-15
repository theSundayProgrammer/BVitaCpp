// Brainvita.cpp : Defines the entry point for the console application.
//

/*
Module JoeChakra.BrainVita
open Microsoft.FSharp.Math
open Microsoft.FSharp.Collections
open System.Collections.Generic;
let IsValid i j =
(i>=2 && i<=4 && j>=0 && j<7) ||
(j>=2 && j<=4 && i>=0 && i<7)
*/

/*
let board = Array2D.init 7 7 (fun i j ->
(IsValid i j) && not (i=3 && j=3) )
*/
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
//const std::vector<Direction> dirs = { East, South, West, North };
Direction dirs[] = { East, South, West, North };
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
public:
	BoardGame()
	{
		initboard();
	}
	moveAction incr(moveAction const& act) const
	{
		int d, i, j;
		std::tie(d, i, j) = act;
		if (d < 3)
			return std::make_tuple(d + 1, i, j);
		else if (i < 6) 
			return std::make_tuple(0, i + 1, j);
		else
			return std::make_tuple(0, 0, j + 1);

	}
	/*
	let movableX i x y =
	board.[x,y] && board.[x+i,y] && not board.[x+2*i,y]

	let movableY j x y =
	board.[x,y] && board.[x,y+j] && not board.[x,y+2*j]
	*/

	bool movableX(int i, int x, int y) const
	{
		return board[x][y] && board[x + i][y] && !board[x + 2 * i][y];
	}

	bool movableY(int j, int x, int y) const
	{
		return	board[x][y] && board[x][y + j] && !board[x][y + 2 * j];
	}
	/*
	let moveX i x y =
	board.[x,y] <- false
	board.[x+i,y] <- false
	board.[x+2*i,y] <- true
	true

	*/
	bool moveX(int i, int x, int y)
	{
		board[x][y] = false;
		board[x + i][y] = false;
		board[x + 2 * i][y] = true;
		return true;
	}

	/*
	let moveY j x y =
	board.[x,y] <- false
	board.[x,y+j] <- false
	board.[x,y+2*j] <- true
	true
	*/
	bool  moveY(int j, int  x, int y)
	{
		board[x][y] = false;
		board[x][y + j] = false;
		board[x][y + 2 * j] = true;
		return	true;
	}




	/*
	let isMovable d x y =
	if IsValid x y then
	match d with
	| East -> (IsValid (x+2) y) && (movableX 1 x y)
	| West -> (IsValid (x-2) y) && (movableX -1 x y)
	| South -> (IsValid x (y+2)) && (movableY 1 x y)
	| North -> (IsValid x (y-2))  && (movableY -1 x y)
	else
	false
	*/

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

	/*
	let move d x y =
	match d with
	| East ->  (moveX 1 x y)
	| West ->  (moveX -1 x y)
	| South -> (moveY 1 x y)
	| North -> (moveY -1 x y)
	*/
	void move(Direction d, int x, int y)
	{
		switch (d)
		{
		case East:  moveX(1, x, y); return;
		case West:  moveX(-1, x, y);  return;
		case South:  moveY(1, x, y); return;
		case North:  moveY(-1, x, y);;
		}

	}
	moveAction findMove(moveAction const& act) const
	{
		int d, i, j;
		std::tie(d, i, j) = act;
		if (j < 7)
		{
			if (isMovable(dirs[d], i, j))
			{
				return act;
			}
			else
			{
				moveAction next = incr(act);
				return findMove(next);
			}
		}
		return invalid;

	}

};


int count = 32;
std::stack<moveAction> moves;
bool  BVMove(BoardGame const& game, moveAction const& act)
{
	moveAction next = game.findMove(act);
	while (next != invalid)
	{
		int d, i, j;
		std::tie(d, i, j) = next;
		BoardGame newBoard(game);
		newBoard.move(dirs[d], i, j);
		--count;
		if (count == 1)
		{
			moves.push(next);
			return true;
		}
		else if (BVMove(newBoard, moveAction(0, 0, 0)))
		{
			moves.push(next);
			return true;
		}
		else
		{
			++count;
			next = game.incr(next);
			next = game.findMove(next);
		}
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

int main(int argc, char* argv[])
{
	BoardGame game;
	BVMove(game, std::make_tuple(0, 0, 0));
	auto result = moves._Get_container();
	for (auto mov : result )
	{
		int d, i, j;
		std::tie(d, i, j) = mov;
		std::cout << DirStr(d) << "," << i << "," << j << std::endl;
	}
	return 0;
}

