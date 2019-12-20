/*
*************************************** FRUIT MACHINE *****************************************************
**** Features:
*		There are three subjects of games which the user can choose
*		Each subject cost a different price
*		The user starts with a balance of 0 credits, can top up multiples of 10 or liquidate his balance 
*		If the user do not have enought credits to pay, that subject will shown in red else green.
*		Each subject has a specific theme (music)
*		Display 7 columns and 5 rows
*		The user can stop column by column from left to right
*		There are differents prizes depends on the subject:
*			- Special prize (If the user mach the full middle row with the special character)
*			- First prize (If the user get a full same characters on row)
*			- Second prize (If the user get six same characters on row)
*			- Third prize (If the user get five same characters on row)
*		The Abertay subject its precial price is match set "A-B-E-R-T-A-Y" in the middle row
**** Creator: FMGD
**** Class: 1st year Computer Games Technology
**** Module: Programming C++ (CMP 104)
**** Year: 2019
**** Contact: 1902654@uad.ac.uk
*/



//1. INCLUDES /////////////////////////////////////////////// 


#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <time.h> //For rand()
#include <conio.h> //For _kbhit() -> detect press key
#pragma comment(lib, "Winmm.lib") //For music

using namespace std;



//2. DEFINES //////////////////////////////////////////////// 




//3. PROTOTYPES ///////////////////////////////////////////// 

/*3.1*/
void setSubject();
void setSpecialPrize();
void setSpecialDetails();
void setValues();
void setSlots();

/*3.2*/
void rotateColumn(const int&);
void rotateSlots();
void stopColumn();
void resetSlotsRotation();

/*3.3*/
int getResults();
void displayEndGame();

/*3.4*/
bool paidPlay(const int&);
void addPrizeToBalance(const int&);

/*3.5*/
void checkKeyPress(const int&);

/*3.6*/
void setOutputColor(const int&, const int& = 0);
void predefinedColorsByChar(const char&, const int&);
void printByCharacter(const string&, const int& = 0);
int getColorDependOnBalance(const int&);

/*3.7*/
void printSlots();
void printIntro();
void printLogo();
void printLoading(const int& = 3000);
void printMainMenu();
void printStartRollingMenu();
void printPrizesInfo();
void printInGameMenu();
void printResults(const int&);
void printEndMenu();
void printCredits();
void clearScreen();

/*3.8*/
void playMusic(string, string = "");




//4. GLOBALS //////////////////////////////////////////////// 

////4.1 - System/Console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

////4.2 - Slots Data
const int NUM_ROWS = 7; //Always has to be a odd number
const int NUM_COLUMNS = 7; //If we modify this value we have to modify the number of column_rolling[]
int middleRow = round(NUM_ROWS / 2.0) - 1; //Main row. It has been susbtracted 1 due to the array start 0

int subject_code = 0; //Subject = 0 = Not subject chosen

//Merely illustrative (Default data)
vector<string> values{ "$", "%", "&", "*", "/", "+", "-" }; //All the characters availables to show in slots 

string slots[NUM_ROWS][NUM_COLUMNS]{ //Slots. If any new character is added it is neccesary modify values[] and/or ocurrences[] and/or column_rolling[]
		{"$", "$", "$", "$", "$", "$", "$"},			//Row 0
		{"%", "%", "%", "%", "%", "%", "%"},			//Row 1
		{"&", "&", "&", "&", "&", "&", "&"},			//Row 2
		{"*", "*", "*", "*", "*", "*", "*"},			//Row 3
		{"/", "/", "/", "/", "/", "/", "/"},			//Row 4
		{"+", "+", "+", "+", "+", "+", "+"},			//Row 5
		{"-", "-", "-", "-", "-", "-", "-"}				//Row 6
};


////4.3 - Balance and Prizes Data
int credit = 0;					//Money of the user
int prizes[4]{ 5000,1000,100,10 };  //Amount to earn in each prize prize[0] = Special price, prize[1]=first prize, prize[2] = second price...
string special_details;				//Info of what character have to mach

////4.4 - State Game and Columns
bool game_running = true; // state of game
bool slots_rolling = false; // state of slots (all)
bool column_rolling[NUM_COLUMNS]{ false, false, false, false, false, false, false }; //Each element represent the state of each column (its state) column_rolling[0] = column 0
bool is_showing_results = false;




