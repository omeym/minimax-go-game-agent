#include "game_data_structures.hpp"
#include "game_utilities.hpp"
#include "file_rw_utilities.hpp"


GO::GO(int n, bool training_flag) {
    player_id = 1;			//Default player is Black
    opponent_id = 2;		//Default opponent is White
    komi = 2.5;
    this->training_flag = training_flag;
    board_size = n;

    
    //file_rw::load_hist_of_moves(this->history_of_moves);
    //file_rw::load_opp_hist_of_moves(this->opponent_history_of_moves);
    //q_player_no_of_moves = history_of_moves.size();
    
    //std::cout << "No of Moves = "<<number_of_moves << std::endl;

}


double GO::minimax_ab(std::vector<std::vector<int>> current_board, std::vector<std::vector<int>> previous_board, int depth, bool is_maximizer, double minimax_alpha, double minimax_beta) {
    
    
    if(player_id == 2){
        if (no_minimax_moves == 24) {
        //std::cout<<"Returning from 1"<<std::endl;    
        terminal_state = true;
        return minimax_eval_function(current_board, previous_board,true);
        }
    }
    
    else if(player_id == 1){
        if (no_minimax_moves == 25) {
        //std::cout<<"returning from terminal"<<std::endl;
        //std::cout<<"Returning from 2"<<std::endl;
        terminal_state = true;
        return minimax_eval_function(current_board, previous_board,true);
        }    
    }

    if (current_moves <= 10) {
        
        if (depth == 2) {
            return minimax_eval_function(current_board, previous_board,false);
        }

    }
    else if(current_moves > 10 && current_moves<=15){
        if (depth == 4) {
            return minimax_eval_function(current_board, previous_board,false);
        }
    
    }

    else if(current_moves > 15 && current_moves < 24){

        if (depth == 6) {
            return minimax_eval_function(current_board, previous_board, false);
        }

    }


    
    
    std::vector<std::vector<int>> board_state = current_board;


    bool center_move_played = false;
    move current_move;
    if (is_maximizer) {

        double best = DBL_MIN;
        int killed_piece_count = 0;
        int move_code = 0;
        double move_value = 0.0;

        std::priority_queue<std::pair<int, int>> ordered_move;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                current_move.initialize_move(i, j);
                //std::cout << "i: " << current_move .row<< "j: " << current_move.col << std::endl;
                if (game_utils::check_if_valid(board_state, previous_state, current_move, player_id)) {
                    board_state[i][j] = player_id;
                    game_utils::remove_killed_pieces(opponent_id, board_state, &killed_piece_count);
                    move_code = this->encode_action(current_move);
                    ordered_move.push(std::make_pair(killed_piece_count, move_code));
                    //Refreshing the board back to original state
                    board_state = current_board;

                }

            }
        }

       

        move killer_move_temp;
        while (!ordered_move.empty())
        {

            std::pair<int, int> most_killer_move;

            most_killer_move = ordered_move.top();
            ordered_move.pop();

            killer_move_temp = decode_action(most_killer_move.second);
            //std::cout << "KIller move = " << killer_move_temp.row << "," << killer_move_temp.col << std::endl;
            //Black  is the maximizer
            board_state[killer_move_temp.row][killer_move_temp.col] = player_id;
            no_minimax_moves++;
            game_utils::remove_killed_pieces(opponent_id, board_state);
            double move_value = minimax_ab(board_state, current_board, depth + 1, false, minimax_alpha, minimax_beta);
            board_state = current_board;
            no_minimax_moves--;

            best = std::max(best, move_value);
            minimax_alpha = std::max(minimax_alpha, best);
            
            if (minimax_beta <= minimax_alpha) {
               // std::cout << "Cutoff Achieved in maximizer" << std::endl;
                break;
            }
            

           
        }

        
        

        

        return best;

    }


    else {

        double best = DBL_MAX;

        int killed_piece_count = 0;
        int move_code = 0;
        double move_value = 0.0;

        std::priority_queue<std::pair<int, int>> ordered_move;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                current_move.initialize_move(i, j);
                //std::cout << "i: " << current_move .row<< "j: " << current_move.col << std::endl;
                if (game_utils::check_if_valid(board_state, previous_state, current_move, opponent_id)) {
                    board_state[i][j] = opponent_id;
                    game_utils::remove_killed_pieces(player_id, board_state, &killed_piece_count);
                    move_code = this->encode_action(current_move);
                    ordered_move.push(std::make_pair(killed_piece_count, move_code));
                    //Refreshing the board back to original state
                    board_state = current_board;

                }

            }
        }
        


        move killer_move_temp;
        while (!ordered_move.empty())
        {

            std::pair<int, int> most_killer_move;

            most_killer_move = ordered_move.top();
            ordered_move.pop();

            killer_move_temp = decode_action(most_killer_move.second);

            //White is the minimizer
            board_state[killer_move_temp.row][killer_move_temp.col] = opponent_id;
            no_minimax_moves++;
            game_utils::remove_killed_pieces(player_id, board_state);
            double move_value = minimax_ab(board_state, current_board, depth + 1, true, minimax_alpha, minimax_beta);
            board_state = current_board;
            no_minimax_moves--;

            best = std::min(best, move_value);
            minimax_beta = std::min(minimax_beta, best);
            //Pruning for alpha and beta
            if (minimax_beta <= minimax_alpha) {
                //std::cout << "Cutoff Achieved in minimizer" << std::endl;
                break;
            }

        }






        return best;

    }



}


