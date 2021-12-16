///****************************AUTHOR**************************///
//Student Name: Omey Mohan Manyar
//USC Email ID: manyar@usc.edu
///***********************************************************//


#ifndef READ_WRITE_UTILITIES
#define	READ_WRITE_UTILITIES


#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <unordered_map>

#include "game_data_structures.hpp"


namespace file_rw {

	//This function will read the input file name
	void read_txt(GO& current_input, const std::string& filename = "input.txt");

	//This function will output the appropriate solution file
	void write_txt(move& current_move, const std::string& filename = "output.txt");

	//This function will delete any output file in current directory
	void delete_txt_file(const std::string& filename = "output.txt");


	//This function will read the zobrist table values
	void read_hash_table(unsigned long long int(&& hash_table)[5][5][3], const std::string& filename = "hash_table.txt");
	//This function will read the zobrist table values
	void read_hash_table(std::vector<std::vector<std::vector<unsigned long long int>>>& hash_table, const std::string& filename = "hash_table.txt");



	//This function will save the zobrist hash values
	void save_hash_table(unsigned long long int hash_table[5][5][3], const std::string& filename = "hash_table.txt");
	void save_hash_table(std::vector<std::vector<std::vector<unsigned long long int>>>& hash_table, const std::string& filename = "hash_table.txt");

	void save_move(std::pair<unsigned long long int, int>, const std::string& filename = "history_of_moves.txt");

	void load_hist_of_moves(std::vector<std::pair<unsigned long long int, int>>& history_of_moves, const std::string& filename = "history_of_moves.txt");
	
	void load_opp_hist_of_moves(std::vector<std::pair<unsigned long long int, int>>& history_of_moves, const std::string& filename = "opp_history_of_moves.txt");

	bool file_is_empty(std::ifstream& pFile);

	void clear_history(const std::string& filename = "history_of_moves.txt");

	void read_white_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& white_q_table, const std::string& filename = "white_q_table.txt");
	void write_white_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& white_q_table, const std::string& filename = "white_q_table.txt");



	void read_black_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& black_q_table, const std::string& filename = "black_q_table.txt");
	void write_black_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& black_q_table, const std::string& filename = "black_q_table.txt");
	
	void save_no_of_moves(int no_of_moves, const std::string& filename = "alpha_beta_moves.txt");

	void load_no_of_moves(int& no_of_moves, const std::string& filename = "alpha_beta_moves.txt");

	void clear_minimax_history(const std::string& filename = "alpha_beta_moves.txt");


}







#endif