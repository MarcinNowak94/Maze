// Maze.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include "cls.h"			//this is included twice
#include "simplemenu.h"
HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

void sleep(float seconds) {
	clock_t startClock = clock();
	float secondsAhead = seconds * CLOCKS_PER_SEC;
	// do nothing until the elapsed time has passed.
	while (clock() < startClock + secondsAhead);
	return;
}

struct Score {
	int level = 0;
	int score = 0;
	std::string name = {};
};
struct position {
	unsigned int level = {};
	unsigned int row = {};
	unsigned int column = {};
	
	char* tileup = {};
	char* tiledown = {};
	char* tileleft = {};
	char* tileright = {};
};

bool Highscore(const Score & player_score, std::string& gamepath) {
	std::cout << '\n' << player_score.name << "\'s score:\n"
		<< "Levels passed:\t" << player_score.level << '\n'
		<< "Points:\t\t" << player_score.score << '\n\n';
	
	const std::string savefilename = gamepath+"Tabela_wynikow.txt";
	Score current_highscore = {};

	std::ifstream saved_score(savefilename);
	if (!saved_score){
		std::cout << "Could not open file " << savefilename << " .\a\n";
		return EXIT_FAILURE;
	};
	saved_score >> current_highscore.level;
	saved_score >> current_highscore.score;
	saved_score >> current_highscore.name;
	saved_score.close();

	if (current_highscore.level>= player_score.level && 
		current_highscore.score< player_score.score ){
		std::cout << "Sorry gal/pal, You were close. Current highscore is:\n"
			<< "Levels passed:\t" << current_highscore.level << '\n'
			<< "Points:\t\t" << current_highscore.score << '\n\n'
			<< "You were " << player_score.score-current_highscore.score << " less moves shy of being a true champion!";
		_getch();
		return EXIT_SUCCESS;
	};

	std::ofstream save_score(savefilename);
	save_score << player_score.level << "\n" << player_score.score << "\n" << player_score.name;
	save_score.close();
	std::cout << "Your score has been saved!:\a\n";

	_getch();
	return EXIT_SUCCESS;
};
void DisplayScore(std::string& filename) {
	Score Score;
	std::ifstream Scores("Tabela_wynikow.txt");
	Scores >> Score.level;
	Scores >> Score.score;
	Scores >> Score.name;
	Scores.close();
	std::cout << "Nick\t|\tLevels passed\t|\tScore\t|\t\n"
		<< "---------------------------------------------------\n"
		<< Score.name << "\t\t" << Score.level << "\t\t\t" << Score.score << "\n\n"
		<< "Press any key to continue ...\n";
}
std::string& DrawMap(
	std::vector<std::vector<char>> map,
	position& hero, 
	position& exit,
	int sessionscore,
	int levelscore,
	std::string& buffer) {
	for (size_t row = 0; row < map.size(); row++) {
		for (size_t column = 0; column < map[row].size(); column++) {

			buffer += (map[row][column] == WALL ? char(219) : map[row][column]);
			switch (map[row][column]) {
			case HERO: {
				hero.row = row;
				hero.column = column; }; break;
			case EXIT: {
				exit.row = row;
				exit.column = column; }; break;
			default:
				break;
			}
		};
		buffer += ('\n');
	};
	buffer += "Current score: "
		+ std::to_string(sessionscore)
		+ "\tLevel score: "
		+ std::to_string(levelscore) + '\n';
	return buffer;
};
int Game(std::vector<std::vector<std::vector<char>>> map, std::string& gamepath, Score current_score) {
	int levelscore = -1;
	position hero;
	position exit;
	char *herotile = NULL;
	char *destinationtile = NULL;
	char *exittile = NULL;
	char move = {};
	
	
	//Game loop ------------------------------
	Solver solver;

	cls(hStdout);
	for (size_t level = 0; level < map.size(); level++) {
		std::vector<std::vector<char>> currentlevel = map[level];
		current_score.level++;
		hero.level = exit.level = level;
		do{
			cls(hStdout);
			std::string buffer{};
			//Draw map
			DrawMap(currentlevel,
				hero, 
				exit, 
				current_score.score, 
				levelscore++, 
				buffer);
			std::cout << buffer;
			
			//Move
			
			//Sztuczna Inteligencja, LAB1: Maze Solver; Sekcja3 5ION 2022/23
			
			//solver.rotate(movLEFT);
			//solver.facingWall(tiles(*destinationtile) 
			// y - solver.rotate(movRIGHT); //rotate -LEFT
			// n - solver.advance(), solver.rotate(movLEFT);
			//move = solver.advance();
			
			//move = _getch();
			
			//do {
				//sleep(1);
				//solver.rotate(movLEFT);		//check left
				herotile = &currentlevel[hero.row][hero.column];
				hero.tileup = &currentlevel[hero.row - 1][hero.column];
				hero.tiledown = &currentlevel[hero.row + 1][hero.column];
				hero.tileleft = &currentlevel[hero.row][hero.column - 1];
				hero.tileright = &currentlevel[hero.row][hero.column + 1];
				exittile= &map[exit.level][exit.row][exit.column];
				
				move = _getch(); //= solver.advance();	//chek if wall
				switch (move) {
				case LEFT: {destinationtile = hero.tileleft; }; break;
				case RIGHT: {destinationtile = hero.tileright; }; break;
				case DOWN: {destinationtile = hero.tiledown; }; break;
				case UP: {destinationtile = hero.tileup; }; break;
				case ESC: {std::cout << "Are you sure you want to exit? Y/N : ";
					if (_getch() == 'y' || _getch() == 'Y') return EXIT_SUCCESS; }; break;
				default: {std::cout << "\nMove not defined!\a\n"; _getch(); }; break;
				}
				
				//if (solver.facingWall((tiles)*destinationtile)) { solver.rotate(movRIGHT); };
			//} while (solver.facingWall(tiles(*destinationtile)));

			//if (move == '0' || move == '0xE0') { move = _getch(); };	//arrow keys are precieded either by 0 or 0xE0 in input
			
			if (*destinationtile != CORRIDOR) { std::cout << '\a'; continue; };
			std::swap(*herotile, *destinationtile);
			levelscore++;
		} while (!(destinationtile==exittile));
		current_score.score += levelscore;
		cls(hStdout);
		//std::cout << "\n\n";
	};

	Highscore(current_score, gamepath);	
	return EXIT_SUCCESS;
};
int PrepareMap(const unsigned int & width, const unsigned int & height, std::string& gamepath) {
	const unsigned int amount_of_levels = 3;

	std::vector<std::vector<std::vector<char>>> map = {};
	std::vector<char> full_row = {};
	std::vector<std::vector<char>> full_level = {};
	char tile = {};
	std::string filename = {};
	unsigned int levels_read = 0;

	switch (width) {
	case 10: {filename = gamepath + "Level_easy.txt";   }; break;
	case 20: {filename = gamepath + "Level_medium.txt"; }; break;
	case 30: {filename = gamepath + "Level_hard.txt";   }; break;
	default: {std::cout << "Wrong dimensions! Expected width 10|20|30\tRecieved: " << width << "\n"; 
		      _getch(); 
			  return EXIT_FAILURE; }break;
	};

	std::ifstream map_file(filename);
	if (!map_file) {
			std::cout << "Could not open file "<< filename <<"\n\a";
			_getch();
			return EXIT_FAILURE;
	};
	//Read map
	
	for (size_t level = 0; level < amount_of_levels; level++){
		full_level.clear();
		for (size_t row = 0; row < width; row++){
			full_row.clear();
			for (size_t column = 0; column < height; column++){
				map_file >> tile;
				full_row.push_back(tile);
			};
			full_level.push_back(full_row);
		};
		map.push_back(full_level);
		//Count how many levels are read [0][2] is always a wall
		if (map[level][0][2] == '#') {
			levels_read = levels_read++;
		};
		
		//Exit at EOF, empty lines are considered EOF
		if (map_file.peek() == EOF) { break; }
	};
	map_file.close();

	Score score = {};
	std::cout << "Input Your nickname: ";
	std::getline(std::cin, score.name);

	Game(map, gamepath, score);
	return EXIT_SUCCESS;
};

