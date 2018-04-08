
// GSP480 Chris Conway
#pragma once

#include <array>
#include <iostream>
#include <random>
#include <cassert>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;

typedef std::int8_t int8;
typedef std::uint8_t uint8;
typedef std::int32_t int32;
typedef std::uint32_t uint32;
typedef std::int64_t int64;
typedef std::uint64_t uint64;



//////////////////////////////////////////////////////////////////////////
// Solving by Dijkstra


// This is a custom board structure to make implementing Dijkstra easier.

struct FifteenTilesBoard
{
public:
	enum class EMoveDirection
	{
		Unknown,
		Up,
		Down,
		Left,
		Right
	};




private:
	// Do not change this as it will break the board packing
	static const int32 BoardSize = 4;
	static const uint8 InvalidValue;
	static const uint64 WonGame;

	uint64 BoardData;

	uint8 ReadBoard(int X, int Y) const
	{
		if (X < 0 || X >= BoardSize ||
			Y < 0 || Y >= BoardSize)
		{
			return InvalidValue;
		}

		int32 CellShift = (15 - (Y * BoardSize + X)) * 4;
		return (BoardData >> CellShift) & 15L;
	}

	void WriteBoard(int X, int Y, uint8 Value)
	{
		if (X < 0 || X >= BoardSize ||
			Y < 0 || Y >= BoardSize)
		{
			return;
		}
		int32 CellShift = (15 - (Y * BoardSize + X)) * 4;
		uint64 Mask = ~(((uint64)0xF) << CellShift);
		uint64 ValueShift = ((uint64)Value) << CellShift;
		BoardData = (BoardData & Mask) | ValueShift;
	}

	void FindEmptySpace(int& X, int& Y) const
	{
		// This is debatable if it's worth it, but it does save memory to keeping the empty space position around

		// Or the 4 bits of each spot together, this should basically leave only 1 bit at 0

		uint64 Mask1 = (BoardData >> 0) & 0x1111111111111111;
		uint64 Mask2 = (BoardData >> 1) & 0x1111111111111111;
		uint64 Mask3 = (BoardData >> 2) & 0x1111111111111111;
		uint64 Mask4 = (BoardData >> 3) & 0x1111111111111111;


		uint64 Mask = Mask1 | Mask2 | Mask3 | Mask4;
		//Mask = (Mask << 0) | (Mask << 1) | (Mask << 2) | (Mask << 3);

		// Now invert the mask which means we should have a single bit set
		Mask = ~Mask;
		Mask = Mask & 0x1111111111111111;

		int CellPosition = 0;

		// Now we can find it in 3 steps by binary search
		if (Mask >= 0x100000000)
		{
			Mask >>= 32;
		}
		else
		{
			CellPosition = 8;
		}

		if (Mask >= 0x10000)
		{
			Mask >>= 16;
		}
		else
		{
			CellPosition += 4;
		}

		if (Mask >= 0x100)
		{
			Mask >>= 8;
		}
		else
		{
			CellPosition += 2;
		}

		assert(Mask == 0x1 || Mask == 0x10);

		if (Mask == 0x1)
		{
			CellPosition += 1;
		}

		Y = CellPosition / 4;
		X = CellPosition % 4;
	}

	bool CalculateMovePosition(EMoveDirection Direction, int& OutCurrentX, int& OutCurrentY, int& OutNewX, int& OutNewY) const
	{
		FindEmptySpace(OutCurrentX, OutCurrentY);
		OutNewX = OutCurrentX;
		OutNewY = OutCurrentY;

		switch (Direction)
		{
		case EMoveDirection::Up:
			OutNewY++;
			break;
		case EMoveDirection::Down:
			OutNewY--;
			break;
		case EMoveDirection::Left:
			OutNewX--;
			break;
		case EMoveDirection::Right:
			OutNewX++;
			break;
		default:
			assert(false && "We shouldn't have gotten here.");
		}

		// Check that we do have the empty space at the right position
		assert(ReadBoard(OutCurrentX, OutCurrentY) == 0);

		// Is this a valid move?
		if (OutNewX < 0 || OutNewX >= BoardSize ||
			OutNewY < 0 || OutNewY >= BoardSize)
		{
			return false;
		}

		return true;
	}


