#include "battleship.hpp"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <string>

/// The size of one side of the playing board.
const int size = 10;

/// Number of the ships.
const int count = 10;

/// Number of ship details.
const int details = 7;

/// The playing board.
char sea[size][size];

/// Array of the ships.
int ships[count][details];

/// Details of a ship.
enum ship_details {length, direction, x1, y1, x2, y2, health};

/// Possible directions of a ship.
enum ship_directions {horizontal, vertical};

/// Assigns '0's to each point of the sea (playing board) and prints it to console.
void create_sea()
{
	std::cout<<"   ";
	for (char i = 'a'; i < 'k'; ++ i){
    	std::cout << i << " ";
	}
	std::cout << "\n\n";
	for (int i = 1; i < size; ++ i){
		std::cout << i << "  ";
		for (int j = 0; j < size; ++ j){
			sea[i - 1][j] = '0';
			std::cout << sea[i - 1][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << 10 << " ";
	int last_row = size - 1;
	for (int j = 0; j < size; ++ j){
		sea[last_row][j] = '0';
		std::cout << sea[last_row][j] << " ";
	}
	std::cout << "\n\n";
}

/// Allows to generate random numbers.
void random_number_generator()
{
	srand(time(0));
}

/// Sets up the ships.
void set_ships()
{
	set_ship_length_health();
	set_ship_direction();
	set_ship_coordinates();
}

/// Sets up the length and deafault health of each ship.
void set_ship_length_health()
{
	int index = 0;
	int pat = 5;
	for (int i = 4; i > 0; -- i){
		for (int j = 1; j <= pat - i; ++ j){
			ships[index][length] = i;
			ships[index][health] = i;
			++ index;
		}
	}
}

/// Randomly sets the direction of the ships that are larger than 1.
/// Considering that direction doesn't make a difference for 1 sized ships, we set their directions to horizontal by default.
void set_ship_direction()
{
	int large_ships = 6;
	for (int i = 0; i < large_ships; ++ i){
		ships[i][direction] = rand() % 2;
	}
	for (int i = large_ships; i < count; ++ i){
    ships[i][direction] = 0;
  }
}

/// Sets the coordinates of the ships.
void set_ship_coordinates()
{
	first_ship_coordinates();
	for (int i = 1; i < count; ++ i){
		next_ship_coordinates(i);
	}
}

/// Randomly generates coordinates for the first ship.
void first_ship_coordinates()
{
	int index = 0;
	if (ships[index][direction] == horizontal){
		horizontal_ship_coordinates(index);
	}
	else{
		vertical_ship_coordinates(index);
	}
}

/// Randomly generates coordinates for a horizontal ship according to its length.
/// index - array index of a given ship.
void horizontal_ship_coordinates(int index)
{
	int range = size - ships[index][length] + 1;
	ships[index][x1] = rand() % range;
	ships[index][y1] = rand() % size;
	ships[index][x2] = ships[index][x1] + (ships[index][length] - 1);
	ships[index][y2] = ships[index][y1];
}

/// Randomly generates coordinates for a vertical ship according to its length.
/// index - array index of a given ship.
void vertical_ship_coordinates(int index)
{
	int range = size - ships[index][length] + 1;
	ships[index][x1] = rand() % size;
	ships[index][y1] = rand() % range;
	ships[index][x2] = ships[index][x1];
	ships[index][y2] = ships[index][y1] + (ships[index][length] - 1);
}

/// Randomly generates coordinates of a given ship different than the first one, according to the rules of the game.
/// index - array index of a given ship.
void next_ship_coordinates(int index)
{
	int clash_counter = 0;
	bool check = false;
	while (check == false){
		clash_counter = 0;
		if (ships[index][direction] == horizontal){
			horizontal_ship_coordinates(index);
		}
		else{
			vertical_ship_coordinates(index);
		}
		clash_counter = check_clashes(index);
		if (clash_counter == 0){
			check = true;
		}
	}
}

/// Checks if a given ship clashes with or positioned next to already set ships.
/// index - array index of a given ship.
int check_clashes(int index)
{
	int clash_counter=0;
	for (int i = 0; i < index; ++ i){
		clash_counter += go_through_new_ship_points(index, i);
	}
	return clash_counter;
}

/// Goes through each point of a given ship and calls the function that goes through each point of already set ships.
/// new_index - array index of the most recently created ship.
/// set_index - array index of an already set ship.
int go_through_new_ship_points(int new_index, int set_index)
{
	int clash_counter = 0;
	int xn1 = ships[new_index][x1];
	int yn1 = ships[new_index][y1];
	int xn2 = ships[new_index][x2];
	int yn2 = ships[new_index][y2];
	for (int x = xn1; x <= xn2; ++ x){
		for (int y = yn1; y <= yn2; ++ y){
			clash_counter += go_through_reserved_points(set_index, x, y);
			if (clash_counter > 0){
				return clash_counter;
			}
		}
	}
	return clash_counter;
}

/// Goes through each point of already set ships, points next to them.
/// Checks if the last generated coordinates match with any of those points.
/// set_index - array index of an already set ship.
/// new_x - x coordinate of one of the points of the most recently created ship.
/// new_y - y coordinate of one of the points of the most recently created ship.
int go_through_reserved_points(int set_index, int new_x, int new_y)
{
	int clash_counter = 0;
	int xs1 = ships[set_index][x1] - 1;
	int ys1 = ships[set_index][y1] - 1;
	int xs2 = ships[set_index][x2] + 1;
	int ys2 = ships[set_index][y2] + 1;
	for (int x = xs1; x <= xs2; ++ x){
		for (int y = ys1; y <= ys2; ++ y){
			if (new_x == x && new_y == y){
				++ clash_counter;
				return clash_counter;
			}
		}
	}
	return clash_counter;
}

/// Enables player to play the game and prints a message when the game is over.
void play()
{
	int sank_counter = 0;
	while (sank_counter < 10){
		sank_counter = 0;
		shoot();
		for (int i = 0; i < count; ++ i){
			if (ships[i][health] == 0){
				++ sank_counter;
			}
		}
	}
	std::cout << "Congratulations! You won the game!\n";
}

/// Receives coordinates from console and proceeds those.
void shoot()
{
	std::cout << "Please enter the coordinates.\n\n";
	std::string x;
	std::string y;
	int x_number = 0;
	int y_number = 0;
	std::cin >> x >> y;
	bool verify = verify_coordinates(x, y);
	if (verify == true){
		x_number = convert_x(x);
		y_number = convert_y(y);
		if (sea[y_number][x_number] == '0'){
			hit_check(x_number, y_number);
		}
		else{
			std::cout << "Already hit.\n\n";
		}
		update_sea();
	}
	else{
		std::cout << "Please enter coordinates in the right format.\n";
		std::cout << "First coordinate should be a letter from 'a' to 'j'.\n";
		std::cout << "Second coordinate should be a number from '1' to '10'.\n";
	}
}

/// Verifies if entered coordinates are in the right format.
/// x - x coordinate entered by the player.
/// y - y coordinate entered by the player.
bool verify_coordinates(std::string x, std::string y)
{
	std::string x_coordinates[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
	std::string y_coordinates[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	bool check_x = false;
	bool check_y = false;
	for (int i = 0; i < size; ++ i){
		if (x == x_coordinates[i]){
			check_x = true;
			break;
		}
	}
	for (int i = 0; i < size; ++ i){
		if (y == y_coordinates[i]){
			check_y = true;
			break;
		}
	}
	if (check_x == true && check_y == true){
		return true;
	}
	else{
		return false;
	}
}

/// Converts the 'x' coordinate from string to int.
/// x - x coordinate entered by the player.
int convert_x(std::string x)
{
	int x_number = 0;
	std::string x_coordinates[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
	for (int i = 0; i < size; ++ i){
		if (x == x_coordinates[i]){
			x_number = i;
			break;
		}
	}
	return x_number;
}

/// Converts the 'y' coordinate from string to int.
/// y - y coordinate entered by the player.
int convert_y(std::string y)
{
	int y_number = 0;
	std::string y_coordinates[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
	for (int i = 0; i < size; ++ i){
		if (y == y_coordinates[i]){
			y_number = i;
			break;
		}
	}
	return y_number;
}

/// Checks if the shot made by player hit any of the ships.
/// x - x coordinate entered by the player converted to int.
/// y - y coordinate entered by the player converted to int.
void hit_check(int x, int y)
{
	bool hit = false;
	int index = 0;
	for (int i = 0; i < count; ++ i){
		if (x >= ships[i][x1] && x <= ships[i][x2]){
			if (y >= ships[i][y1] && y <= ships[i][y2]){
				hit = true;
				index = i;
				-- ships[i][health];
				sea[y][x] = 'X';
				break;
			}
		}
	}
	if (hit == true){
		health_check(index);
	}
	else{
		std::cout << "Miss!\n\n";
		sea[y][x] = '.';
	}
}

/// Checks the health of a hit ship.
/// index - array index of a given ship.
void health_check(int index)
{
	if (ships[index][health] > 0){
		std::cout << "Hit!\n\n";
	}
	else{
		std::cout << "Sank!\n\n";
	}
}

/// Prints the updated sea to console.
void update_sea()
{
	std::cout<<"   ";
  	for (char i = 'a'; i < 'k'; ++ i){
    	std::cout << i << " ";
  	}
	std::cout << "\n\n";
	for (int i = 1; i < size; ++ i){
    	std::cout << i << "  ";
    	for (int j = 0; j < size; ++ j){
      		std::cout << sea[i - 1][j] << " ";
    	}
    	std::cout << "\n";
  	}
  	std::cout << 10 << " ";
  	int last_row = size - 1;
  	for (int j = 0; j < size; ++ j){
  		std::cout << sea[last_row][j] << " ";
  	}
  	std::cout << "\n\n";
}
