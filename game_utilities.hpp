///****************************AUTHOR**************************///
//Student Name: Omey Mohan Manyar
//USC Email ID: manyar@usc.edu
///***********************************************************//


#ifndef GAME_UTILITIES
#define	GAME_UTILITIES


#include <stdio.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>


#include "game_data_structures.hpp"



namespace game_utils {


	unsigned long long int random_int();

	bool check_if_same_state(int state_1[5][5], int state_2[5][5]);

	//Checks if there is liberty for all stones
	bool check_liberty(int i, int j, int board_state[5][5], int player_id);

	//Get Neigbors at a particular location i,j
	std::vector<std::pair<int, int>> get_neighbors(int i, int j);

	//Get Ally Neigbors at a particular location i,j
	std::vector<std::pair<int, int>> get_ally_neighbors(int i, int j, int board_state[5][5], int player_id);

	//Get Allies at a particular location i,j across the whole board
	std::vector<std::pair<int, int>> get_ally_group(int i, int j, int board_state[5][5], int player_id);


	//Remove a piece at a particular location
	void remove_piece_at(int i, int j, int(&board_state)[5][5]);


	//Remove multiple pieces at once
	void remove_piece_at(std::vector<std::pair<int, int>> pieces_to_remove, int(&board_state)[5][5]);


	//Get Your Pieces that have died
	std::vector<std::pair<int, int>> get_killed_pieces(int piece_id, int board_state[5][5]);



	//Remove pieces that have died
	void remove_killed_pieces(int piece_id, int (&board_state)[5][5]);


	//Check the validity of the move
	bool check_if_valid(int board_state[5][5] , move current_move, int player_id);





	bool check_if_same_state(std::vector<std::vector<int>> state_1, std::vector<std::vector<int>> state_2);

	bool check_if_occupied(std::vector<std::vector<int>> current_board_state, int i, int j);

	std::vector<std::pair<int, int>> get_ally_group(int i, int j, std::vector<std::vector<int>> board_state, int player_id);



	bool check_liberty(int i, int j, std::vector<std::vector<int>> board_state, int player_id, std::vector<int>* ordered_liberties = nullptr);


	std::vector<std::pair<int, int>> get_killed_pieces(int piece_id, std::vector<std::vector<int>> board_state);


	void remove_piece_at(int i, int j, std::vector<std::vector<int>>& board_state);


	void remove_piece_at(std::vector<std::pair<int, int>> pieces_to_remove, std::vector<std::vector<int>>& board_state);

	bool check_if_valid(std::vector<std::vector<int>> board_state, std::vector<std::vector<int>> previous_state, move current_move, int player_id);
	void remove_killed_pieces(int piece_id, std::vector<std::vector<int>>& board_state, int* no_of_pieces = nullptr);
	int get_ordered_liberties(std::vector<std::vector<int>> board_state, int player_id);

	double get_euler_number(std::vector<std::vector<int>> board_state, int player_id);
}




#endif