	uint64 GetKey() const
	{
		return BoardData;
	}

public:

	FifteenTilesBoard()
	{
		BoardData = 0;
	}

	bool IsSolvable() const
	{
		auto GetCellValue = [&](int32 Index) -> uint8
		{
			int32 CellShift = (15 - Index) * 4;
			return (BoardData >> CellShift) & 15L;
		};

		auto GetInversionCount = [&]() -> int32
		{
			int32 InversionCount = 0;
			for (int32 I = 0; I < BoardSize * BoardSize - 1; I++)
			{
				for (int32 J = I + 1; J < BoardSize * BoardSize; J++)
				{
					if (GetCellValue(J) && GetCellValue(I) && GetCellValue(I) > GetCellValue(J))
					{
						InversionCount++;
					}
				}
			}
			return InversionCount;
		};

		auto FindXPosition = [&]() -> int32
		{
			for (int32 I = BoardSize - 1; I >= 0; I--)
			{
				for (int32 J = BoardSize - 1; J >= 0; J--)
				{
					if (ReadBoard(J, I) == 0)
						return BoardSize - I;
				}
			}
			assert(false && "Unable to find blank.");
		};

		int32 InversionCount = GetInversionCount();

		if (BoardSize % 2)
		{
			return !(InversionCount % 2);
		}
		else
		{
			int32 Pos = FindXPosition();
			if (Pos % 2)
			{
				return !(InversionCount % 2);
			}
			else
			{
				return InversionCount % 2;
			}
		}
	}

	void RandomBoard(int NumRandMoves)
	{
		random_device DeviceRandom;
		mt19937 Random(DeviceRandom());

		// Copy the winning game
		BoardData = WonGame;

		// This version always creates a solvable puzzle as it just randomly shifts tiles
		for (int Index = 0; Index < NumRandMoves; Index++)
		{
			int SwapDirection = Random() % 4;
			switch (SwapDirection)
			{
			case 0:// Up
			{
				if (!MoveSpace(EMoveDirection::Up)) Index--;
				break;
			}
			case 1:// Down
			{
				if (!MoveSpace(EMoveDirection::Down)) Index--;
				break;
			}
			case 2: // Left
			{
				if (!MoveSpace(EMoveDirection::Left)) Index--;
				break;
			}
			case 3: // Right
			{
				if (!MoveSpace(EMoveDirection::Right)) Index--;
				break;
			}
			}
		}
	}

	bool MoveSpace(EMoveDirection Direction)
	{
		int EmptySpaceX, EmptySpaceY;
		int NewX, NewY;
		if (!CalculateMovePosition(Direction, EmptySpaceX, EmptySpaceY, NewX, NewY))
		{
			return false;
		}

		WriteBoard(EmptySpaceX, EmptySpaceY, ReadBoard(NewX, NewY));
		WriteBoard(NewX, NewY, 0);
		return true;
	}

	bool CanMove(EMoveDirection Direction) const
	{
		int EmptySpaceX, EmptySpaceY;
		int NewX, NewY;

		return CalculateMovePosition(Direction, EmptySpaceX, EmptySpaceY, NewX, NewY);
	}

	bool IsGameWon() const
	{
		return BoardData == WonGame;
	}

	bool IsValid() const
	{
		return BoardData != 0;
	}

	vector<EMoveDirection> GetValidMoves() const
	{
		// ignore the inverse of the last move
		vector<EMoveDirection> Moves;
		if (CanMove(EMoveDirection::Up)) Moves.push_back(EMoveDirection::Up);
		if (CanMove(EMoveDirection::Down)) Moves.push_back(EMoveDirection::Down);
		if (CanMove(EMoveDirection::Left)) Moves.push_back(EMoveDirection::Left);
		if (CanMove(EMoveDirection::Right)) Moves.push_back(EMoveDirection::Right);
		return Moves;
	}