//5. FUNCTIONS ////////////////////////////////////////////// 


////5.1. FUNCTIONS - SET PLAY//////////////////////////////////////////////

void setSubject()
{
	//Change Values (characters)
	setValues();

	//Insert that values in slots
	setSlots();

	//Reset slots rotation
	resetSlotsRotation();

	//Set Special prize
	setSpecialPrize();

	//Set the info message for the special prize of each subject
	setSpecialDetails();

}

	///////////////////////////////////////////////////////////

void setSpecialPrize()
{
	if (subject_code == 3)
		prizes[0] = 9000;
	else
		prizes[0] = 5000;
}

///////////////////////////////////////////////////////////

void setSpecialDetails()
{
	special_details = "";

	if (subject_code == 3)
	{
		for (int i = 0; i < NUM_ROWS - 1; i++) //We copy in the same order the characters of values[]
			special_details += values[i] + "-";
		special_details += values[NUM_ROWS -1];
	}
	else
	{
		for (int i = 0; i < NUM_COLUMNS - 1; i++) //We copy one specific character of values[]
			special_details += values[middleRow] + "-";
		special_details += values[middleRow];
	}
}

	///////////////////////////////////////////////////////////

void setValues()
{
	//Set Values
	values.clear();
	switch (subject_code)
	{
	case 1:
		values = { "\x8C", "\xA3", "\x80", "7", "?", "!", "\x24" };
		break;
	case 2:
		values = { "\xD8", "\xDF", "\xBD", "\xD7", "=", "\xF7", "%" };
		break;
	case 3:
		values = { "A", "B", "E", "R", "T", "A", "Y" }; //It is neccessary that the letters are in order (word)
		break;
	}
}

	///////////////////////////////////////////////////////////

void setSlots()
{
	//Set Slots
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int column = 0; column < NUM_COLUMNS; column++)
		{
			slots[i][column] = values[i]; //Change slotsArray
		}

	}
}






////5.2. FUNCTIONS - LOGIC (MECHANISMS SLOTS) //////////////////////////////////////////////

void rotateColumn(const int& column)
{
	string temp;
	temp = slots[NUM_ROWS - 1][column]; //We get the last element in the array

	for (int i = NUM_ROWS - 1; i > 0; i--) //This 'for' go over the array from the last element (from the bottom to top)
	{
		slots[i][column] = slots[i - 1][column]; //Set the current element whith the value of the above element
	}

	slots[0][column] = temp; //Set the saved element to the first position
}

	/////////////////////////////////////////////////////////// 

void rotateSlots()
{

	for (int i = 0; i < NUM_COLUMNS; i++) //Check what columns have to be rotated
	{
		if (column_rolling[i]) rotateColumn(i); //Only rotate columns which are activated
	}
}

	///////////////////////////////////////////////////////////

void stopColumn() //Change the status of the column rotation
{
	for (int i = 0; i < NUM_COLUMNS; i++) //Check what column has to be stopped
	{
		if (column_rolling[i]) //Get the first column that is rotating (from left to right)
		{	
			column_rolling[i] = false; //We set that column rotation to false
			if (i == NUM_COLUMNS - 1) slots_rolling = false; //The player has stopped the last column is neccessary to check if it has won
			break;
		}
	}
}

	/////////////////////////////////////////////////////////

void resetSlotsRotation()
{
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		column_rolling[i] = true;
	}

}






////5.3. FUNCTIONS - LOGIC WIN //////////////////////////////////////////////