move GO::find_best_minimax_move(std::vector<std::vector<int>> current_board) {

    double best_score = DBL_MIN;

    move best_move, current_move;
    std::vector<std::vector<int>> board_state;

    board_state = current_board;
    //Implement Move Ordereding
    //In remove killed pieces function add to get the number of killed pieces
    //Then try to order the moves by placing them in a priority queue and then do move ordering from high to low
    //Then lookup in the transposition table to look for stored board value
    //Add variable depths for cutoff
    
    int killed_piece_count = 0;
    int move_code = 0;
    double move_value = 0.0;

    std::priority_queue<std::pair<int, int>> ordered_move;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            current_move.initialize_move(i, j);
            //std::cout << "i: " << current_move .row<< "j: " << current_move.col << std::endl;
            if (game_utils::check_if_valid(board_state, previous_state, current_move, player_id)) {
                board_state[i][j] = player_id;
                game_utils::remove_killed_pieces(opponent_id, board_state, &killed_piece_count);
                move_code = this->encode_action(current_move);
                ordered_move.push(std::make_pair(killed_piece_count,move_code));
                //Refreshing the board back to original state
                board_state = current_board;

            }

        }
    }


    move killer_move_temp;
    while (!ordered_move.empty())
    {

        std::pair<int, int> most_killer_move;

        most_killer_move = ordered_move.top();
        ordered_move.pop();
        
        killer_move_temp = decode_action(most_killer_move.second);
        
        board_state[killer_move_temp.row][killer_move_temp.col] = player_id;
        no_minimax_moves++;
        game_utils::remove_killed_pieces(opponent_id, board_state);
        
        move_value = minimax_ab(board_state, current_board, 0, false, DBL_MIN, DBL_MAX);
        

        //Refreshing the board back to original state
        board_state = current_board;
        no_minimax_moves--;
        //std::cout << "Killer Move temp = " << killer_move_temp.row << " " << killer_move_temp.col << std::endl;

        if (move_value >= best_score) {
            best_move.row = killer_move_temp.row;
            best_move.col = killer_move_temp.col;
            best_move.status = "MOVE";
            best_score = move_value;

        }


    }



    no_minimax_moves++;
    return best_move;

    

}

