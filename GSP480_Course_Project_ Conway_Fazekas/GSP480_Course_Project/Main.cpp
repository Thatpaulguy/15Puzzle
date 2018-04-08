//GSP480 PFazekas and CConway
//Advanced AI 15 Puzzle Game
// Game employs several algorithms to try to solve classic 15 puzzle tile game


#include <ctime>
#include <random>
#include <iostream>
#include "Header.h"
#include "FifteenPuzzleDijkstra.h"



using namespace std;

// Creating instances of node and Astar class
node boardNode;
node tempNode;
node playNode;
aStar astar;




// main function
int main()
{
	srand(time(NULL));
	
	cout << endl << endl;

	cout << "	This simulation will use one of Five methods, " << endl                                              // intro
		<< "'Hill Climbing', 'Simulated Annealing', 'Dijkstra', 'A Star' or 'Totally Random' " << endl;

	moveRandom();                                                   // first we have to randomize the moving board 




	cout << endl << endl << "	Please choose one of the several methods " << endl
		<< " HILL CLIMBING (1) will look at a single move to see if it is" << endl
		<< "	closer to the goal but may get stuck." << endl
		<< " SIMULATED ANNEALING (2) will force Hill Climbing to make a move" << endl
		<< "	if it gets stuck, however it will stop after 100 assists" << endl
		<< " DIJKSTRA ALGORITHM (3) the most direct path (But the slowest) " << endl
		<< " A STAR ALGORITHM (4) to find the least cost path to solve the puzzle " << endl
		<< " RANDOM (0) will not care if it is closer to the goal, but may take a" << endl
		<< "	while, and the simulation will end in 5000 moves " << endl;
	cin >> userChoice;

	switch (userChoice)                                             //  Random, HillClimbing and SA will most likely not be seen because the algorithm they run on displays 
	{                                                               // each move and the statement will be lost, but Dijkstra and A* will disply easily.
	case 0:
		cout << " Random " << endl;
		break;
	case 1:
		cout << " Hill Climbing " << endl;
		break;
	case 2:
		cout << " Simulated Annealing " << endl;
		break;
	case 3:
		cout << " Dijkstra" << endl;
		break;
	case 4:
		cout << "A* " << endl << " This operation may take a while if there were a " << endl
			<< "large number of random moves to scramble the board " << endl;
		break;
	default:
		cout << " A*  has been chosen for you " << endl;           // in the case of miss tyoing or laziness A* is selected.
		userChoice = 4;
		break;

	}



	if (userChoice == 3)
	{
		// Run the entire dijkstra search

		// Copy the board to the dijkstra search structure
		FifteenTilesBoard NewBoard;
		NewBoard.RandomBoard(/* Number of random moves */50);
		NewBoard.DrawBoard();
		bool bWasBoardSolvable = NewBoard.IsSolvable();
		cout << "Board is solvable: " << bWasBoardSolvable << endl;

		auto Path = FifteenTilesBoardDijkstra::Solve(NewBoard);

		if (Path.size() > 0)
		{
			cout << endl << endl << "Dijkstra solution... Found in " << Path.size() - 1 << " moves!" << endl;
			for (int Index = 0; Index < Path.size(); Index++)
			{
				Path[Index].DrawBoard();
			}
		}
		else
		{
			cout << endl << endl << "Dijkstra unable to solve board." << endl;
			if (bWasBoardSolvable)
			{
				cout << "    Board was solvable..." << endl;
			}
		}

		system("PAUSE");
	}
	else
	{
		do                                                  // do-while loop to run the original game sequence
		{
			cout << endl << endl << endl << endl;
			
			currentMH = manHattan();              			// transfers value to check in Manhattan after each move

			
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					boardNode.current[i][j] = gameBoard[i][j];
				}
			}

			// parts of A* that need to run in original loop 
			boardNode.Hscore = manHattan2();                                                  
			tempNode.Fscore = tempNode.Hscore + tempNode.Gscore;
			astar.openlist.push_back(boardNode);
			
			// excludes A* as needing a constant update of stats from original program
			if (userChoice != 4)
			{
				for (int i = 0; i < 4; i++)                                                  // prints gameboard
				{
					for (int j = 0; j < 4; j++)
						cout << "	" << gameBoard[i][j];
					cout << endl << endl;
				}
				cout << " Manhattan Heuristic is currently " << currentMH << endl;
				cout << " Current number of moves is " << winCount << endl;
				if (userChoice == 2)
				{
					cout << " Current assists by the Annealing is " << annealCount << endl;
				}
				cout << " The space is now in row " << vertSpace + 1 << ", column " << horSpace + 1 << endl << endl << endl; // identifies position of blank space
			}


			//call to A* Algorithm 
			if (userChoice == 4)
			{

				astar.runAstar();
				game = false;
				for each (auto& board in astar.path)
				{
					astar.display();
					cout << " A* solved the puzzle in " << winCount << " moves " << endl;
					cout << endl << endl;
					game = false;
					break;

				}
			}


			//original game loop 
			if (mCup > 0 && mCdown > 0 && mCleft > 0 && mCright > 0)
			{
				if (userChoice == 1)                                                                         // Stops the sim if Hill Climbing plateaus
				{
					cout << " Hill Climbing got stuck at " << winCount << " moves " << endl;
					game = false;
				}
				else if (userChoice == 2 && annealCount == 100)                                             // stops the sim at 100 assists by the Annealing
				{
					cout << " Simulated Annealing got stuck at " << winCount << " moves " << endl;
					game = false;
				}
				else if (userChoice == 2)
				{
					mCup = 0; mCdown = 0; mCleft = 0; mCright = 0;                                           // resets counter from Hill Climbing
					sAnneal = sAnneal + 1;                                                                   // used to count when to help Hill Climbing 
					annealCount = annealCount + 1;                                                           // keeps up with number of times Anneal helped to end program

				}

			}
			else if (userChoice == 0)
			{
				if (winCount > 4999)
				{
					cout << " Random did not solve the puzzle within 5000 moves " << endl;
					game = false;
				}
			}



			moveTile = rand() % 4 + 1;                                                                          // randomly chooses direction
			winCount = winCount + 1;                                                                            // running count of moves made

			if (userChoice == 0 || (userChoice == 2 && sAnneal > 0))                                            // checks if user chose random OR annealing needs to help Hill Climbing
			{
				sAnneal = 0;                                                                                    // resets SA count 
				switch (moveTile)                                                                               // switch to take input of user and assign function
				{
				case 1:
					moveUp();
					break;
				case 2:
					moveLeft();
					break;
				case 3:
					moveDown();
					break;
				case 4:
					moveRight();
					break;
				case 'q':
					cout << " You have quit the game " << endl;                                                   // quitters never win, will still work to exit 
					game = false;
					break;
				default:
					cout << " Not a valid selection, please try again. " << endl;                                 //  flag for invalid input 
					break;

				}
			}
			else if (userChoice > 0 && userChoice < 4)                                                             // Tries a move before chosing one to be the actyal move in HC and SA
			{
				switch (moveTile)                                                                                  // switch to take input of user and assign function
				{
				case 1:
					moveUp1();
					break;
				case 2:
					moveLeft1();
					break;
				case 3:
					moveDown1();
					break;
				case 4:
					moveRight1();
					break;
				case 'q':
					cout << " You have quit the game " << endl;                                                   // quitters never win, will still work to exit 
					game = false;
					break;
				default:
					cout << " Not a valid selection, please try again. " << endl;                                 //  flag for invalid input 
					break;

				}
			}

			if (checkWin() == true && userChoice < 4)                                                        //  calls check win function in original game
			{
				for (int i = 0; i < 4; i++)                                                                  // loop to print winning use board
				{
					for (int j = 0; j < 4; j++)
						cout << "	" << gameBoard[i][j];
					cout << endl << endl;
				}
				
				cout << " The puzzle was solved in " << winCount << " move(s)!" << endl << endl;

				game = false;                                                                                 // sets to false ending the game

			}


		} while (game == true);                                                                                // will run game sequence until changed to false by any of the ending choices

	}



	return 0;

}





