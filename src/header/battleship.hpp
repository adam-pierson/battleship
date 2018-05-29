#include <string>

void create_sea();
void random_number_generator();
void set_ships();
void set_ship_length_health();
void set_ship_direction();
void set_ship_coordinates();
void first_ship_coordinates();
void next_ship_coordinates(int);
void horizontal_ship_coordinates(int);
void vertical_ship_coordinates(int);
int check_clashes(int);
int go_through_new_ship_points(int, int);
int go_through_reserved_points(int, int, int);
void play();
void shoot();
bool verify_coordinates(std::string, std::string);
int convert_x(std::string);
int convert_y(std::string);
void hit_check(int, int);
void health_check(int);
void update_sea();