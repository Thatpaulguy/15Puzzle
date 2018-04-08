//Created by Paul Fazekas and Chris Conway 
// for GSP 480 course project 


#pragma once
#include <list>





// variables
bool game = true;
bool win = false;
int  moveTile;
int vertSpace = 3; // Verticle position of empty space
int horSpace = 3;  // Horizontal position of empty space
int scramble;
int winCount;
int currentMH;     // manhattan count to compare
int mCup = 0;      // manhattan variable to use in comparison manHattan counting. 
int mCdown = 0;    // manhattan variable to use in comparison manHattan counting.      
int mCleft = 0;    // manhattan variable to use in comparison manHattan counting.
int mCright = 0;   // manhattan variable to use in comparison manHattan counting.
int userChoice;    // variable for user choice
int sAnneal = 0;   // variables used for Anneal counting
int annealCount = 0;
int manH = 0;
int manH2 = 0;

// functions
void moveUp();                       // set function to "move" tile up
void moveDown();                     // function to "move" tile down
void moveLeft();                     // function to "move" tile left
void moveRight();                    // function to "move tile right
void moveUp1();                       // set function to "move" tile up  and check mahattan
void moveDown1();                     // function to "move" tile down and check manhattan
void moveLeft1();                     // function to "move" tile left and check manhattan
void moveRight1();                    // function to "move tile right and check manhattan
void moveRandom();                   // set to randomize gameBoard
bool checkWin();
int manHattan();                     // function to compare tile to position and return Manhattan Distance for our heuristic
int manHattan2();


// gameboards
char gameBoard[4][4] = { { 'A', 'B', 'C', 'D' },{ 'E', 'F', 'G', 'H' },{ 'I', 'J', 'K', 'L' },{ 'M', 'N', 'O', ' ' } };   // changing game board
char gameWin[4][4] = { { 'A', 'B', 'C', 'D' },{ 'E', 'F', 'G', 'H' },{ 'I', 'J', 'K', 'L' },{ 'M', 'N', 'O', ' ' } };     // winning solution that we will compare to moving board.


// node struct to be used to make gameboards and hold values 
struct node
{
public:
	char current[4][4];                   //current array 
	int Hscore = manH;                    // initial Manhattan set to first Manhattan algorithm   
	int Gscore = 0;                       // G Score set and initialized to 0
	int Fscore = Gscore + Hscore;         // F Score set and initialized 
	int tempFscore = 0;
	
};


//  A* class including lists and prototypes for running A* Algorithm in the Main.cpp
class aStar
{
public:
	//list of lists
	std::list<node> openlist;
	std::list<node> closedlist;
	std::list<node> trymove;
	std::list<node> path;
	
	//prototypes
	aStar();                              // constructor
	~aStar();                             // destructor
	void tryMove();                       // prototype to try each possible move before sending it to the appropriate list
	void runAstar();                      // prototype for holding the while loop 
	void display();                       // displays all boards in the path when a solution is attained
	bool checkwinner();                   // checks to see if each current board is a match to the winning board
	static bool compareboards(const node &a, const node &b);                // used to compare against open and closd lists
	static bool compare_Fscore(const node& first, const node& second);      // used for the sort function	
	int findSpace(int&, int&);            // prototype for find space function 
	
};