double GO::minimax_eval_function(std::vector<std::vector<int>> input_state, std::vector<std::vector<int>> previous_state,bool is_final) {

    std::vector<std::vector<int>> board_state;
    board_state = input_state;
    double maximizer_score = 0;
    double minimizer_score = 0;
    double empty_cells = 0;

    
    if (is_final) {

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board_state[i][j] == player_id) {

                    maximizer_score++;

                }

                else if (board_state[i][j] == opponent_id) {

                    minimizer_score++;

                }
            }
        }
        //If player is black
        if (player_id == 1) {
            if ((maximizer_score) > (minimizer_score + this->komi)) {
                //std::cout << "I will win the game" << std::endl;
                return DBL_MAX;
            }
            else {
                //std::cout << "I will lose the game" << std::endl;
                return DBL_MIN;
            }
        }

        //If player is white
        else if (player_id == 2) {
            if ((maximizer_score + this->komi) > (minimizer_score )) {
                //std::cout << "I will win the game" << std::endl;
                return DBL_MAX;
            }
            else {
                //std::cout << "I will lose the game" << std::endl;
                return DBL_MIN;
            }

        }
        
    }

    //Add Transpostion Table Line here
    

    //Checking the board score
    double number_of_liberties_player = 0;
    double number_of_liberties_opponent = 0;
    
    double Q1_p = 0.0, Q3_p = 0.0, Qd_p = 0.0;
    double Q1_o = 0.0, Q3_o = 0.0, Qd_o = 0.0;
    double euler_number_p = 0, euler_number_o = 0;
    double f1_p = 0.0, f2_p = 0.0, f3_p = 0.0;
    double f1_o = 0.0, f2_o = 0.0, f3_o = 0.0;

    double previous_minimizer_score = 0, previous_maximizer_score = 0;

    std::vector<int> maximizer_liberty;
    std::vector<int> minimizer_liberty;
    //Evaluating scores
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board_state[i][j] == player_id) {
                /*if (game_utils::check_liberty(i, j, board_state, 1, &maximizer_liberty)) {
                    f1_p += maximizer_liberty[0];
                    f2_p += maximizer_liberty[1];
                    f3_p += maximizer_liberty[2];
                    

                }*/

                maximizer_score++;
                
            }

            else if (board_state[i][j] == opponent_id) {
               /* if (game_utils::check_liberty(i, j, board_state, 2, &minimizer_liberty)) {
                    f1_o += minimizer_liberty[0];
                    f2_o += minimizer_liberty[1];
                    f3_o += minimizer_liberty[2];
                }
*/
               
                minimizer_score++;
               
            }

            else if (board_state[i][j] == 0) {
                
                empty_cells++;
            }

            ////Euler Number Computation
            //if (i < 4 && j < 4) {
            //    int empty_count = 0, maximizer_count = 0, minimizer_count = 0;
            //    std::vector<bool> config_flag(4, false);
            //    
            //    //Position 1
            //    if ((board_state[i][j] == 0)) {
            //        empty_count++;
            //        config_flag[0] = true;
            //    }
            //    else if ((board_state[i][j] == 1)) {
            //        maximizer_count++;
            //    }

            //    else if ((board_state[i][j] == 2)) {
            //        minimizer_count++;
            //    }

            //    //Position 2
            //    if ((board_state[i + 1][j] == 0)) {
            //        empty_count++;
            //        config_flag[1] = true;
            //    }

            //    else if ((board_state[i+1][j] == 1)) {
            //        maximizer_count++;
            //    }

            //    else if ((board_state[i+1][j] == 2)) {
            //        minimizer_count++;
            //    }

            //    //Position 3
            //    if ((board_state[i][j+1] == 0)) {
            //        empty_count++;
            //        config_flag[2] = true;
            //    }

            //    else if ((board_state[i][j+1] == 1)) {
            //        maximizer_count++;
            //    }

            //    else if ((board_state[i][j+1] == 2)) {
            //        minimizer_count++;
            //    }


            //    //Position 4
            //    if ((board_state[i+1][j+1] == 0)) {
            //        empty_count++;
            //        config_flag[3] = true;
            //    }

            //    else if ((board_state[i+1][j + 1] == 1)) {
            //       maximizer_count++;
            //    }

            //    else if ((board_state[i+1][j + 1] == 2)) {
            //        minimizer_count++;
            //    }

            //    //Counting the euler number
            //    if (empty_count == 3 ) {
            //        if (maximizer_count == 1 && minimizer_count == 0) {
            //            Q1_p++;
            //        }

            //        else if (maximizer_count == 0 && minimizer_count == 1) {
            //            Q1_o++;
            //        }

            //        else {
            //            //std::cout << "Unexpected case in Q1 calculations" << std::endl;
            //        }
            //       
            //    }

            //    else if (empty_count == 1) {
            //        if (maximizer_count == 3 && minimizer_count == 0) {
            //            Q3_p++;
            //        }

            //        else if (maximizer_count == 0 && minimizer_count == 3) {
            //            
            //            Q3_o++;
            //        }

            //        else if (maximizer_count == 2 && minimizer_count == 1) {
            //            

            //        }

            //        else {
            //            //std::cout << "Unexpected case in Q3 calculations" << std::endl;

            //        }
            //    }

            //    else if (empty_count == 2) {
            //        if ((config_flag[0] == config_flag[3]) && (config_flag[1] == config_flag[2])) {
            //            
            //            if (maximizer_count == 2 && minimizer_count == 0) {
            //                Qd_p++;
            //            }

            //            else if (maximizer_count == 0 && minimizer_count == 2) {
            //                Qd_o++;
            //            }

            //            else {
            //                //std::cout << "Unexpected case in Qd calculations" << std::endl;
            //            }
            //            

            //        }

            //    }


            //}


            //if (((i == 0) && (j == 0)) || ((i == 0) && (j == 4)) || ((i == 4) && (j == 0)) || ((i == 4) && (j == 4))) {


            //    if (board_state[i][j] == 1) {
            //        Q1_p++;
            //    }

            //    else if (board_state[i][j] == 2) {
            //        Q1_o++;
            //    }

            //    else {
            //        //std::cout << "Unexpected case in Q1 calculations" << std::endl;
            //    }
            //}

            //Killer Scoring
            if (previous_state[i][j] == player_id) {
                previous_maximizer_score++;
            }
            else if (previous_state[i][j] == opponent_id) {
                previous_minimizer_score++;
            }
            

        }
    }

    
    //std::cout << "Euler Number = " << (euler_number_p - euler_number_o)*4 << std::endl;
   
    double value = 0;
    //double my_liberties_val = (f1_p - f1_o) + (f2_p - f2_o) + 0.5 * (f3_p - f3_o);

    //std::cout << "Value of Liberties = " << my_liberties_val <<std::endl;
    f1_p = game_utils::get_ordered_liberties(board_state, player_id);
    f1_o = game_utils::get_ordered_liberties(board_state, opponent_id);
    double my_liberties_val = (f1_p - f1_o);
    euler_number_o = game_utils::get_euler_number(board_state,opponent_id);
    euler_number_p = game_utils::get_euler_number(board_state,player_id);

    
    if (player_id == 1) {
        

    //euler_number_o = (Q1_o - Q3_o + 2*(Qd_o));
    //euler_number_p = (Q1_p - Q3_p + 2*(Qd_p));

        
        //value = - 4*(maximizer_score - previous_maximizer_score);//- 2 * (euler_number_p - euler_number_o) + std::min(std::max(my_liberties_val, -3.0), 3.0) - (2*(current_moves/24) * this->komi);
        //Best Functtion
        value = -1.5*(this->komi) -7*(minimizer_score - previous_minimizer_score) + (maximizer_score - (minimizer_score))+std::min(std::max(my_liberties_val, -3.0), 3.0) - 2 * (euler_number_p - euler_number_o);// + std::min(std::max(my_liberties_val, -3.0), 3.0);
        //value = (maximizer_score - (minimizer_score)) - 3*(minimizer_score - previous_minimizer_score) + (maximizer_score - previous_maximizer_score) - 3*this->komi;
        //value = - 3*this->komi - 2*(minimizer_score - previous_opponent_score) + 4*(maximizer_score - previous_player_score);
        
        //std::cout << "value function for player "<<player_id<<" = " << value << std::endl;
    
    }
    
    else if (player_id == 2) {
        //value = (maximizer_score - (minimizer_score)) - 6*(minimizer_score - previous_opponent_score) + (maximizer_score - previous_player_score);// - 2 * (euler_number_p - euler_number_o) + std::min(std::max(my_liberties_val, -3.0), 3.0)+ this->komi ;
        //Best Evaluation Function
        value = (maximizer_score - (minimizer_score)) - 11*(minimizer_score - previous_minimizer_score) +std::min(std::max(my_liberties_val, -3.0), 3.0)- 2 * (euler_number_p - euler_number_o);
       //std::cout << "value function for player "<<player_id<<" = " << value << std::endl;
    
        
    }
    
    //value = (- this->komi - 2) - (euler_number_p - euler_number_o) + std::min(std::max(my_liberties_val, -3.0), 3.0);
    

    return value;
    

}



