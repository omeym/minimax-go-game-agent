///****************************AUTHOR**************************///
//Student Name: Omey Mohan Manyar
//USC Email ID: manyar@usc.edu
///***********************************************************//


#ifndef GAME_DATASTRUCTURES
#define	GAME_DATASTRUCTURES


#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <random>
#include <climits>
#include <algorithm>
#include <float.h>
#include <queue>

class move {
public:

	int row;
	int col;
	std::string status;


	move() {
		row = -1;
		col = -1;
		status = "PASS";
	}

	void initialize_move(int row, int col){
		this->row = row;
		this->col = col;
		status = "MOVE";
	}


};


class GO{
public:

	GO(int n, bool training_flag = false);

	//board size
	int board_size;

	bool terminal_state;
	//Training flag
	bool training_flag;

	//Number of current moves
	int current_moves = 0;

	//Assignning the komi values
	double komi;

	//Player ID
	int player_id;			//This value is 1 for Black Player and 2 for White Player

	//Opponent ID
	int opponent_id;

	//Number of moves played (Max is 24 in this case) used for training
	int q_player_no_of_moves = 0;
	
	//Keeping a track of scores towards the end
	double previous_player_score = 0;
	double previous_opponent_score = 0;

	double final_player_score = 0;
	double final_opponent_score = 0;

	//Previous move of the opponent (0-25)
	int previous_move;
	

	//The number of pieces on the current and previous board
	double player_pieces = 0.0, opponent_pieces = 0;
	double previous_player_pieces = 0.0, previous_opponent_pieces = 0;
	

	//Board Configurations
	std::vector<std::vector<int>> previous_state{ 5, std::vector<int>(5) }, current_state{ 5, std::vector<int>(5) }, future_state{ 5, std::vector<int>(5) };

	//Vector containing the coordinates that have the pieces which were removed
	std::vector<std::pair<int, int>> killed_pieces;


	//This will have the entire history of moves
	std::vector<std::pair<unsigned long long int, int>> history_of_moves;

	std::vector<std::pair<unsigned long long int, int>> opponent_history_of_moves;
	
	//Get max allowed moves
	int get_max_moves() { return max_moves; }

	//Initialize the board
	void init_board();

	//Playing a move and updating the game status
	void play_q_move(move played_move);


	

	//Check the previous board score
	void compute_prev_pieces();
	


	//Check the current board score
	void compute_current_pieces();

	void compute_final_scores();


	
	//This function will only be used once to initialize the zobrist hash table
	void save_hash_table();

	//This function will always be used to load the saved hash table
	void load_hash_table();


	void init_hash_table();

	unsigned long long int compute_hash(bool update_current_hash = false);

	unsigned long long int Current_HashTable[5][5][3] = { {{0} }};

	std::vector<std::vector<std::vector<unsigned long long int>>> ZobristTable{ 5, std::vector< std::vector<unsigned long long int>>(5,std::vector<unsigned long long int>(3) ) };


	unsigned long long int current_hash = 0, previous_hash = 0;

	void init_hash_values();

	bool check_if_terminal(move played_move);


	bool win_with_pass();


	//Determines whether this is the absolute start of the game
	bool check_if_start();

	//Decode Opponent's move
	int decode_opponent_move();

	void train_player();

	//Q tables
	std::unordered_map<unsigned long long int, std::vector<double>> black_q_table, white_q_table;


	//Alpha Beta Transposition Table
	std::unordered_map<unsigned long long int, double> transposition_table;

	move decode_action(int action_code);

	int encode_action(move current_move);

	bool first_move = false;


	double minimax_ab(std::vector<std::vector<int>> current_board, std::vector<std::vector<int>> previous_board,int depth, bool is_maximizer, double minimax_alpha, double minimax_beta);

	move find_best_minimax_move(std::vector<std::vector<int>> board_state);


	double minimax_eval_function(std::vector<std::vector<int>> input_state, std::vector<std::vector<int>> previous_state,bool is_final);

	int no_minimax_moves;

	void init_minimax_board();



private:

	//Maximum number of allowed moves
	int max_moves = 24;

	//Rewards
	double terminal_pos_reward = 10.0;
	double terminal_neg_reward = -10.0;


	//Learning Rate
	double alpha = 0.5;

	//Discount factor
	double gamma = 0.7;


	
	
	

};




#endif