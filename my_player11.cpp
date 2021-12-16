#include "file_rw_utilities.hpp"
#include "game_data_structures.hpp"
#include "game_utilities.hpp"
#include <chrono>



int main() {

	std::cout << "Game On!" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start, end;


	//Initializing the Current Game Status
	GO my_player(5, true);

	start = std::chrono::system_clock::now();

	//This function is initializing the player too
	//Reading the input values
	file_rw::read_txt(my_player);



	//Checking Input
	std::cout << "The player id = " << my_player.player_id << std::endl;

	std::cout << "\nPrevious State of the Board = " << std::endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			std::cout << my_player.previous_state[i][j];
		}

		std::cout << "\n";
	}

	std::cout << "\nCurrent State of the Board = " << std::endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			std::cout << my_player.current_state[i][j];
		}

		std::cout << "\n";
	}

	

	//Loading the zobrist hash table values used to compute the hash value of a board state
	my_player.load_hash_table();
	//Need to load Q tables here

	//Initializing the board
	my_player.init_minimax_board();
	
	//This move is mainly done for any moves which are less than 5 for special case when 
	//my player is about to win by just passing
	move initial_move;

	//Checking for easy move
	/*if (my_player.win_with_pass()) {
		std::cout << "Winning by Pass" << std::endl;
		initial_move.status = "PASS";
		file_rw::write_txt(initial_move);
		file_rw::clear_minimax_history();
		return 0;
	}*/


	

	//Move ordering to optimize for time
	//If First move always 2,2 is played
	if (my_player.check_if_start()) {
        file_rw::clear_minimax_history();
		std::cout << "First move" << std::endl;
		if(my_player.current_state[2][2] == 0){
            initial_move.initialize_move(2, 2);
            file_rw::write_txt(initial_move);
		    my_player.no_minimax_moves++;
		    file_rw::save_no_of_moves(my_player.no_minimax_moves);
		    return 0;
        }
            
		
	}

	////If its the first 6 moves play in the central region
	if (my_player.no_minimax_moves < 6) {
		std::cout << "2,2 will be played" << std::endl;
		initial_move.initialize_move(2, 2);
		if (game_utils::check_if_valid(my_player.current_state, my_player.previous_state, initial_move, my_player.player_id)) {
			file_rw::write_txt(initial_move);
			my_player.no_minimax_moves++;
			file_rw::save_no_of_moves(my_player.no_minimax_moves);
			return 0;

		}

	//	//Playing in the center initially
		for (int i = 1; i < 4; i++) {
			for (int j = 1; j < 4; j++) {
				initial_move.initialize_move(i, j);
				if (game_utils::check_if_valid(my_player.current_state, my_player.previous_state, initial_move, my_player.player_id)) {
					file_rw::write_txt(initial_move);
					my_player.no_minimax_moves++;
					file_rw::save_no_of_moves(my_player.no_minimax_moves);
					return 0;

				}


			}
		}

	}

	
	
	//std::cout << "minimax will play with " << my_player.no_minimax_moves << " no of moves" << std::endl;

	move played_move = my_player.find_best_minimax_move(my_player.current_state);
	my_player.minimax_eval_function(my_player.current_state, my_player.previous_state, false);


	std::cout << "Played Move = " << played_move.row << "," << played_move.col << std::endl;
	//played_move.status = "MOVE";
	//my_player.no_minimax_moves++;
	if (my_player.no_minimax_moves == 24) {
		file_rw::clear_minimax_history();
	}
	else {
		file_rw::save_no_of_moves(my_player.no_minimax_moves);
	}
	

	file_rw::write_txt(played_move);

	end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "elapsed time for making the move: " << elapsed_seconds.count() << "s\n";



	return 0;

}