	void DrawBoard()
	{
		static const string BoardLetters = " ABCDEFGHIJKLMNO";

		cout << endl << endl;

		const auto DrawHorizontalDivider = []()
		{
			cout << "|";
			for (int Index = 0; Index < BoardSize; Index++)
			{
				if ((Index + 1) == BoardSize)
				{
					cout << "----";
				}
				else
				{
					cout << "-----";
				}
			}
			cout << "|" << endl;
		};

		DrawHorizontalDivider();

		for (int RowId = 0; RowId < BoardSize; RowId++)
		{
			cout << "|";

			for (int ColId = 0; ColId < BoardSize; ColId++)
			{
				cout << " " << setw(2) << BoardLetters[ReadBoard(ColId, RowId)] << " |";
			}

			cout << endl;

			DrawHorizontalDivider();
		}

		cout << endl;
	}

	bool operator==(const FifteenTilesBoard& Other) const
	{
		// Since the key is actually 100% unique to the board we can use that
		return GetKey() == Other.GetKey() && IsValid() && Other.IsValid();
	}
	bool operator!=(const FifteenTilesBoard& Other) const
	{
		// Since the key is actually 100% unique to the board we can use that
		return !(*this == Other);
	}
	friend struct hash<FifteenTilesBoard>;
};


uint64 CalculateWonGame()
{
	uint64 Board = 0;

	for (int32 Index = 0; Index < 16; Index++)
	{
		uint64 NewVal = Index < 15 ? Index + 1 : 0;

		uint64 NewValShifted = NewVal << ((15 - Index) * 4);
		Board |= NewValShifted;
	}
	return Board;
}

const uint8 FifteenTilesBoard::InvalidValue = 255;
const uint64 FifteenTilesBoard::WonGame = CalculateWonGame();

// Create the hash function for the board
namespace std {
	template <>
	struct hash<FifteenTilesBoard>
	{
		std::size_t operator()(const FifteenTilesBoard& Board) const
		{
			return hash<uint64>()(Board.GetKey());
		}
	};
}

struct FifteenTilesBoardDijkstra
{
	static vector<FifteenTilesBoard> Solve(const FifteenTilesBoard& Board)
	{
		// We're not using a priority queue here since every move has an equal weight
		queue<FifteenTilesBoard> Open;
		unordered_map<FifteenTilesBoard, FifteenTilesBoard> Predecessors;

		Open.push(Board);
		Predecessors[Board] = FifteenTilesBoard();

		int NodesProcessed = 0;
		cout << "Dijkstra Number Evaluated Nodes: " << "           0";
		while (!Open.empty())
		{
			NodesProcessed++;
			if (NodesProcessed % 10000 == 0)
			{
				cout << "\b\b\b\b\b\b\b\b\b\b\b\b" << setw(12) << NodesProcessed;
			}
			FifteenTilesBoard Candidate = Open.front(); Open.pop();

			if (Candidate.IsGameWon())
			{
				// Walk the backtrace and setup the list of moves to return
				vector<FifteenTilesBoard> ResultPath;
				FifteenTilesBoard Previous = Candidate;
				while (Previous.IsValid())
				{
					ResultPath.insert(ResultPath.begin(), Previous);
					if (Predecessors.find(Previous) == Predecessors.end())
					{
						break;
					}
					Previous = Predecessors[Previous];
				}
				return ResultPath;
			}
			for (FifteenTilesBoard::EMoveDirection Move : Candidate.GetValidMoves())
			{
				FifteenTilesBoard NewCandidate = Candidate;
				NewCandidate.MoveSpace(Move);
				if (Predecessors.find(NewCandidate) == Predecessors.end())
				{
					Predecessors[NewCandidate] = Candidate;
					Open.push(NewCandidate);
				}
			}
		}

		// Unable to find solution
		return vector<FifteenTilesBoard>();
	}
};