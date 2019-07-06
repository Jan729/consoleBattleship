#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>

#define BOARDSIZE 8 //change this number to make the gameboard bigger or smaller
#define BLUE 1
#define GOLD 6
#define GREEN 2
#define GREY 8
#define RED 4
#define WHITE 15
#define MAX_LENGTH 2048

//Decription: This is a 2 player battleship game for the console

void printWelcome();
void printInstructions();
void resetBoard(int board[][BOARDSIZE]);
void eraseInput();
void printColLabels();
void dispBoard(int board[][BOARDSIZE], int guessBoard[][BOARDSIZE], int player);
void hideBoard();
void placeUserShips(int myBoard[][BOARDSIZE], int guessBoard[][BOARDSIZE], int player);
int isValidPlacement(int myBoard[][BOARDSIZE], int row, int col, int boatSize, char alignment, int *rowStep, int *colStep);
void getMove(int move[], int board[][BOARDSIZE]);
void makeMove(int player, int move[], int OppBoard[][BOARDSIZE], int myBoard[][BOARDSIZE], int guessBoard[][BOARDSIZE]);
bool wonGame(int oppBoard[][BOARDSIZE],int player);
void SetColor(int ForgC);

int main(void)
{
    int board1[BOARDSIZE][BOARDSIZE];
	int guessBoard[BOARDSIZE][BOARDSIZE];
    int board2[BOARDSIZE][BOARDSIZE];
	int guessBoard2[BOARDSIZE][BOARDSIZE];
    int player = 1;
    int move[2]; //stores coordinates of move
   
    resetBoard(board1);
	resetBoard(guessBoard);
    resetBoard(board2);
	resetBoard(guessBoard2);
    
    printWelcome();
	
	placeUserShips(board1, guessBoard, 1);
    hideBoard();
	placeUserShips(board2, guessBoard2, 2);
    hideBoard();
   
    printf("\nIt's player 1's turn!\n\n");
    
    while (true) {
		if(player == 1) {
			dispBoard(board1, guessBoard, player); //start out as dashes
            getMove(move, guessBoard);
            makeMove(player, move, board2, board1, guessBoard);
        } else if(player == 2) {
			dispBoard(board2, guessBoard2, player);
            getMove(move, guessBoard2);
            makeMove(player, move, board1, board2, guessBoard2);
		}	
        
		//check for wins
		if(wonGame(board1, player)) {
			printf("\nPLAYER 2 WINS! :D\n");
			return 0;
		}
        if(wonGame(board2, player)) {
			printf("\nPLAYER 1 WINS! :D\n");
			return 0;
		}
        
        hideBoard();
		//switch to next player
		if(player == 1) {
			player = 2;
			printf("\nIt's player 2's turn!\n");
		} else {
			player = 1;
			printf("\nIt's player 1's turn!\n");
		}
	}
}

void printWelcome() {
    
    printf("Welcome to Console Battleship!\n\nEnter ");
    SetColor(GOLD);
    printf("? ");
    SetColor(WHITE);
    printf("to learn how to play, or ");
    SetColor(GOLD);
    printf("press ENTER ");
    SetColor(WHITE);
    printf("to start the game: ");
    
    char startInput = getchar();
    if(startInput == '?') {
        printInstructions();
    } else if (startInput != '\n'){
        eraseInput();
    }
}

void printInstructions() {
	
    printf("HOW TO PLAY\n\n"
	"Aim of the game: sink all of your opponent's ships before they sink yours!\n\n"
	"\nYou have 5 ships:"
	 "\n1) Carrier (5 units long)\n2) Battleship (4 units long)\n3) Cruiser (3 units long)\n4) Submarine (3 units long)"
	 "\n5) Destroyer (2 units long)\n\n");
    printf("To place a battleship on the gameboard, follow the prompt to place each battleship.\nEnter the column (ex. A), row (ex. 0)."
	"Then you can either enter 'h' to align your ship from\nleft to right, or 'v' to align your ship up and down.\nFor example, 'A0h' puts your battleship"
	" in the upper left corner along row 0.\n\n");
    
    int test = 0;
    int example[BOARDSIZE][BOARDSIZE];
    int exampleGuess[BOARDSIZE][BOARDSIZE];
    resetBoard(example);
    //place a 4 unit ship at A0h
    for(int i = 0; i < 4; i++) { 
        example[0][i] = 1;
    }
    dispBoard(example, exampleGuess, test);
    
	printf("After each player places their ships, take turns entering a coordinate to shoot a missile at your opponent.\n"
	"See the legend below for an explanation of the gameboard symbols.\n\n");
	
    printf("Game Board Legend:\n");
	SetColor(GREY);
	printf("X");
	SetColor(WHITE);
	printf(" - Your Ship\n");
	SetColor(RED);
	printf("X");
	SetColor(WHITE);
	printf(" - Your Damaged/Sunken Ship\n");
	SetColor(GREEN);
	printf("O");
	SetColor(WHITE);
	printf(" - Location of opponent's damaged ship\n");
	printf("O");
	SetColor(WHITE);
	printf(" - Missed Shot\n");
	SetColor(BLUE);
	printf("Anything blue");
	SetColor(WHITE);
	printf(" - Water\n");
    
}
//set all items in board array to 0
void resetBoard(int board[][BOARDSIZE]) {
    for(int row = 0; row < BOARDSIZE; row++) {
     for(int col = 0; col < BOARDSIZE; col++) {
         board[row][col] = 0;
     }
     }
}