int getResults()
{
	//Get the middle row
	int match_found;
	int occurrences[NUM_ROWS] = { 0, 0, 0, 0, 0, 0, 0 }; //Each element correspond to its value in values[]

	for (int i = 0; i < NUM_ROWS; i++) //This for go over values[]
	{
		match_found = 0;
		for (int j = 0; j < NUM_COLUMNS; j++) //This array go over the middle row comparing with the character selected in values[i]
		{
			if (values[i] == slots[middleRow][j])
			{
				match_found++;
			}

		}
		occurrences[i] = match_found; //here it is saved the number of ocurrences found for one character of values[]
	}

	if (subject_code == 3 && NUM_ROWS == NUM_COLUMNS) //For special Subjects
	{
		bool same_order = true; //Check if the middle row is in the same order as values[]
		for (int i = 0; i < NUM_ROWS; i++) //This for go over values[]
		{
			if (values[i] != slots[middleRow][i]) //Compare if the character X on both arrays are in the same position
			{
				same_order = false;
			}
		}
		if (same_order) return 10; //Special prize for subject 3 'word'
	}

	//Read array - //Here is the type of prize (Depending on how may matched found)
	for (int i = 0; i < NUM_ROWS; i++) {

		if (occurrences[i] == NUM_COLUMNS) // First Prize
		{
			if (i == 4 && subject_code != 3) //If it is a full matched row of the "special character" (values[4])
			{
				return 10; //Special prize for subject 1, and 2
			}
			else
				return 1; //Normal characters
		}
		else if (occurrences[i] == NUM_COLUMNS - 1 && (NUM_COLUMNS - 1 > 0)) // Second Price
		{
			return 2;
		}
		else if (occurrences[i] == NUM_COLUMNS - 2 && (NUM_COLUMNS - 2 > 0)) // Third Price
		{
			return 3;
		}
	}

	return -1; //The rest of results, the player has not won.

}

	///////////////////////////////////////////////////////////

void displayEndGame()
{
	int results = getResults(); //Check the middle Row

	clearScreen(); //Clean

	//Printing depend on results

	printSlots();

	printResults(results);

	if (results != -1)
	{
		playMusic("theme" + to_string(subject_code), "prize");
		addPrizeToBalance(results);
	}

	printEndMenu();

	do
	{
		checkKeyPress(3);
	} while (subject_code != 0);

}



////5.4. FUNCTIONS - BALANCE OPERATIONS //////////////////////////////////////////////

bool paidPlay(const int& subject_to_pay)
{

	if ((subject_to_pay == 1 || subject_to_pay == 2) && credit >= 10) //For the first two games
	{
		credit -= 10; //1st and 2nd Game Price
		return true;
	}
	else if (subject_to_pay == 3 && credit >= 30) //For the last game
	{
		credit -= 30; //1st and 2nd Game Price
		return true;
	}
	else //If the user don't have enought money
	{
		return false;
	}
}

	///////////////////////////////////////////////////////////

void addPrizeToBalance(const int& result_code)
{
	credit += prizes[result_code];
}






////5.5. FUNCTIONS - INPUTS CONTROL //////////////////////////////////////////////

void checkKeyPress(const int& menu_type) //Menu
{

	char key;// player input data

	if (_kbhit()) //If any key is push it
	{
		key = toupper(_getch());// get keyboard data, and filter it

		if (menu_type == 0) //Menu for chose subject
		{
			switch (key)
			{
				case 'V': //Player want to play the Las Vegas subject		
					if (paidPlay(1))
					{
						subject_code = 1;
						setSubject();
						printLoading(1500);
						playMusic("theme" + to_string(subject_code));
					}
					break;

				case 'M': //Player want to play the Maths subject
					if (paidPlay(2))
					{
						subject_code = 2;
						setSubject();
						printLoading(1500);
						playMusic("theme" + to_string(subject_code));
					}
					break;

				case 'A': //Player want to play the Abertay Subject
					if (paidPlay(3))
					{
						subject_code = 3;
						setSubject();
						printLoading(1500);
						playMusic("theme" + to_string(subject_code));
					}
					break;

				case 'T': //Player want to top up his credit
					credit += 10;
					//playMusic("lounge","insert_credit");
					printMainMenu(); //We update the screen
					break;

				case 'L': //Player want to top up his credit
					if (credit > 0) {
						credit = 0;
						//playMusic("lounge", "get_credit");
						printMainMenu(); //We update the screen
					}
					break;

				case 'E': // player trying to exit
					game_running = false;
					break;
			}
			
		}
		else if (menu_type == 1) //Menu for start rolling
		{
			switch (key)
			{

				case 'R': //Player want to roll the slots
					slots_rolling = true;
					break;
			}

		}
		else if (menu_type == 2) { //Menu for slots
			switch (key)
			{
				case 'S': //Player want to stop a column
					if (slots_rolling) stopColumn();
					break;
			}
		}
		else if (menu_type == 3) { //Menu for slots
			switch (key)
			{
				case 'B': //Player want to go back exit to Main Menu				
					is_showing_results = false;
					subject_code = 0;
					printLoading(2000);
					break;
			}
			
		}

	} // end if 

}