// functions



void moveRandom()     // mixes up gameBoard
{
	cout << "	Scrambling the board a random number of times from 10 to 60" << endl;      // asks for user input to randomize puzzle 
																							  

	if (scramble == 0)   //and it will
	{
		scramble = rand() % 50 + 10;                                                       // randomly assigns the scramble variable between 10 and 60 times
		cout << " The board has been scrambled " << scramble << " times " << endl << endl;
	}

	
	for (int i = 0; i < scramble; i++)                                                    // switch statement to mix up board 
	{
		int ranScram = rand() % 4 + 1;
		
		switch (ranScram)
		{
		case 1:
			moveUp();
			break;
		case 2:
			moveLeft();
			break;
		case 3:
			moveDown();
			break;
		case 4:
			moveRight();
			break;
		}
	}

	for (int i = 0; i < 4; i++)                                                  // prints gameboard
	{
		for (int j = 0; j < 4; j++)
			cout << "	" << gameBoard[i][j];
		cout << endl << endl;

	}

		
}




// Set of movements used in the random selection, and if the Simulated Annealing need to make a random move to help Hill Climbing

void moveUp()
{

	int v = vertSpace;                    // transferred verticle position to marker insuring move was not outside game board
	if (v + 1 < 4 && v >= 0)              // this line drove me up a wall for over an hour, but I based the other functions on it. 
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace + 1][horSpace];     // changed position of where the space is kept
		gameBoard[vertSpace + 1][horSpace] = ' ';                                // assigned the character [space] into new position
		vertSpace = vertSpace + 1;                                                // updated position of the space for next run
	}

}