void GO::play_q_move(move played_move) {

    if (played_move.status.compare("MOVE")) {
        history_of_moves.emplace_back(std::make_pair(current_hash, encode_action(played_move)));
    }

    else {
        //25 is encoded as a pass
        history_of_moves.emplace_back(std::make_pair(current_hash, 25));
    }
    
   
    file_rw::save_move(history_of_moves[history_of_moves.size() - 1]);
    file_rw::save_move(opponent_history_of_moves[opponent_history_of_moves.size() - 1], "opp_history_of_moves.txt");



    if (check_if_terminal(played_move)) {
        this->compute_final_scores();
        if (!training_flag) {
            train_player();
        }

       
        //Need to clear the history of moves if the game has reached terminal state before starting a new game
        file_rw::clear_history();
        file_rw::clear_history("opp_history_of_moves.txt");
        return;
        
    }

    

}

void GO::compute_final_scores() {
    if (player_id == 1) {
        final_player_score = player_pieces;
        final_opponent_score = opponent_pieces + this->komi;
    }

    else if (player_id == 2) {
        final_player_score = player_pieces + this->komi;
        final_opponent_score = opponent_pieces;
    }

}


void GO::init_minimax_board() {

    this->compute_current_pieces();
    this->compute_prev_pieces();
    file_rw::load_no_of_moves(no_minimax_moves);
    this->current_moves = no_minimax_moves;

        //Checking if its players first move
        if (this->check_if_start()) {
            std::cout << "Check if start true" << std::endl;
            no_minimax_moves = 1;
            first_move = true;
        }


        else{
            //This is to show that opponent has already played
            no_minimax_moves++;
        }

    //std::cout << "No of moves = " << no_minimax_moves << std::endl;

}