//5.6. FUNCTIONS - Outputs Logic/Customizations //////////////////////////////////////////////

void setOutputColor(const int& f_color, const int& bg_color) { //Default black

	/*
		COLOR               INT VALUES
		------------------------------ -
		BLACK                   0
		BLUE                    1
		GREEN                   2
		CYAN                    3
		RED                     4
		MAGENTA                 5
		BROWN                   6
		LIGHTGRAY               7
		DARKGRAY                8
		LIGHTBLUE               9
		LIGHTGREEN             10
		LIGHTCYAN              11
		LIGHTRED               12
		LIGHTMAGENTA           13
		YELLOW                 14
		WHITE                  15
	*/

	SetConsoleTextAttribute(hConsole, (WORD)((bg_color << 4) | f_color));
}

	///////////////////////////////////////////////////////////

void predefinedColorsByChar(const char& ch, const int& bg_color)
{
	switch (ch)
	{
	case 'A':
	case '\x8C':
	case'\xBD':
		setOutputColor(1, bg_color);
		break;
	case 'C':
	case '\xA3':
	case'\xF7':
		setOutputColor(2, bg_color);
		break;
	case 'E':
	case'\x80':
	case 'G':
	case'=':
		setOutputColor(3, bg_color);
		break;
	case 'F':
	case '\x24':
	case '\xDF':
		setOutputColor(4, bg_color);
		break;
	case 'H':
	case '?':
	case'\xD7':
		setOutputColor(5, bg_color);
		break;
	case 'I':
	case '!':
	case'%':
	case 'Y':
		setOutputColor(6, bg_color);
		break;
	case 'M':
		setOutputColor(7, bg_color);
		break;
	case 'N':
	case '\xD8':
	case '7':
		setOutputColor(8, bg_color);
		break;
	case 'R':
		setOutputColor(9, bg_color);
		break;
	case 'T':
		setOutputColor(10, bg_color);
		break;
	case 'U':
		setOutputColor(11, bg_color);
		break;
	case 'B':
		setOutputColor(12, bg_color);
		break;
	case 'D':
		setOutputColor(13, bg_color);
		break;
	case 'P':
		setOutputColor(14, bg_color);
		break;
	case '#':
		setOutputColor((rand() % 14) + 1, 0);
		break;
	default:
		setOutputColor(15, bg_color);
		break;

	}
}

	///////////////////////////////////////////////////////////

void printByCharacter(const string& str, const int& special_bg_color) {
	for (size_t i = 0; i < str.length(); i++)
	{
		predefinedColorsByChar(str[i], special_bg_color);
		cout << str[i];
	}
}

	///////////////////////////////////////////////////////////

int getColorDependOnBalance(const int& cost)
{
	if (credit >= cost)
	{
		return 2; //GREEN
	}
	else
	{
		return 4; //RED
	}
}

	///////////////////////////////////////////////////////////






//5.7. FUNCTIONS - Messages (Outputs) //////////////////////////////////////////////

void printSlots()
{
	//Clean Screen before 

	clearScreen();
	string border = "				#########################################################\n";
	string line;
	const int bg_color_mid = 15;//(rand() % 14) + 1;

	//Print decoration top
	cout << endl << endl << endl;
	printByCharacter(border);

	//Print the slots (and lateral decoration)
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int column = 0; column < NUM_COLUMNS; column++)
		{
			if (column == 0)
			{
				line = "				#		 " + slots[row][column] + " "; //First column
			}
			else if (column == NUM_COLUMNS - 1)
			{
				line = "  " + slots[row][column] + " " + "		#				"; //Last column
			}
			else
			{
				line = "  " + slots[row][column] + " "; //Rest column
			}

			if (row == middleRow) printByCharacter(line, bg_color_mid); //Print the middle row with different bg 
			else printByCharacter(line);

		}
		cout << endl;
	}

	//Print bottom decoration
	printByCharacter(border);
}

	///////////////////////////////////////////////////////////

void printIntro() {

	//Print Logo
	cout << "\n\n\n\n\n\n\n\n\n\n";
	printLogo();
	Sleep(3000);

	//Print Loading info
	printLoading();

}

	///////////////////////////////////////////////////////////

