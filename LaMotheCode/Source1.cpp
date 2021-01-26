//  A game fragment adapted from Andre La Mothe's book 

//    The Black Art of 3D Games Programming 

//  Modified by CJM & NC to run without error or warnings 

//    Note: this is very old-fashioned code originally written for 16-bit PCs 



// INCLUDES /////////////////////////////////////////////// 



#include <stdio.h> 

#include <stdlib.h> 

#include <ctype.h>  

#include <conio.h> 

#include <windows.h> 

#include <time.h> 

#include <string> 

#include <iostream> 


using namespace std;



// DEFINES //////////////////////////////////////////////// 



#define MAX_X        110  // maximum x position for player 

#define SCROLL_POS   29  // the point that scrolling occurs 



// PROTOTYPES ///////////////////////////////////////////// 



void init_Graphics();

void Set_Color(int fcolor, int bcolor);

void Draw_String(int x, int y, string st);



// GLOBALS //////////////////////////////////////////////// 



CONSOLE_SCREEN_BUFFER_INFO con_info;   // holds screen info 



HANDLE hconsole;         // handle to console 

int    game_running = 1; // state of game, 0=done, 1=run 

int    scroll_number = 0;


// FUNCTIONS ////////////////////////////////////////////// 



void init_Graphics()

{

	// this function initializes the console graphics engine 



	COORD console_size = { 110, 30 }; // size of console 



									 // seed the random number generator with time 

	srand((unsigned)time(NULL));



	// open i/o channel to console screen 

	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,

		FILE_SHARE_READ | FILE_SHARE_WRITE,

		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);



	// make sure we are in 80x25 

	SetConsoleScreenBufferSize(hconsole, console_size);



	// get details for console screen                        

	GetConsoleScreenBufferInfo(hconsole, &con_info);



} // end init_Graphics 



  /////////////////////////////////////////////////////////// 



void Set_Color(int fcolor, int bcolor = 0)

{

	// this function sets the color of the console output 

	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) |

		fcolor));



} // Set_Color 



  /////////////////////////////////////////////////////////// 



void Draw_String(int x, int y, string s)

{

	// this function draws a string at the given x,y 



	COORD cursor_pos; // used to pass coords 



					  // set printing position 

	cursor_pos.X = x;

	cursor_pos.Y = y + scroll_number;

	SetConsoleCursorPosition(hconsole, cursor_pos);



	// print the string in current color 

	//printf("%s", s); 

	cout << s;



} // end Draw_String 



  /////////////////////////////////////////////////////////// 



void Clear_Screen(void)

{

	// this function clears the screen 



	// set color to white on black 

	Set_Color(15, 0);



	// clear the screen 

	for (int index = 0; index <= 30; index++)

		Draw_String(0, SCROLL_POS, "\n");
	//system("cls");


} // end Clear_Screen 


// MAIN GAME LOOP ///////////////////////////////////////// 

int main()

{

	char key;            // player input data 

	int  player_x = 40;  // player's x 

	//7.Display a score:
	int score = 0;
	const int MAX_SCORE = 99;



						 // set up the console text graphics system 

	init_Graphics();



	// clear the screen 

	Clear_Screen();



	// SECTION: main event loop, this is where all the action   

	// takes place, the general loop is erase-move-draw 



	while (game_running)

	{

		// SECTION: erase all the objects or clear screen 



		// nothing to erase in our case 


		// SECTION: get player input 

		if (_kbhit())

		{

			// get keyboard data, and filter it 

			key = toupper(_getch());


			// is player trying to exit, if so exit 

			//5.Input: if (key == 'Q' || key == 27)
			if (key == 'X' || key == 27)

			{

				game_running = 0;

			}

			// is player moving left         

			//5.Input: if (key == 'A')
			if (key == 'L' || key == 75)

			{

				//4.Movement:  player_x--;
				player_x -= 2;

			}

			// is player moving right 

			//5.Input: if (key == 'S')
			if (key == 'R' || key == 77)

			{

				//4.Movement: player_x++;
				player_x += 2;
			}



		} // end if    



		  // SECTION: game logic and further processing 

		//7.Display a score:
		//SECTION: Increment score
		score++;

		  // make sure player stays on screen  

		if (++player_x > MAX_X)

		{

			player_x = MAX_X;

		}



		if (--player_x < 0)

		{

			player_x = 0;

		}



		// SECTION: draw everything 



		// draw next star at random position 

		Set_Color(15, 0);

		//3.Missiles Draw_String(rand() % MAX_X, SCROLL_POS, ".\n");
		string missiles[] = { "F", "M", "G", "D" };
		Draw_String(rand() % MAX_X, SCROLL_POS, missiles[rand() % 4] + "\n");

		// draw player

		//2.Colours: Set_Color(rand() % 15, 0); // 15 is because there is a max of 15 colors available.
		Set_Color(1,9); // The first parameter is front-color and the second in background-color

		//1.Craft: Draw_String(player_x, 0, "<--*-->");
		Draw_String(player_x, 0, "<--^.^-->");

		scroll_number++;

		// SECTION: synchronize to a constant frame rate 

		//6.Speed: Sleep(100);
		Sleep(100);//Parameter Milliseconds

		//7. Displaying a score: Draw_String(0, 0, "");
		Set_Color(0, 0);
		Draw_String(MAX_X, 1, ""); //First parameter MAX_X put it on the right, 0 put it on the top

		//7. Displaying a score
		printf("33[2K");// Erases the entire line where the cursor is currently on

		//7. Displaying a score Draw_String(0, 0, "");
		Set_Color(rand() % 15, 0);
		Draw_String(MAX_X, 2, to_string(score)); //First parameter MAX_X put it on the right, 0 put it on the top

		//Display Score
		if (score == MAX_SCORE)
			game_running = 0;


	} // end while 



	  // SECTION: shutdown and bail 

	Clear_Screen();



	cout << "\nG A M E  O V E R \n\n";



	return 0;

} // end main 