void GO::init_board() {

    
    if (q_player_no_of_moves == 0) {
        this->compute_hash();
    }

    else {
        this->init_hash_values();
    }
    
    this->check_if_start();

    std::cout << current_hash << std::endl;

    if (!first_move) {
        opponent_history_of_moves.emplace_back(previous_hash, decode_opponent_move());
    }
    
    file_rw::read_black_q_table(black_q_table);
    file_rw::read_white_q_table(white_q_table);

}


void GO::init_hash_values() {

    this->previous_move = decode_opponent_move();
    move opponent_move = decode_action(previous_move);
    previous_hash = history_of_moves[history_of_moves.size()-1].first;
    
    if (opponent_move.status == "MOVE") {
        current_hash = previous_hash ^ Current_HashTable[opponent_move.row][opponent_move.col][opponent_id];
    }

    else if(opponent_move.status == "PASS"){
        //If Previous move was pass
        current_hash = previous_hash;
    }

    std::cout <<"current hash = " <<current_hash << std::endl;
    std::cout << "prev hash = " << previous_hash << std::endl;
    

}


int GO::decode_opponent_move() {

    if (!(previous_state == current_state)) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {


                if ((previous_state[i][j] == 0) && (current_state[i][j] == opponent_id)) {
                    return (5 * i + j);
                }



            }
        }

    }

    else {
        return 25;
    }
    

}