void printLogo()
{
	string title = "\n" +
		string(	"			FFFF RRRR  U   U III TTTTTT  M   M  AA   CCC H  H III N   N EEEE			\n") +
							"			F    R   R U   U  I    TT    MM MM A  A C    H  H  I  NN  N E				\n" +
							"			FFF  RRRR  U   U  I    TT    M M M AAAA C    HHHH  I  N N N EEE				\n" +
							"			F    R R   U   U  I    TT    M   M A  A C    H  H  I  N  NN E				\n" +
							"			F    R  RR  UUU  III   TT    M   M A  A  CCC H  H III N   N EEEE	BY FMGD	\n";

	printByCharacter(title);
}

	///////////////////////////////////////////////////////////

void printLoading(const int& milliseconds)
{
	clearScreen();
	string loading =
		string( "				 _                          _   _\n") +
				"				| |      ___     __ _    __| | (_)  _ __     __ _\n" +
				"				| |     / _ \x5C   / _` |  / _` | | | | '_ \x5C   / _` |\n" +
				"				| |___ | (_) | | (_| | | (_| | | | | | | | | (_| |  _  _  _\n" +
				"				|_____| \x5C___ /  \x5C__,_|  \x5C__,_| |_| |_| |_|  \x5C__, | (_)(_)(_)\n" +
				"				                                            |___/\n";
	cout << "\n\n\n\n\n\n\n\n\n\n";
	cout << loading;
	Sleep(milliseconds);//Time showing loading
}

	///////////////////////////////////////////////////////////

void printMainMenu()
{
	clearScreen();
	printLogo();

	cout << "\n\n\n\n" +
		string("			Select one of the following modes of games:		  \n\n") +
		"			Subjects			Press the key		Cost (credits)\n" +
		"			---------			-------------		-------------\n";

	setOutputColor(getColorDependOnBalance(10));
	cout << "			Las Vegas			       V		     10	\n" +
		string("			Maths				       M		     10	\n");

	setOutputColor(getColorDependOnBalance(30));
	cout <<
		string("			Abertay				       A		     30	\n\n\n");

	setOutputColor(15);
	cout <<
		string("			My account:\n\n") +
		"			Options				Press the key		My Balance (credits)\n" +
		"			----------------		-------------		------------------\n" +
		"			Top up 10 credits		       T		     " + to_string(credit) + " \n" +
		"			Liquidate account		       L		     \n\n\n" +
		"			For Exit press 'E'.\n";

	//Wait for key press
	do
	{
		checkKeyPress(0);
	} while (subject_code == 0 && game_running);

}

	///////////////////////////////////////////////////////////

void printStartRollingMenu()
{
	cout <<			"\n" +
		string("				Commands Availables:							\n") +
				"				Action				Press the key				\n" +
				"				---------			-------------				\n" +
				"				Rolling					R						\n" +
				"\n";
}

	///////////////////////////////////////////////////////////

void printPrizesInfo()
{
	cout << "\n" +
		string("				Prizes:	\n") +
		"				Name			Details				Amount(Credits)\n" +
		"				---------		-------------			---------------\n" +
		"				Special Event		" + special_details + "			" + to_string(prizes[0]) + "\n"	+
		"				Jackpot 		Seven characters matched	" + to_string(prizes[1]) + "	\n" +
		"				MiniJackpot		Six characters matched		" + to_string(prizes[2]) + "		\n" +
		"				Third Prize		Five characters matched		" + to_string(prizes[3]) + "		\n" +
		"\n";
}

	///////////////////////////////////////////////////////////

void printInGameMenu()
{
	cout << "\n" +
		string("				Commands Availables:						\n") +
				"				Action				Press the key			\n" +
				"				---------			-------------			\n" +
				"				Stop Column				S					\n" +
				"				(left to right)								\n" +
				"\n";
			
}

	///////////////////////////////////////////////////////////

