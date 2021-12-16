#include "game_utilities.hpp"


std::mt19937 mt(01234567);


namespace game_utils{
	
	
	unsigned long long int random_int() {
		
		
		std::uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX);
		return dist(mt);

	}

    bool check_if_same_state(std::vector<std::vector<int>> state_1, std::vector<std::vector<int>> state_2) {

        if (state_1 == state_2) {
            return true;
        }

        return false;
    }

	bool check_if_same_state(int state_1[5][5], int state_2[5][5]) {

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (state_1[i][j] == state_2[i][j]) {
					continue;
				}

				else
				{
					return false;
				}
			}
		}

		return true;

	}

    bool check_if_occupied(int current_board_state[5][5], int i, int j) {

        if (current_board_state[i][j] != 0) {
            return true;
        }

        else {
            return false;
        }

    }


	bool check_if_occupied(std::vector<std::vector<int>> current_board_state, int i, int j) {

		if (current_board_state[i][j]!=0) {
			return true;
		}

		else {
			return false;
		}

	}

    std::vector<std::pair<int, int>> get_neighbors(int i, int j) {

        std::vector<std::pair<int, int>> neighbors;
        //Checking for corners and getting neigbors accordingly
        if (i > 0) {
            neighbors.emplace_back(std::make_pair(i - 1, j));
        }

        if (i < 4) {
            neighbors.emplace_back(std::make_pair(i + 1, j));

        }

        if (j > 0) {
            neighbors.emplace_back(std::make_pair(i, j - 1));
        }

        if (j < 4) {
            neighbors.emplace_back(std::make_pair(i, j + 1));
        }


        return neighbors;




    }


    std::vector<std::pair<int, int>> get_ally_neighbors(int i, int j, int board_state[5][5], int player_id) {

        std::vector<std::pair<int, int>> ally_neighbors, neighbors;

        neighbors = get_neighbors(i, j);
        for (std::size_t k = 0; k < neighbors.size(); k++) {
            if (board_state[neighbors[k].first][neighbors[k].second] == player_id) {
                
                ally_neighbors.emplace_back(std::make_pair(neighbors[k].first, neighbors[k].second));
            }
        }

        return ally_neighbors;


    }


    std::vector<std::pair<int, int>> get_ally_neighbors(int i, int j, std::vector<std::vector<int>> board_state, int player_id) {

        std::vector<std::pair<int, int>> ally_neighbors, neighbors;

        neighbors = get_neighbors(i, j);
        for (std::size_t k = 0; k < neighbors.size(); k++) {
            if (board_state[neighbors[k].first][neighbors[k].second] == player_id) {

                ally_neighbors.emplace_back(std::make_pair(neighbors[k].first, neighbors[k].second));
            }
        }

        return ally_neighbors;


    }

    std::vector<std::pair<int, int>> get_ally_group(int i, int j, int board_state[5][5], int player_id) {

        std::vector<std::pair<int, int>> ally_neighbors, allies;


        std::stack<std::pair<int, int>> ally_stack;

        ally_stack.push(std::make_pair(i, j));

        std::pair<int, int> current_piece;
        bool on_stack[5][5] = { false };
        bool on_allies[5][5] = { false };

        while (!ally_stack.empty()) {
            current_piece = ally_stack.top();
            ally_stack.pop();
            allies.emplace_back(current_piece);
            on_stack[current_piece.first][current_piece.second] = true;
            on_allies[current_piece.first][current_piece.second] = true;


            ally_neighbors.clear();
            ally_neighbors = get_ally_neighbors(current_piece.first, current_piece.second, board_state, player_id);
            
            for (std::size_t k = 0; k < ally_neighbors.size(); k++) {
                if (!on_stack[ally_neighbors[k].first][ally_neighbors[k].second] && (!on_allies[ally_neighbors[k].first][ally_neighbors[k].second])) {
                    ally_stack.push(ally_neighbors[k]);
                    on_stack[ally_neighbors[k].first][ally_neighbors[k].second] = true;
                }

            }





        }


        return allies;


    }


    std::vector<std::pair<int, int>> get_ally_group(int i, int j, std::vector<std::vector<int>> board_state, int player_id) {

        std::vector<std::pair<int, int>> ally_neighbors, allies;


        std::stack<std::pair<int, int>> ally_stack;

        ally_stack.push(std::make_pair(i, j));

        std::pair<int, int> current_piece;
        bool on_stack[5][5] = { false };
        bool on_allies[5][5] = { false };

        while (!ally_stack.empty()) {
            current_piece = ally_stack.top();
            ally_stack.pop();
            allies.emplace_back(current_piece);
            on_stack[current_piece.first][current_piece.second] = true;

            ally_neighbors.clear();
            ally_neighbors = get_ally_neighbors(current_piece.first, current_piece.second, board_state, player_id);
            on_allies[current_piece.first][current_piece.second] = true;

            for (std::size_t k = 0; k < ally_neighbors.size(); k++) {

                if (!on_stack[ally_neighbors[k].first][ally_neighbors[k].second] && (!on_allies[ally_neighbors[k].first][ally_neighbors[k].second])) {
                    ally_stack.push(ally_neighbors[k]);
                }

            }





        }


        return allies;


    }



    bool check_liberty(int i, int j, int board_state[5][5], int player_id) {

        std::vector<std::pair<int, int>> allies = get_ally_group(i, j, board_state, player_id);
        std::vector<std::pair<int, int>> ally_neigbors;
        
        for (std::size_t k = 0; k < allies.size(); k++) {
            ally_neigbors.clear();
            ally_neigbors = get_neighbors(allies[k].first, allies[k].second);
            
            for (std::size_t l = 0; l < ally_neigbors.size(); l++) {
               
                if (board_state[ally_neigbors[l].first][ally_neigbors[l].second] == 0) {
                    return true;
                }
            }


        }


        return false;



    }

    int get_ordered_liberties(std::vector<std::vector<int>> board_state, int player_id) {

        std::vector<std::pair<int, int>> neigbors, super_neighbors;
        bool liberty_flag = false;
        int f1 = 0, f2 = 0, f3 = 0;
        
        std::vector<std::vector<bool>> visited{ 5, std::vector<bool>(5) };

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                neigbors.clear();
                if (board_state[i][j] == player_id) {
                    neigbors = get_neighbors(i, j);

                    for (std::size_t l = 0; l < neigbors.size(); l++) {
                        if (board_state[neigbors[l].first][neigbors[l].second] == 0 && !visited[neigbors[l].first][neigbors[l].second]) {
                            f1++;
                            //std::cout << "first order liberty" << i << "," << j << std::endl;
                            visited[neigbors[l].first][neigbors[l].second] = true;

                        }
                        

                    }

                    
                }
            }
        }

        return f1;
        

    }

    double get_euler_number(std::vector<std::vector<int>> board_state, int player_id) {

        double Q1 = 0, Q2 = 0, Q3 = 0;
        
        
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {

              
                int piece_count = 0, empty_count = 0;
                std::vector<bool> config(4);
                //Counting Q1s at Corner
                if ((i == 0 && j == 0) || (i == 0 && j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4) && board_state[i][j] == player_id) {
                    Q1++;
                }
               

                if (i < 4 && j < 4) {
                    if (board_state[i][j] == player_id) {
                        piece_count++;
                        config[0] = true;
                    }
                    else {
                        empty_count++;
                    }

                    if (board_state[i+1][j] == player_id) {
                        piece_count++;
                        config[1] = true;
                    }
                    else {
                        empty_count++;
                    }

                    if (board_state[i][j+1] == player_id) {
                        piece_count++;
                        config[2] = true;
                    }
                    else {
                        empty_count++;
                            
                    }

                    if (board_state[i+1][j+1] == player_id) {
                        piece_count++;
                        config[3] = true;
                    }
                    else {
                        empty_count++;
                    }
                }

                if (piece_count == 2 && (config[0]==config[3]) && (config[1] == config[2])) {
                    Q2++;
                }

                if (piece_count == 1) {
                    Q1++;
                }

                if (piece_count == 3) {
                    Q3++;
                }

            }
        
        }
        //std::cout << "Q1 = " << Q1 << std::endl;
        //std::cout << "Q2 = " << Q2 << std::endl;
        //std::cout << "Q3 = " << Q3 << std::endl;

        double euler_no = (Q1 - Q3 + 2 * (Q2))/4;

        return euler_no;


    }

    bool check_liberty(int i, int j, std::vector<std::vector<int>> board_state, int player_id, std::vector<int>* ordered_liberties) {

        std::vector<std::pair<int, int>> allies = get_ally_group(i, j, board_state, player_id);
        std::vector<std::pair<int, int>> neigbors;
        bool liberty_flag = false;
        int f1 = 0, f2 = 0, f3 = 0;

               

         

        for (std::size_t k = 0; k < allies.size(); k++) {
            neigbors.clear();
            neigbors = get_neighbors(allies[k].first, allies[k].second);

            /*if (((abs(i - allies[k].first)) == 0) && ((abs(j - allies[k].second)) == 1) && !visited[allies[k].first][allies[k].second] && (board_state[allies[k].first][allies[k].second] == 0)) {
                f1++;
                std::cout << "first order liberty" << i << "," << j << std::endl;
                visited[allies[k].first][allies[k].second] = true;
            }

            else if (((abs(i - allies[k].first)) == 2) && ((abs(j - allies[k].second)) == 2) && !visited[allies[k].first][allies[k].second] && (board_state[allies[k].first][allies[k].second] == 0)) {
                f2++;
                std::cout << "second order liberty" << i << "," << j << std::endl;
                visited[allies[k].first][allies[k].second] = true;
            }

            else if (((abs(i - allies[k].first)) == 3) && ((abs(j - allies[k].second)) == 3) && !visited[allies[k].first][allies[k].second] && (board_state[allies[k].first][allies[k].second] == 0)) {
                f3++;
                std::cout << "third order liberty" << i << "," << j << std::endl;
                visited[allies[k].first][allies[k].second] = true;
            }*/
            

            for (std::size_t l = 0; l < neigbors.size(); l++) {
                
                if (board_state[neigbors[l].first][neigbors[l].second] == 0) {
                    if (ordered_liberties != nullptr) {
                        std::cout << "Yes" << std::endl;
                        
                        

                        liberty_flag = true;
                    }
                    
                    else {
                        return true;
                    }
                    
                }
            }


        }

        if (ordered_liberties != nullptr) {
            ordered_liberties->clear();
            ordered_liberties->emplace_back(f1);
            ordered_liberties->emplace_back(f2);
            ordered_liberties->emplace_back(f3);
            //std::cout << "Ordered Liberties = " << ordered_liberties->at(0) << "," << ordered_liberties->at(1) << "," << ordered_liberties->at(2) << std::endl;
        }
        
        return liberty_flag;



    }

    std::vector<std::pair<int, int>> get_killed_pieces(int piece_id, int board_state[5][5]) {
        std::vector<std::pair<int, int>> killed_pieces;


        for (std::size_t i = 0; i < 5; i++) {
            for (std::size_t j = 0; j < 5; j++) {
                if (board_state[i][j] == piece_id) {
                    if (!check_liberty(i, j, board_state, piece_id)) {
                        killed_pieces.emplace_back(std::make_pair(i, j));
                    }
                }
            }
        }

        return killed_pieces;


    }


    std::vector<std::pair<int, int>> get_killed_pieces(int piece_id, std::vector<std::vector<int>> board_state) {
        std::vector<std::pair<int, int>> killed_pieces;


        for (std::size_t i = 0; i < 5; i++) {
            for (std::size_t j = 0; j < 5; j++) {
                if (board_state[i][j] == piece_id) {
                    if (!check_liberty(i, j, board_state, piece_id)) {
                        killed_pieces.emplace_back(std::make_pair(i, j));
                    }
                }
            }
        }

        return killed_pieces;


    }

    void remove_piece_at(int i, int j, int(&board_state)[5][5]) {

        board_state[i][j] = 0;

    }


    void remove_piece_at(int i, int j, std::vector<std::vector<int>>& board_state) {
        board_state[i][j] = 0;
    }



    void remove_piece_at(std::vector<std::pair<int, int>> pieces_to_remove, int(&board_state)[5][5]) {


        for (std::size_t i = 0; i < pieces_to_remove.size(); i++) {
            remove_piece_at(pieces_to_remove[i].first, pieces_to_remove[i].second, board_state);
        }

    }

    void remove_piece_at(std::vector<std::pair<int, int>> pieces_to_remove, std::vector<std::vector<int>>& board_state) {


        for (std::size_t i = 0; i < pieces_to_remove.size(); i++) {
            remove_piece_at(pieces_to_remove[i].first, pieces_to_remove[i].second, board_state);
        }

    }



    void remove_killed_pieces(int piece_id, int (&board_state)[5][5]) {

        std::vector<std::pair<int, int>> killed_pieces = get_killed_pieces(piece_id, board_state);

        remove_piece_at(killed_pieces, board_state);
    }

    void remove_killed_pieces(int piece_id, std::vector<std::vector<int>>& board_state, int* no_of_pieces) {

        std::vector<std::pair<int, int>> killed_pieces = get_killed_pieces(piece_id, board_state);

        if (no_of_pieces != nullptr) {
            *no_of_pieces = killed_pieces.size();
        }

        remove_piece_at(killed_pieces, board_state);
    }


    bool check_if_valid(int board_state[5][5], move current_move, int player_id) {

        int future_state[5][5];
        int opponent_id = 3 - player_id;
        //Checking if the move lies within the board limits
        if ((current_move.row < 0) || (current_move.row > 4) || (current_move.col < 0) || (current_move.col > 4)) {
            return false;
        }

        //If the chosen position is already occupied
        if (board_state[current_move.row][current_move.col] != 0) {
            return false;
        }

        //Making sure for a new call of check if valid future state is current state before any update
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                future_state[i][j] = board_state[i][j];
            }
        }

        //Playing the move after it has satisfied all the above criteria
        future_state[current_move.row][current_move.col] = player_id;


        if (game_utils::check_liberty(current_move.row, current_move.col, future_state, player_id)) {
            return true;
        }


        //If No liberty
        std::vector<std::pair<int, int>> killed_opponents = game_utils::get_killed_pieces(opponent_id, future_state);
        game_utils::remove_killed_pieces(opponent_id, future_state);

        if (!game_utils::check_liberty(current_move.row, current_move.col, future_state, player_id)) {
            return false;
        }


        //Checking for KO move
        else {

            if ((killed_opponents.size() != 0) && (game_utils::check_if_same_state(board_state, future_state))) {
                return false;
            }
        }


        return true;



    }


    bool check_if_valid(std::vector<std::vector<int>> board_state, std::vector<std::vector<int>> previous_state,move current_move, int player_id) {

        std::vector<std::vector<int>> future_state;
        int opponent_id = 3 - player_id;
        //Checking if the move lies within the board limits
        if ((current_move.row < 0) || (current_move.row > 4) || (current_move.col < 0) || (current_move.col > 4)) {
            //std::cout << "returning from case 1" << std::endl;
            return false;
        }

        //If the chosen position is already occupied
        if (board_state[current_move.row][current_move.col] != 0) {
            //std::cout << "returning from case 2" << std::endl;
            return false;
        }

        //Making sure for a new call of check if valid future state is current state before any update
        future_state = board_state;

        //Playing the move after it has satisfied all the above criteria
        future_state[current_move.row][current_move.col] = player_id;


        if (game_utils::check_liberty(current_move.row, current_move.col, future_state, player_id)) {
            return true;
        }


        //If No liberty
        std::vector<std::pair<int, int>> killed_opponents = game_utils::get_killed_pieces(opponent_id, future_state);
        game_utils::remove_killed_pieces(opponent_id, future_state);

        if (!game_utils::check_liberty(current_move.row, current_move.col, future_state, player_id)) {
            //std::cout << "returning from case 3" << std::endl;
            return false;
        }


        //Checking for KO move
        else {

            if ((killed_opponents.size() != 0) && (future_state == previous_state)) {
                //std::cout << "returning from case 4" << std::endl;
                return false;
            }
        }


        return true;



    }


    
}