bool GO::check_if_start() {

    if (player_id == 1 && (current_state==previous_state) && (player_pieces==0) && (opponent_pieces == 0) && (previous_player_pieces == 0) && (previous_opponent_pieces == 0)) {
        std::cout << "check if start true" << std::endl;
        first_move = true;
        return true;
    }
    
   if(player_id == 2 && (player_pieces==0) && (opponent_pieces == 1) && (previous_player_pieces == 0) && (previous_opponent_pieces == 0)){
        std::cout << "check if start true" << std::endl;
        first_move = true;
        return true;
    }
  
    return false;

}

bool GO::win_with_pass() {

    if ((current_state == previous_state)) {

        if (player_id == 1) {
            if (player_pieces > (opponent_pieces + this->komi)) {
                return true;
            }
        }

        else if (player_id == 2) {
            if ((player_pieces + this->komi) > opponent_pieces) {
                return true;
            }
        }
    }

    return false;
}


void GO::compute_prev_pieces() {
    
    int player_count = 0, opponent_count = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (previous_state[i][j] == player_id) {
                player_count++;
            }

            else if (previous_state[i][j] == opponent_id) {
                opponent_count++;
            }
        }
    }

    previous_player_pieces = player_count;
    previous_opponent_pieces = opponent_count;

    return;



}




void GO::compute_current_pieces() {

    int player_count = 0, opponent_count = 0;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (current_state[i][j] == player_id) {
                player_count++;
            }

            else if (current_state[i][j] == opponent_id) {
                opponent_count++;
            }
        }
    }
    

    player_pieces = player_count;
    opponent_pieces = opponent_count;

    return;

}


void GO::init_hash_table() {

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 3; k++) {
               ZobristTable[i][j][k] =  game_utils::random_int();
               //std::cout << Current_HashTable[i][j][k] << std::endl;
            }
        }
    }

}


//This function will only be used once to initialize the zobrist hash table
void GO::save_hash_table() {

    file_rw::save_hash_table(ZobristTable);


}

//This function will always be used to load the saved hash table
void GO::load_hash_table() {

    file_rw::read_hash_table(ZobristTable);

    /*for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 3; k++) {
                std::cout << ZobristTable[i][j][k] << std::endl;
            }
        }
    }*/

}


unsigned long long int GO::compute_hash(bool update_current_hash) {

    unsigned long long int hash =  0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            
            hash ^= ZobristTable[i][j][current_state[i][j]];

        }
    }

    if (update_current_hash) {
        current_hash = hash;
    }

    return hash;
}



//Need to update this part check if terminal won't work in case when last move is not 24
bool GO::check_if_terminal(move played_move) {

    //Check if max moves are reached

    if (this->q_player_no_of_moves == this->get_max_moves()) {
        return true;
    }

    else if ((this->current_state == this->previous_state) && played_move.status == "PASS"){
        return true;
    }

    else {
        return false;
    }

    return false;

}



move GO::decode_action(int action_code) {

    move decoded_move;
    int i = 0, j = 0;
    if (action_code!= 25) {
        i = std::floor(action_code / board_size);

        j = action_code - (5 * i);

        decoded_move.status = "MOVE";
    }

    else {
        decoded_move.status = "PASS";
    }

    decoded_move.row = i;
    decoded_move.col = j;
    
    return decoded_move;
}

int GO::encode_action(move current_move) {

    int code = 0;

    if (current_move.status == "MOVE") {
        code = (board_size * current_move.row + current_move.col);
    }

    else {
        code = 25;
    }
    
    return code;

}