void moveDown()
{

	int v = vertSpace;                   // transferred verticle position to marker insuring move was not outside game board
	if (v + 1 <= 4 && v > 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace - 1][horSpace];     // changed position of where the space is kept
		gameBoard[vertSpace - 1][horSpace] = ' ';                                // assigned the character [space] into new position
		vertSpace = vertSpace - 1;                                               // updated position of the space for next run
	}

}

void moveLeft()
{

	int h = horSpace;                  // transferred horizontal position to marker insuring move was not outside game board
	if (h + 1  < 4 && h >= 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace][horSpace + 1];     // changed position of where the space is kept
		gameBoard[vertSpace][horSpace + 1] = ' ';                                // assigned the character [space] into new position
		horSpace = horSpace + 1;                                                 // updated position of the space for next run             
	}

}

void moveRight()
{

	int h = horSpace;                   // transferred horizontal position to marker insuring move was not outside game board
	if (h + 1 <= 4 && h > 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace][horSpace - 1];      // changed position of where the space is kept
		gameBoard[vertSpace][horSpace - 1] = ' ';                                 // assigned the character [space] into new position
		horSpace = horSpace - 1;                                                  // updated position of the space for next run
	}

}

// alternate set of moves to evaluate difference in Manhattan distance - these run while the hillclimbing is running and while the Sim Annealing is not assisting

void moveUp1()
{

	if (mCup > 0)
		return;
	mCup = +1;


	int v = vertSpace;                  // transferred verticle position to marker insuring move was not outside game board
	if (v + 1 < 4 && v >= 0)              // this line drove me up a wall for over an hour, but I based the other functions on it. 
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace + 1][horSpace];     // changed position of where the space is kept
		gameBoard[vertSpace + 1][horSpace] = ' ';                                // assigned the character [space] into new position
		vertSpace = vertSpace + 1;                                                // updated position of the space for next run
		if (manHattan() > currentMH)
		{
			moveDown();                                                          // uses uncounted move
		}
		else mCup = 0;
	}

}

void moveDown1()
{
	if (mCdown > 0)
		return;
	mCdown = +1;

	int v = vertSpace;                  // transferred verticle position to marker insuring move was not outside game board
	if (v + 1 <= 4 && v > 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace - 1][horSpace];     // changed position of where the space is kept
		gameBoard[vertSpace - 1][horSpace] = ' ';                                // assigned the character [space] into new position
		vertSpace = vertSpace - 1;                                               // updated position of the space for next run
		if (manHattan() > currentMH)
		{
			moveUp();
		}
		else mCdown = 0;
	}

}