void printResults(const int& results_code)
{

	cout << "\n\n				 _______________________________________________________\n";
	cout << "				|                                                       |" << endl;

	switch (results_code)
	{
	case 1:
		cout << "				|                FIRST PRIZE: " + to_string(prizes[1]) + " credits              |" << endl;
		break;
	case 2:
		cout << "				|                SECOND PRIZE: " + to_string(prizes[2]) + " credits              |" << endl;
		break;
	case 3:
		cout << "				|                THIRD PRIZE: " + to_string(prizes[3]) + " credits                |" << endl;
		break;
	case 10:
		cout << "				|               SPECIAL PRIZE: " + to_string(prizes[0]) + " credits             |" << endl;
		break;
	default:
		cout << "				|                Try again, you can do it!              |" << endl;
		break;

	}

	cout << "				|_______________________________________________________|\n\n";

}

	///////////////////////////////////////////////////////////

void printEndMenu()
{
	cout << "\n" +
		string("				Commands Availables:							\n") +
		"				Action				Press the key				\n" +
		"				---------			-------------				\n" +
		"				Go Back to Main Menu			B						\n" +
		"\n";
}

	///////////////////////////////////////////////////////////

void printCredits()
{
	clearScreen();
	string credits = "\n\n\n\n\n\n\n" +
				string("			#################################################################################\n") +
				"			#	    				    	                                #\n" +
				"			#	Credits:								#\n" +
				"			#	---------			-------------	                        #\n" +
				"			#	Creater				Francisco Diaz				#\n" +
				"			#	Class				Computer Games Technology - 1st Year	#\n" +
				"			#	Module				Programming with C++ (CMP104)	        #\n" +
				"			#	Year				2019	                                #\n" +
				"			#	Contact				1902654@uad.ac.uk	                #\n" +
				"			#	    				    	                                #\n" +
				"			#################################################################################\n" +
				"\n\n\n\n\n\n";
	printByCharacter(credits);
}


///////////////////////////////////////////////////////////

void clearScreen() {
	system("cls");
}




//5.8. FUNCTIONS - Music //////////////////////////////////////////////

void playMusic(string theme_file_name, string event_sound_file_name) { //Only for .WAV files, just play the music directly without opening the file

	theme_file_name = "music/" + theme_file_name + ".wav"; //Set route
	DWORD  fdwSound = SND_FILENAME | SND_NODEFAULT ; //Flags for playing the sound
	//SND_NODEFAULT = Don't play the default sound if filename cannot be found

	if (!event_sound_file_name.empty()) //For sounds (events)
	{
		event_sound_file_name = "music/" + event_sound_file_name + ".wav";
		fdwSound += SND_SYNC; //The sound is played synchronously, and PlaySound returns after the sound event completes. 
		PlaySound(event_sound_file_name.c_str(), GetModuleHandle(NULL), fdwSound);//just play the music directly without opening the file

	}

	fdwSound += SND_ASYNC | SND_LOOP;
	PlaySound(theme_file_name.c_str(), GetModuleHandle(NULL), fdwSound);//just play the music directly without opening the file
	//SND_ASYNC: The sound is played asynchronously and PlaySound returns immediately after beginning the sound
	//SND_LOOP: The sound plays repeatedly until PlaySound is called again with the pszSound parameter set to NULL

}





// MAIN GAME LOOP ///////////////////////////////////////// 

int main()

{
	//SECTION: CONFIGURE

	SetConsoleOutputCP(1252); //Set to Windows Codepage 1252
	
	srand((unsigned)time(NULL));// seed the random number generator with time for use rand()



	//SECTION: GAME

	printIntro();

	while (game_running)
	{
		//SECTION: MAIN MENU

		playMusic("lounge");
		printMainMenu();


		//SECTION: PLAY

		if (subject_code != 0)
		{

			//SECTION: SHOW INFO BEFORE START PLAY

			do //Show the slots stoped
			{
				printSlots();
				printStartRollingMenu();
				printPrizesInfo();
				checkKeyPress(1);
				Sleep(60);
			} while (!slots_rolling); //While the user doesn't start to play



			//SECTION: ROLLING SLOTS (PLAYING)

			while (slots_rolling) //Here the user is playing
			{
				//- Get player input
				checkKeyPress(2);

				//- Rotation action
				rotateSlots();
				printSlots();
				printInGameMenu();
				Sleep(70);
			}


			//SECTION: END OF PLAY

			displayEndGame(); //Show results and wait for action

		}

	}


	//SECTION: GAME CLOSED

	printCredits();

	return 0;

} // end main 