//erase any extra input that hasn't been scanned
void eraseInput() {
    while(getchar() != '\n') {
    }
}

//print alphabetical labels at top of board
void printColLabels() {
	char colLabel[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
	printf("   ");
	for(int i = 0; i < BOARDSIZE; i++) {
	printf("%c ", colLabel[i]);
	}
	printf("\n");
}

//print player's board with ships, and board with previous shots
void dispBoard(int board[][BOARDSIZE], int guessBoard[][BOARDSIZE], int player) {
    SetColor(15); //start with white text colour
	if(player == 1 || player == 2) {
		printf("Player %d's ocean:\n\n", player);
	}
	//display current player's ocean, including floating ships and sunken ships
    //1 = ship
    //2 = damaged ship
    //3 = hit
    //4 = miss
	printColLabels();
    for(int row = 0; row < BOARDSIZE; row++) {
		printf("%d  ", row);
        for(int col = 0; col < BOARDSIZE; col++) {
			if (board[row][col] == 1) {
				SetColor(8);
				printf("X ");
			} else if (board[row][col] == 2) {
				SetColor(4);
				printf("X ");
			} else {
				SetColor(1);
                printf("w ");
			}
			SetColor(255);
        }
		printf("\n");
	}
	
    //function returns now if player 1 or player 2 is NOT making a move
    //(like when you're placing boats)
	if(player != 1 && player != 2) {
		return;
	}
	//display player's previous shots at opponent
	printf("\nYour previous missile launches:\n\n");
	printColLabels();

	for(int row = 0; row < BOARDSIZE; row++) {
		printf("%d  ", row);
		for(int col = 0; col < BOARDSIZE; col++) {
		   if (guessBoard[row][col] == 3) {
				SetColor(GREEN);
				printf("O ");
			} else if (guessBoard[row][col] == 4) {
				SetColor(WHITE);
				printf("O ");
			} else {
				SetColor(BLUE);
				printf("- ");
			}
			SetColor(WHITE);
		}
		printf("\n");
	}
}	

//clears console output to hide your board
void hideBoard() {
    
    printf("\nPress ENTER to hide your board, then pass computer to next player: ");
    eraseInput(); //erase input and wait for user to enter '\n'
	system ("cls");
    
	printf("\nNEXT PLAYER: Press ENTER to continue. (Make sure previous player isn't looking): \n");
    eraseInput();
    system ("cls");
}

//Prompt user to place ships in valid positions
void placeUserShips(int myBoard[][BOARDSIZE], int guessBoard[][BOARDSIZE], int player) {
	int shipSize[] = {5, 4, 3, 3, 2};
	char * shipNames[] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
	
	printf("\nPlayer %d's turn! (Make sure your opponent can't see the screen)\n\n", player);
	
	dispBoard(myBoard, guessBoard, player);
	
	for(int i = 0; i < 5; i++) {
		bool validInput = false;
		int row = 0;
		char col = 0, alignment;
		
		while(!validInput) {
            char userInput[MAX_LENGTH]; //Can I make this better?
            for(int i = 0; i < MAX_LENGTH; i++) {
                userInput[i] = ' ';
            }
			printf("Enter a co-ordinate (ex A0v) to place your ");
			SetColor(GOLD);
			printf("%s (%d units long) ", shipNames[i], shipSize[i]);
			SetColor(WHITE);
			printf(": ");
			SetColor(GOLD);
			scanf(" %s", userInput);
            
            //it only assigns the input values to row, col, align if the
            //input str is 3 chars long
            if(strlen(userInput) != 3) {
                row = -1;
                col = -1;
                alignment = -1;
            } else {
                col = userInput[0];
                row = userInput[1] - 48;
                alignment = userInput[2];
                //printf("col: %c, row = %d, align = %c", col, row, alignment);
            }
            SetColor(WHITE);
			
			//remove case sensitivity
			if(col >= 'a' && col <= 'z') {
				col -= 32;
			}
			if(alignment >= 'A' && alignment <= 'Z') {
				alignment += 32;
			} 
			
			col -= 65;
			
			//check for valid boat placement
			int rowStep = 0, colStep = 0;
			int validPlacement = isValidPlacement(myBoard, row, col, shipSize[i], alignment, &rowStep, &colStep);
			if(validPlacement == 1) {
				validInput = true;
				int tempRow = row, tempCol = col;
				for(int j = 0; j < shipSize[i]; j++) {
					myBoard[tempRow][(int)tempCol] = 1;
					tempRow += rowStep;
					tempCol += colStep;
				}
				SetColor(GOLD);
				printf("Placed %s at %c%d.\n", shipNames[i], col + 65, row); //confirm boat placement
				dispBoard(myBoard, guessBoard, 4);
			} 
            
            getchar(); //erase remaining '\n' that hasn't been scanned
		}
	}
}

//checks to see if coordinate places ship in an empty spot on the ocean
//returns 1 if it's a valid placement
//returns 2 if there's an overlap with another ship or partially out of bounds
//returns 0 for all other invalid cases
int isValidPlacement(int myBoard[][BOARDSIZE], int row, int col, int boatSize, char align, int *rowStep, int *colStep) {
	
    if((row < 0 || row > BOARDSIZE - 1) || (col < 0 || col > BOARDSIZE - 1) || (align != 'h' && align != 'v')) {
        SetColor(RED);
        printf("Invalid coordinate. Please try again.\n");
        SetColor(WHITE); 
        return 0;
    }
    
	//at this point, row, col, and align should be valid inputs

	if(align == 'h') {
		*colStep = 1;
	} else {
		*rowStep = 1; //align should only be 'v' at this point
	}
	//loop thru all squares of potential ship placement and check to make sure the ocean's empty there
	for(int i = 0; i < boatSize; i++) {
		if(row >= BOARDSIZE || col >= BOARDSIZE) {
			SetColor(RED);
            printf("Part of the ship is out of bounds. Please try again.\n");
            SetColor(WHITE);
            return 0;
		}
        
        if(myBoard[row][col] != 0) {
            SetColor(RED);
            printf("The ship placement overlaps with another ship. Please try again.\n");
            SetColor(WHITE);
            return 0;
        }
		row += *rowStep;
		col += *colStep;
	}
	
	return 1;
}

//accept user input, check if valid
void getMove(int move[], int guessBoard[][BOARDSIZE]) {

	bool validInput = false;
	int row, col;
	
	while (!validInput) {
		printf("Enter a coordinate (ex. A0) to launch a missile: ");

		int newRow;
		char newCol;
		scanf(" %c%d", &newCol, &newRow);
        
        eraseInput(); //in case there's extra input

		if((newCol < 'A' || newCol > 'J') || (newRow < 0 || newRow > BOARDSIZE - 1)){
            SetColor(RED);
			printf("Invalid coordinate. Please try again.\n");
            SetColor(WHITE);
		} else {
			newCol -= 65;
			col = (int)newCol;
			row = newRow;
			
			if(guessBoard[row][col] != 0) {
                SetColor(RED);
				printf("You've already shot a missile there. Choose another co-ordinate!\n");
                SetColor(WHITE);
			} else {
				validInput = true;
			}
		}
	}	
	
	move[0] = row;
	move[1] = col;
}
//takes the move from getMove and updates the board. Also tells the player if they hit or miss
void makeMove(int player, int move[], int OppBoard[][BOARDSIZE], int myBoard[][BOARDSIZE], int guessBoard[][BOARDSIZE]) {
    
    system ("cls");
    
	if(OppBoard[move[0]][move[1]] == 1) {
		OppBoard[move[0]][move[1]] = 2;
		guessBoard[move[0]][move[1]] = 3;
        SetColor(GREEN);
		printf("\nYou hit your opponent's ship!\n");
        SetColor(WHITE);
		dispBoard(myBoard, guessBoard, player);
		
	} else {
        SetColor(RED);
		printf("\nYou missed!\n");
        SetColor(WHITE);
		guessBoard[move[0]][move[1]] = 4;
		dispBoard(myBoard, guessBoard, player);
	}
	//change player's board
	//print if made hit or miss
} 
//returns true if player won the game
bool wonGame(int oppBoard[][BOARDSIZE], int player) {
    for(int row = 0; row < BOARDSIZE; row++) {
        for (int col = 0; col < BOARDSIZE; col++) {
            if(oppBoard[row][col] == 1) { //1 means an undamaged ship
                return false;
            }
        }
        
    }
    return true;
 
}

//All credit for the SetColor function goes to:
//https://www.codewithc.com/change-text-color-in-codeblocks-console-window/
void SetColor(int ForgC)
{
     WORD wColor;               
     //This handle is needed to get the current background attribute
     
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;   
     //csbi is used for wAttributes word
             
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