void moveLeft1()
{

	if (mCleft > 0)
		return;
	mCleft = +1;

	int h = horSpace;                    // transferred horizontal position to marker insuring move was not outside game board
	if (h + 1  < 4 && h >= 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace][horSpace + 1];     // changed position of where the space is kept
		gameBoard[vertSpace][horSpace + 1] = ' ';                                // assigned the character [space] into new position
		horSpace = horSpace + 1;                                                 // updated position of the space for next run             
		if (manHattan() > currentMH)
		{
			moveRight();
		}
		else mCleft = 0;
	}
	
}

void moveRight1()
{
	if (mCright > 0)
		return;
	mCright = +1;

	int h = horSpace;                   // transferred horizontal position to marker insuring move was not outside game board
	if (h + 1 <= 4 && h > 0)
	{
		gameBoard[vertSpace][horSpace] = gameBoard[vertSpace][horSpace - 1];      // changed position of where the space is kept
		gameBoard[vertSpace][horSpace - 1] = ' ';                                 // assigned the character [space] into new position
		horSpace = horSpace - 1;                                                  // updated position of the space for next run
		if (manHattan() > currentMH)
		{
			moveLeft();
		}
		else mCright = 0;
	}
}

// function to check winning scenario in original program
bool checkWin()
{
	for (int i = 0; i<4; i++)                                                 // runs through loop to compare elements of arrays
		for (int j = 0; j <4; j++)
		{
			if (gameBoard[i][j] == gameWin[i][j])
			{
			}                                                                // need as a space holder to be able to compare and return 
			else return false;
		}
	return true;

}

//One of two manhattan loops - this one is for the original loop, may have been able to also use for other loop, but considered taking all of the old program out
int manHattan()
{
	int manH = 0;                                                               // runs through entire set and adds the manhattan distance of each 'tile'


	for (int i = 0; i < 4; i++)                                                 // runs through loop to compare elements of arrays
	{
		for (int j = 0; j < 4; j++)
		{
			if (gameBoard[i][j] == 'A')
			{
				manH = manH + (abs(i - 0) + abs(j - 0));
			}
			else if (gameBoard[i][j] == 'B')
			{
				manH = manH + (abs(i - 0) + abs(j - 1));
			}
			else if (gameBoard[i][j] == 'C')
			{
				manH = manH + (abs(i - 0) + abs(j - 2));
			}
			else if (gameBoard[i][j] == 'D')
			{
				manH = manH + (abs(i - 0) + abs(j - 3));
			}
			else if (gameBoard[i][j] == 'E')
			{
				manH = manH + (abs(i - 1) + abs(j - 0));
			}
			else if (gameBoard[i][j] == 'F')
			{
				manH = manH + (abs(i - 1) + abs(j - 1));
			}
			else if (gameBoard[i][j] == 'G')
			{
				manH = manH + (abs(i - 1) + abs(j - 2));
			}
			else if (gameBoard[i][j] == 'H')
			{
				manH = manH + (abs(i - 1) + abs(j - 3));
			}
			else if (gameBoard[i][j] == 'I')
			{
				manH = manH + (abs(i - 2) + abs(j - 0));
			}
			else if (gameBoard[i][j] == 'J')
			{
				manH = manH + (abs(i - 2) + abs(j - 1));
			}
			else if (gameBoard[i][j] == 'K')
			{
				manH = manH + (abs(i - 2) + abs(j - 2));
			}
			else if (gameBoard[i][j] == 'L')
			{
				manH = manH + (abs(i - 2) + abs(j - 3));
			}
			else if (gameBoard[i][j] == 'M')
			{
				manH = manH + (abs(i - 3) + abs(j - 0));
			}
			else if (gameBoard[i][j] == 'N')
			{
				manH = manH + (abs(i - 3) + abs(j - 1));
			}
			else if (gameBoard[i][j] == 'O')
			{
				manH = manH + (abs(i - 3) + abs(j - 2));
			}
			else if (gameBoard[i][j] == ' ')
			{
				manH = manH + (abs(i - 3) + abs(j - 3));
			}

		}
		
	}
	
	return manH;
}


// second Manhattan algorithm for use in A* 