int menu() {
	static std::string gamepath = "E:\\Projects\\VisualStudio\\Projects\\Git\\C++\\Maze\\Maze\\";
	
	cls(hStdout);			/*clear terminal window*/

	const std::string mainmenu[] = {
		"New game",
		"Help",
		"Best scores"
	};
	const std::string difficulty_level[] = {
		"Easy",
		"Medium",
		"Hard"
	}; 
	const unsigned int menu_options = (sizeof(mainmenu) / sizeof(*mainmenu));
	const unsigned int difficulty_options = (sizeof(difficulty_level) / sizeof(*difficulty_level));

	int input[2]{}; /*input for menu and submenus*/

	//Prepare instructions
	std::string instructions = "1.DESCRIPTION------------------------------------------------------------.\n";
	instructions += "Goal of this simple game is going through maze.\n";
	instructions += "Player has unlimitted moves and time to finish, there are also no enemies.\n\n";
	instructions += "2.LEGEND-----------------------------------------------------------------.\n";
	instructions += HERO + " - Player controlled character.\n";
	instructions += CORRIDOR + " - Corridor that player can go through.\n";
	instructions += WALL + " - Wall.\n";
	instructions += EXIT + " - Level exit.\n";
	instructions += "-------------------------------------------------------------------------.\n\n";
	instructions += "Press any key to continue ...\n";

	//menu 
	do
	{
		input[0] = simplemenu(mainmenu, "Main Menu");
		switch (input[0]) {
			case 0: {
				do	{
					input[1] = simplemenu(difficulty_level, "Chose difficulty");
					switch (input[1]){
						case 0: {PrepareMap(10,20, gamepath);}; break;
						case 1: {PrepareMap(20,40, gamepath);}; break;
						case 2: {PrepareMap(30,60, gamepath);}; break;
						default: break;
						};
					} while (input[1] != difficulty_options); 
			}; break;
			case 1: {
				cls(hStdout); 
				std::cout << instructions;
				_getch(); 
			}; break;
			case 2: {
				DisplayScore(gamepath + "Tabela_wynikow.txt");
				_getch(); 
			}; break;
		default: break;
		};
	} while (input[0] != menu_options);
	std::cout << "\n\a\tFarewell!\n"
		<< "Press any key to exit...";
	_getch();
	return EXIT_SUCCESS;
};

int main(){
	menu();
};