void GO::train_player() {

    double player_reward = 0.0, opponent_reward = 0.0;
    double q_value = 0.0;

   

    if (final_player_score > final_opponent_score) {
            
        player_reward = terminal_pos_reward;
        opponent_reward = terminal_neg_reward;

    }
    else if (final_player_score == final_opponent_score) {


    }
    else {
        player_reward = terminal_neg_reward;
        opponent_reward = terminal_pos_reward;
    }

    double max_q_value = -1;
   
    std::vector<double> new_q_values(26,0.0);
    std::vector<double> old_q_values;

    std::vector<double> new_q_values_opp(26,0.0);
    std::vector<double> old_q_values_opp;


    for (int i = history_of_moves.size() - 1; i >= 0; i--) {
        old_q_values.clear();
        old_q_values_opp.clear();

        new_q_values.assign(26,0);
        new_q_values_opp.assign(26,0);

        if (max_q_value < 0) {
            //If player is black
            if (player_id == 1) {
                auto iterator = black_q_table.find(history_of_moves[i].first);
                if (iterator == black_q_table.end()) {
                    new_q_values[history_of_moves[i].second] = player_reward;
                    black_q_table[history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    (iterator->second)[history_of_moves[i].second] = player_reward;
                }

                

               /* auto opponent_iterator = white_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == white_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second] = player_reward;
                    white_q_table[opponent_history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    (iterator->second)[opponent_history_of_moves[i].second] = player_reward;
                }*/

                max_q_value = *max_element(black_q_table.at(history_of_moves[i].first).begin(), black_q_table.at(history_of_moves[i].first).end());
                //max_q_val_opp = *max_element(white_q_table.at(opponent_history_of_moves[i].first).begin(), white_q_table.at(opponent_history_of_moves[i].first).end());


            }


            //If Player is white
            else if (player_id == 2) {
                auto iterator = white_q_table.find(history_of_moves[i].first);
                if (iterator == white_q_table.end()) {
                    new_q_values[history_of_moves[i].second] = player_reward;
                    white_q_table[history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    (iterator->second)[history_of_moves[i].second - 1] = player_reward;
                }


                /*auto opponent_iterator = black_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == black_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second] = player_reward;
                    black_q_table[opponent_history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    (opponent_iterator->second)[opponent_history_of_moves[i].second] = player_reward;
                    

                }*/

                max_q_value = *max_element(white_q_table.at(history_of_moves[i].first).begin(), white_q_table.at(history_of_moves[i].first).end());
                //max_q_val_opp = *max_element(black_q_table.at(opponent_history_of_moves[i].first).begin(), black_q_table.at(opponent_history_of_moves[i].first).end());

                

                


            }



        }

        else {

            //If player is black
            if (player_id == 1) {
                auto iterator = black_q_table.find(history_of_moves[i].first);
                if (iterator == black_q_table.end()) {
                    new_q_values[history_of_moves[i].second - 1] = (new_q_values[history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma *(max_q_value);
                    black_q_table[history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    old_q_values = iterator->second;
                    old_q_values[history_of_moves[i].second-1] = (old_q_values[history_of_moves[i].second -1] * (1 - alpha)) + alpha * gamma *(max_q_value);
                    iterator->second = old_q_values;
                }


               /* auto opponent_iterator = white_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == white_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second] = (new_q_values_opp[opponent_history_of_moves[i].second] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    white_q_table[history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    old_q_values_opp = opponent_iterator->second;
                    old_q_values_opp[opponent_history_of_moves[i].second] = (old_q_values_opp[opponent_history_of_moves[i].second] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    opponent_iterator->second = old_q_values;
                }*/

                max_q_value = *max_element(black_q_table.at(history_of_moves[i].first).begin(), black_q_table.at(history_of_moves[i].first).end());
                //max_q_val_opp = *max_element(white_q_table.at(opponent_history_of_moves[i].first).begin(), white_q_table.at(opponent_history_of_moves[i].first).end());




                
            }


            //If Player is white
            else if (player_id == 2) {
                auto iterator = white_q_table.find(history_of_moves[i].first);
                if (iterator == white_q_table.end()) {
                    new_q_values[history_of_moves[i].second-1] = (new_q_values[history_of_moves[i].second-1] * (1 - alpha)) + alpha * gamma * (max_q_value);
                    white_q_table[history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    old_q_values = iterator->second;
                    old_q_values[history_of_moves[i].second-1] = (old_q_values[history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma * (max_q_value);
                    iterator->second = old_q_values;
                }


                /*auto opponent_iterator = black_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == black_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second] = (new_q_values_opp[opponent_history_of_moves[i].second] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    black_q_table[history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    old_q_values_opp = opponent_iterator->second;
                    old_q_values_opp[opponent_history_of_moves[i].second] = (old_q_values_opp[opponent_history_of_moves[i].second] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    opponent_iterator->second = old_q_values;
                }*/
            
                max_q_value = *max_element(white_q_table.at(history_of_moves[i].first).begin(), white_q_table.at(history_of_moves[i].first).end());
                //max_q_val_opp = *max_element(black_q_table.at(opponent_history_of_moves[i].first).begin(), black_q_table.at(opponent_history_of_moves[i].first).end());

            
            
            
            }

        }

        
        
    }


    int max_q_val_opp = -1;

    for (int i = 0; i < opponent_history_of_moves.size(); i++) {
        old_q_values_opp.clear();
        new_q_values_opp.assign(26, 0);

        if (max_q_val_opp < 0) {
            //If player is black
            if (player_id == 1) {



                auto opponent_iterator = white_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == white_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second - 1] = player_reward;
                    white_q_table[opponent_history_of_moves[i].first] = new_q_values;
                }

                else
                {
                    (opponent_iterator->second)[opponent_history_of_moves[i].second - 1] = player_reward;
                }

                max_q_val_opp = *max_element(white_q_table.at(opponent_history_of_moves[i].first).begin(), white_q_table.at(opponent_history_of_moves[i].first).end());


            }


            //If Player is white
            else if (player_id == 2) {



                auto opponent_iterator = black_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == black_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second - 1] = player_reward;
                    black_q_table[opponent_history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    (opponent_iterator->second)[opponent_history_of_moves[i].second - 1] = player_reward;


                }

                //max_q_value = *max_element(white_q_table.at(history_of_moves[i].first).begin(), white_q_table.at(history_of_moves[i].first).end());
                max_q_val_opp = *max_element(black_q_table.at(opponent_history_of_moves[i].first).begin(), black_q_table.at(opponent_history_of_moves[i].first).end());






            }



        }

        else {

            if (player_id == 1) {

                auto opponent_iterator = white_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == white_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second - 1] = (new_q_values_opp[opponent_history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    white_q_table[history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    old_q_values_opp = opponent_iterator->second;
                    old_q_values_opp[opponent_history_of_moves[i].second - 1] = (old_q_values_opp[opponent_history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    opponent_iterator->second = old_q_values;
                }

                //max_q_value = *max_element(black_q_table.at(history_of_moves[i].first).begin(), black_q_table.at(history_of_moves[i].first).end());
                max_q_val_opp = *max_element(white_q_table.at(opponent_history_of_moves[i].first).begin(), white_q_table.at(opponent_history_of_moves[i].first).end());






            }

            //If Player is white
            else if (player_id == 2) {



                auto opponent_iterator = black_q_table.find(opponent_history_of_moves[i].first);
                if (opponent_iterator == black_q_table.end()) {
                    new_q_values_opp[opponent_history_of_moves[i].second - 1] = (new_q_values_opp[opponent_history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    black_q_table[history_of_moves[i].first] = new_q_values_opp;
                }

                else
                {
                    old_q_values_opp = opponent_iterator->second;
                    old_q_values_opp[opponent_history_of_moves[i].second - 1] = (old_q_values_opp[opponent_history_of_moves[i].second - 1] * (1 - alpha)) + alpha * gamma * (max_q_val_opp);
                    opponent_iterator->second = old_q_values;
                }

                //max_q_value = *max_element(white_q_table.at(history_of_moves[i].first).begin(), white_q_table.at(history_of_moves[i].first).end());
                max_q_val_opp = *max_element(black_q_table.at(opponent_history_of_moves[i].first).begin(), black_q_table.at(opponent_history_of_moves[i].first).end());




            }


        }





        
    }

    file_rw::write_black_q_table(black_q_table);
    file_rw::write_white_q_table(white_q_table);




}