int manHattan2()
{
	int manH2 = 0;                                                               // runs through entire set and adds the manhattan distance of each 'tile'


	for (int i = 0; i < 4; i++)                                                 // runs through loop to compare elements of arrays
	{
		for (int j = 0; j < 4; j++)
		{
			if (tempNode.current[i][j] == 'A')
			{
				manH2 = manH2 + (abs(i - 0) + abs(j - 0));
			}
			else if (tempNode.current[i][j] == 'B')
			{
				manH2 = manH2 + (abs(i - 0) + abs(j - 1));
			}
			else if (tempNode.current[i][j] == 'C')
			{
				manH2 = manH2 + (abs(i - 0) + abs(j - 2));
			}
			else if (tempNode.current[i][j] == 'D')
			{
				manH2 = manH2 + (abs(i - 0) + abs(j - 3));
			}
			else if (tempNode.current[i][j] == 'E')
			{
				manH2 = manH2 + (abs(i - 1) + abs(j - 0));
			}
			else if (tempNode.current[i][j] == 'F')
			{
				manH2 = manH2 + (abs(i - 1) + abs(j - 1));
			}
			else if (tempNode.current[i][j] == 'G')
			{
				manH2 = manH2 + (abs(i - 1) + abs(j - 2));
			}
			else if (tempNode.current[i][j] == 'H')
			{
				manH2 = manH2 + (abs(i - 1) + abs(j - 3));
			}
			else if (tempNode.current[i][j] == 'I')
			{
				manH2 = manH2 + (abs(i - 2) + abs(j - 0));
			}
			else if (tempNode.current[i][j] == 'J')
			{
				manH2 = manH2 + (abs(i - 2) + abs(j - 1));
			}
			else if (tempNode.current[i][j] == 'K')
			{
				manH2 = manH2 + (abs(i - 2) + abs(j - 2));
			}
			else if (tempNode.current[i][j] == 'L')
			{
				manH2 = manH2 + (abs(i - 2) + abs(j - 3));
			}
			else if (tempNode.current[i][j] == 'M')
			{
				manH2 = manH2 + (abs(i - 3) + abs(j - 0));
			}
			else if (tempNode.current[i][j] == 'N')
			{
				manH2 = manH2 + (abs(i - 3) + abs(j - 1));
			}
			else if (tempNode.current[i][j] == 'O')
			{
				manH2 = manH2 + (abs(i - 3) + abs(j - 2));
			}
			else if (tempNode.current[i][j] == ' ')
			{
				manH2 = manH2 + (abs(i - 3) + abs(j - 3));
			}

		}

	}

	return manH2;
}


// All of the following functions are exclusive to the A* Algorithm 
aStar::aStar()                           // defined in case I needed to deploy
{
}


aStar::~aStar()                         // also included in case needed
{
}



//Modified move functions from original game to 'try' each move and push it into Node for use later with openlist and closed list
void aStar::tryMove()
{

	tempNode = playNode;                 // sets a copy of Node to the existing board being looked at so we alter the copy and not the actual board   
	findSpace(vertSpace, horSpace);      // calls the find space function because although a new board may be passed the reference of position of the space is still in its original place


	int v = vertSpace;                   // transferred vertical position to marker insuring move was not outside game board
	int h = horSpace;                    // transferred horizontal position to marker insuring move was not outside game board

	
	if (v + 1 < 4 && v >= 0)                         // moves something into the space up, space moves down
	{
		tempNode.current[vertSpace][horSpace] = tempNode.current[vertSpace + 1][horSpace];     // changed position of where the space is kept
		tempNode.current[vertSpace + 1][horSpace] = ' ';                                      // assigned the character [space] into new position

		                                                                               // ALL OF THESE ARE REPEATED IN EACH TRY MOVE
		tempNode.Hscore = manHattan2();                                                       // calls second Manhattan and copies info into Node
		tempNode.Gscore += 1;                                                                 // all moves have equal penalty for the move 
		tempNode.Fscore = tempNode.Hscore + tempNode.Gscore;                                  // compute F-Score for move try 
		trymove.push_back(tempNode);                                                          //embeds the current try onto Node
		tempNode = playNode;                                                                  // resets the copy of Node back to the current actual gameboard to try the next move


	}

	if (v + 1 <= 4 && v > 0)                           // moves something into the space down, space moves up
	{
		tempNode.current[vertSpace][horSpace] = tempNode.current[vertSpace - 1][horSpace];     // changed position of where the space is kept
		tempNode.current[vertSpace - 1][horSpace] = ' ';                                // assigned the character [space] into new position

		tempNode.Hscore = manHattan2();
		tempNode.Gscore += 1;
		tempNode.Fscore = tempNode.Hscore + tempNode.Gscore;
		trymove.push_back(tempNode);
		tempNode = playNode;

	}

	if (h + 1 < 4 && h >= 0)
	{
		tempNode.current[vertSpace][horSpace] = tempNode.current[vertSpace][horSpace + 1];     // changed position of where the space is kept
		tempNode.current[vertSpace][horSpace + 1] = ' ';                                // assigned the character [space] into new position

		tempNode.Hscore = manHattan2();
		tempNode.Gscore += 1;
		tempNode.Fscore = tempNode.Hscore + tempNode.Gscore;
		trymove.push_back(tempNode);
		tempNode = playNode;


	}

	if (h + 1 <= 4 && h > 0)
	{
		tempNode.current[vertSpace][horSpace] = tempNode.current[vertSpace][horSpace - 1];      // changed position of where the space is kept
		tempNode.current[vertSpace][horSpace - 1] = ' ';                                 // assigned the character [space] into new position

		tempNode.Hscore = manHattan2();
		tempNode.Gscore += 1;
		tempNode.Fscore = tempNode.Hscore + tempNode.Gscore;
		trymove.push_back(tempNode);
		tempNode = playNode;



	}




}


