// Maze.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include "cls.h"			//this is included twice
#include "simplemenu.h"

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

enum move { LEFT  = 'a',
			RIGHT = 'd',
            DOWN  = 's',
			UP    = 'w',
			ESC   = 27 /*ASCII for ESC*/};

//enum arrows{	LEFT = 75,	//Arrow left
//			RIGHT= 77,	//Arrow right
//			DOWN = 80,	//Arrow down
//			UP	 = 72,	//Arrow up
//			ESC  = 27 /*ASCII for ESC*/};


enum tiles{ HERO     = 'H',
	        WALL     = '#',
	        CORRIDOR = '.',
	        EXIT     = 'O'};



struct Score {
	int level = 0;
	int score = 0;
	std::string nickname = {};
};
struct position {
	unsigned int level = {};
	unsigned int row = {};
	unsigned int column = {};
};


bool Highscore(const Score & player_score) {
	
	std::cout << '\n' << player_score.nickname << "\'s score:\n"
		<< "Levels passed:\t" << player_score.level << '\n'
		<< "Points:\t\t" << player_score.score << '\n\n';
	
	const std::string savefilename = "E:\\pliki\\Projekty\\Github\\Maze\\Maze\\Tabela_wynikow.txt";
	Score current_highscore = {};

	std::ifstream saved_score(savefilename);
	if (!saved_score){
		std::cout << "Could not open file " << savefilename << " .\a\n";
		return EXIT_FAILURE;
	};
	saved_score >> current_highscore.level;
	saved_score >> current_highscore.score;
	saved_score >> current_highscore.nickname;
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
	save_score << player_score.level << "\n" << player_score.score << "\n" << player_score.nickname;
	save_score.close();
	std::cout << "Your score has been saved!:\a\n";

	_getch();
	return EXIT_SUCCESS;
};
int Game(std::vector<std::vector<std::vector<char>>> map) {
	Score current_score = {};
	std::cout << "Input Your nickname: ";
	std::getline(std::cin, current_score.nickname);
	unsigned int levelscore = 0;
	position hero;
	char *herotile = NULL;
	char *destinationtile = NULL;
	char *exittile = NULL;
	char move = {};
	std::string buffer{};
	
	//Game loop
	cls(hStdout);
	for (size_t level = 0; level < map.size(); level++) {
		current_score.level++;
		levelscore = 0;
		do{
			//Draw map
			for (size_t row = 0; row < map[level].size(); row++) {
				for (size_t column = 0; column < map[level][row].size(); column++) {
					/*switch (map[level][row][column]){
							case HERO: {std::cout << char(234); }; break;
							case WALL: {std::cout << char(219); }; break;
							case EXIT: {std::cout << EXIT; }; break;
							case CORRIDOR: {std::cout << char(234); }; break;
							default: {std::cout << "Wrong tile supplied, expected ("
												<< HERO << ' ' << WALL << ' ' << CORRIDOR << ' ' << EXIT
												<< "), recieved " << map[level][row][column] << '\a\n';
												_getch();
												return EXIT_FAILURE; } break;
					};*/

					std::cout << (map[level][row][column] == WALL ? char(219) : map[level][row][column]);
					//buffer.append(map[level][row][column] == WALL ? char(219) : map[level][row][column]);
					switch (map[level][row][column]) {
					case 'H': { herotile = &map[level][row][column]; 
								hero.level = level; 
								hero.row = row;
								hero.column = column; }; break;
					case 'O': { exittile = &map[level][row][column]; }; break;

					default:
						break;
					}
				};
				std::cout << '\n';
				//buffer.append('\n');
			};
			//std::cout << buffer;
			std::cout << "Current score: " << current_score.score << "\tLevel score: " << levelscore << '\n';
			
			//Move
			move = _getch();

			//if (move == '0' || move == '0xE0') { move = _getch(); };	//arrow keys are precieded either by 0 or 0xE0 in input
			switch (move) {
				case LEFT: {destinationtile = &map[hero.level][hero.row][hero.column - 1];
							if (*destinationtile != CORRIDOR) { std::cout << '\a'; break; };
							std::swap(*herotile, *destinationtile);
							levelscore++; }; break;
				case RIGHT:{destinationtile = &map[hero.level][hero.row][hero.column + 1];
							if (*destinationtile != CORRIDOR) { std::cout << '\a'; break; };
							std::swap(*herotile, *destinationtile);
							levelscore++; }; break;
				case DOWN: {destinationtile = &map[hero.level][hero.row + 1][hero.column];
							if (*destinationtile != CORRIDOR) { std::cout << '\a'; break; };
							std::swap(*herotile, *destinationtile);
							levelscore++; }; break;
				case UP:   {destinationtile = &map[hero.level][hero.row - 1][hero.column];
							if (*destinationtile != CORRIDOR) { std::cout << '\a'; break; };
							std::swap(*herotile, *destinationtile);
							levelscore++; }; break;
				case ESC:  {std::cout << "Are you sure you want to exit? Y/N : "; 
					        if (_getch() == 'y' || _getch() == 'Y') return EXIT_SUCCESS; }; break;
				default: {std::cout << "\nMove not defined!\a\n"; _getch(); }; break;
			}
			cls(hStdout);
		} while (!(destinationtile==exittile));
		current_score.score += levelscore;
		cls(hStdout);
		//std::cout << "\n\n";
	};

	Highscore(current_score);	
	return EXIT_SUCCESS;
};
int PrepareMap(const unsigned int & width, const unsigned int & height) {
	const unsigned int amount_of_levels = 3;

	std::vector<std::vector<std::vector<char>>> map = {};
	std::vector<char> full_row = {};
	std::vector<std::vector<char>> full_level = {};
	char tile = {};

	unsigned int levels_read = 0;
	std::string filename = {};
	switch (width) {
	case 10: {filename = "E:\\pliki\\Projekty\\Github\\Maze\\Maze\\Level_easy.txt";   }; break;
	case 20: {filename = "E:\\pliki\\Projekty\\Github\\Maze\\Maze\\Level_medium.txt"; }; break;
	case 30: {filename = "E:\\pliki\\Projekty\\Github\\Maze\\Maze\\Level_hard.txt";   }; break;
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

	////////////////////////////////////////////////////////////////////////Wrzucanie mapy z pliku do tablicy tab
	
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
	Game(map);
	return EXIT_SUCCESS;
};
int menu() {
	Score Score;
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

	//menu 
	do
	{
		input[0] = simplemenu(mainmenu, "Main Menu");

		switch (input[0])
		{
		case 0: {do	{
						input[1] = simplemenu(difficulty_level, "Chose difficulty");
						switch (input[1])
						{
						case 0: {PrepareMap(10,20);}; break;
						case 1: {PrepareMap(20,40);}; break;
						case 2: {PrepareMap(30,60);}; break;
						default: break;
						};
					} while (input[1] != difficulty_options); }; break;
		case 1: {cls(hStdout); 
				 std::cout << "1.DESCRIPTION------------------------------------------------------------.\n"
						   << "Goal of this simple game is going through maze.\n"
						   << "Player has unlimitted moves and time to finish, there are also no enemies.\n\n"
						   << "2.LEGEND-----------------------------------------------------------------.\n"
			               << "H - Player controlled hero.\n"
			               << ". - Corridor that player can go through.\n"
			               << "# - Wall.\n"
			               << "O - Level exit.\n"
			               << "-------------------------------------------------------------------------.\n\n"
					       << "Press any key to continue ...\n";
				 _getch(); }; break;
		case 2: {std::ifstream Scores("Tabela_wynikow.txt");
				 Scores >> Score.level;
				 Scores >> Score.score;
				 Scores >> Score.nickname;
				 Scores.close();
				 std::cout << "Nick\t|\tLevels passed\t|\tScore\t|\t\n" 
					       << "---------------------------------------------------\n"
					       << Score.nickname << "\t\t" << Score.level << "\t\t\t" << Score.score << "\n\n"
					       << "Press any key to continue ...\n";
				 _getch(); }; break;
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