// Actual A* algorithm including while loop - as long as there is a board in the open list the loop will continue

void aStar::runAstar()
{


	cout << " The number of Moves the program is running is: " << endl;            // added so user will see progress being made on an extremely scrambled board    


	while (!openlist.empty() && game == true)
	{
		
		openlist.sort(compare_Fscore);												// sort function to take lowest F value board.
		playNode = openlist.front();                                                // updates -playNode to the 

		openlist.pop_front();                                                       // pops the board off of theopen list and places it on to the closed list
		closedlist.push_back(playNode);

		if (checkwinner() == true)
		{
			path.push_back(playNode);
			break;

		}



		// uses check winner function to run loop seeing if we have a match and the puzzle is solved. 
		tryMove();                                                                  // checks each of the possible moves. 


		for each (auto& moveboard in trymove)                                        // iterative for each loops to check each board open and closed lists for matches 
		{

			for each (auto& board in closedlist)
			{
				if (compareboards(moveboard, board) == true)
				{
					break;
				}

			}
			for each (auto& board in openlist)
			{
				if (compareboards(moveboard, board) == true)
				{
					break;
				}


			}
			openlist.push_back(moveboard);                                         // if it isn't on either list drop it into the open list to be sorted next time through  
		}
		path.push_back(playNode);                                                  // throws the best completely looked at board onto the path to solve puzzle
		winCount += 1;                                                             // simple count function 
		cout << winCount << " \b\b\b\b\b\b\b\b\b\b\b\b";                           // prints current number of moves to console

		trymove.clear();                                                           // clears the counter fro number of boards held in the list of trying moves. 
		
	}
	
}


// displays path when puzzle is solved
void aStar::display()
{
	for each (auto& board in path)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				cout << "	" << board.current[i][j];
			cout << endl << endl;

		}
		cout << endl << endl << endl << endl;
	}
	
}



bool aStar::compare_Fscore(const node& first, const node& second)               // compare openlist to bring lowest F cost to front
{
	return (first.Fscore < second.Fscore);
}



bool aStar::checkwinner()                                                       // function to check the winning solution against current board. 
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (playNode.current[i][j] == gameWin[i][j])
			{
			}
			else return false;
		}
	return true;

}




// compare function looking at open and closed lists passed to it
bool aStar::compareboards(const node &a, const node &b)
{

	for (int i = 0; i < 4; i++)                                                  // prints gameboard
		for (int j = 0; j < 4; j++)
		{
			if (a.current[i][j] == b.current[i][j])
			{
			}
			else return false;
		}
	return true;
	
}


// function that runs through current gameboard looking for the placement of the space character
int aStar::findSpace(int& v, int& h)
{

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tempNode.current[i][j] == ' ')
			{
				v = i;
				h = j;
			}
		}
		
	}
	return v, h;
}