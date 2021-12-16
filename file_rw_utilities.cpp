#include "file_rw_utilities.hpp"



namespace file_rw {


	void read_txt(GO& my_player, const std::string& filename){

		std::ifstream read_file_handle(filename.c_str());
		std::string filedata_buffer;

		
		std::string input_line;
		const char delimeter = ' ';	//Space is used as a delimeter in txt file

		std::vector<std::string> current_line;

		int number_of_lines = 0;

		//std::cout << "The size of current state = " << my_player.current_state.size() << my_player.current_state[0].size() << std::endl;


		if (read_file_handle.is_open()) {

			while (std::getline(read_file_handle, input_line)) {

				if (number_of_lines == 0) {
					my_player.player_id = std::stoi(input_line);
					//If the player is white
					if (my_player.player_id == 2) {
						my_player.opponent_id = 1;
						//my_player.komi = 0;
					}

					
					//For black we keep the komi as 0 itself
				}

				else if ((number_of_lines > 0) && (number_of_lines < 6)){

					
					for (int i = 0; i < 5; i++) {
						my_player.previous_state[number_of_lines - 1][i] = int(input_line[i]) - 48;
					}
										

				}

				else if((number_of_lines >= 6)){

					for (int i = 0; i < 5; i++) {
						my_player.current_state[number_of_lines - 6][i] = int(input_line[i]) - 48;
						my_player.future_state[number_of_lines - 6][i] = int(input_line[i]) - 48;
					}


				}

				number_of_lines++;
			}

		}
		



	}


	void write_txt(move& current_move, const std::string& filename) {

		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);


		if (output_file_handle.is_open()) {

			if (current_move.status.compare("PASS") == 0) {
				output_file_handle << current_move.status;
				std::cout << "PASS" << std::endl;

			}

			else if (current_move.status.compare("MOVE") == 0) {
				output_file_handle << current_move.row << "," << current_move.col;
			}

			output_file_handle.close();
			return;


		}

	}


	void delete_txt_file(const std::string& filename) {

		if (std::remove(filename.c_str()) == 0) {
			std::cout << "Successfully deleted the file with name: " << filename << std::endl;
		}

		else {
			std::cout << "No Such File or Directory Exists to delete, Please Check Filename: " << filename << std::endl;
		}
	}

	//This function will read the zobrist table values
	void read_hash_table(std::vector<std::vector<std::vector<unsigned long long int>>>& hash_table, const std::string& filename) {
		std::ifstream read_file_handle(filename.c_str());

		std::string input_line;



		if (read_file_handle.is_open()) {


			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					for (int k = 0; k < 3; k++) {
						std::getline(read_file_handle, input_line);
						hash_table[i][j][k] = std::stoull(input_line);

					}
				}
			}



		}
	}




	//This function will read the zobrist table values
	void read_hash_table(unsigned long long int (&&hash_table)[5][5][3], const std::string& filename) {

		
		std::ifstream read_file_handle(filename.c_str());
		
		std::string input_line;
		
		

		if (read_file_handle.is_open()) {

			
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						for (int k = 0; k < 3; k++) {
							std::getline(read_file_handle, input_line);
							hash_table[i][j][k] = std::stoull(input_line);
							
						}
					}
				}

			
			
		}
	}


	void save_hash_table(std::vector<std::vector<std::vector<unsigned long long int>>>& hash_table, const std::string& filename) {

		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 3; k++) {
					if ((i == 4 && j == 4 && k == 2)) {
						output_file_handle << hash_table[i][j][k];
					}

					else {
						output_file_handle << hash_table[i][j][k] << "\n";
					}


				}
			}
		}


		output_file_handle.close();


	}

	//This function will save the zobrist table values
	void save_hash_table(unsigned long long int hash_table[5][5][3], const std::string& filename) {

		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 3; k++) {
					if ((i == 4 && j == 4 && k == 2)) {
						output_file_handle << hash_table[i][j][k];
					}

					else {
						output_file_handle << hash_table[i][j][k]<<"\n";
					}
					

				}
			}
		}




	}


	void save_no_of_moves(int no_of_moves, const std::string& filename) {
		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);

		output_file_handle << no_of_moves;


	}

	void load_no_of_moves(int& no_of_moves, const std::string& filename) {
		std::ifstream input_file_handle;
		input_file_handle.open(filename, std::ios::in);
		std::string input_line;
	

		if (input_file_handle.is_open()) {
			int c = input_file_handle.peek();
			input_file_handle.seekg(0, std::ios_base::beg);
			//std::cout << "Value of c = " << c << std::endl;
			if (c == std::ifstream::traits_type::eof()) {
				//std::cout << "returning from eof condition for no_moves = 0" << std::endl;
				no_of_moves = 0;
			}

			else {
				std::getline(input_file_handle, input_line);
				
				no_of_moves = std::stoi(input_line);
				//std::cout << no_of_moves << std::endl;

			}

		}

		
		
	}


	void load_hist_of_moves(std::vector<std::pair<unsigned long long int, int>>& history_of_moves, const std::string& filename) {

		std::ifstream input_file_handle;
		input_file_handle.open(filename, std::ifstream::in);
		std::string input_line, current_line;
		int element_count = 0;
		unsigned long long int board_state = 0;
		int move = 0;
		char* p_end;


		if (input_file_handle.is_open()) {
			if (!file_is_empty(input_file_handle)) {

				while (std::getline(input_file_handle, input_line)) {
					element_count = 0;
					std::stringstream current_buffer(input_line);
					while (std::getline(current_buffer, current_line, ' ')) {
						if (element_count == 0) {
							board_state = std::strtoull(&current_line[0], &p_end, 10);
						}

						else if (element_count == 1) {
							move = std::stoi(current_line);
						}

						element_count++;
						
					}

					history_of_moves.emplace_back(std::make_pair(board_state, move));
				
				}


			}



		}

		

		input_file_handle.close();
		return;

	}

	void clear_minimax_history(const std::string& filename) {
		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);
		output_file_handle.close();

	}




	void load_opp_hist_of_moves(std::vector<std::pair<unsigned long long int, int>>& history_of_moves, const std::string& filename) {

		load_hist_of_moves(history_of_moves, filename);

	}



	void save_move(std::pair<unsigned long long int,int> current_action, const std::string& filename) {
		std::ofstream output_stream;
		output_stream.open(filename, std::ios_base::app);

		std::cout << current_action.first << " " << current_action.second << std::endl;
		output_stream << current_action.first << " " << current_action.second << std::endl;

		output_stream.close();

		return;

	}


	bool file_is_empty(std::ifstream& pFile)
	{
		return pFile.peek() == std::ifstream::traits_type::eof();
	}


	void clear_history(const std::string& filename) {

		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);
		output_file_handle.close();

	}



	void read_white_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& white_q_table, const std::string& filename)
	{

		std::ifstream input_file_handle;
		input_file_handle.open(filename, std::ifstream::in);
		std::string input_line, current_word;
		int element_count = 0;
		unsigned long long int board_state = 0;
		int move = 0;
		char* p_end;
		std::vector<std::string> current_line;
		std::vector<double> q_values;
		if (input_file_handle.is_open()) {
			if (!file_is_empty(input_file_handle)) {

				while (std::getline(input_file_handle, input_line)) {
					element_count = 0;
					q_values.clear();
					current_line.clear();
					std::stringstream current_buffer(input_line);
					
					while (std::getline(current_buffer, current_word, ' ')) {
						
						current_line.emplace_back(current_word);
					}
					
					for(int i = 1; i< current_line.size(); i++){
						
						q_values.emplace_back(std::stod(current_line[i]));
						

					}
					
					white_q_table[std::strtoull(&current_line[0][0], &p_end, 10)] = q_values;

				}


			}



		}



		input_file_handle.close();
		return;

		

	}
	void write_white_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& white_q_table, const std::string& filename) {


		std::ofstream output_file_handle;
		output_file_handle.open(filename, std::ios::trunc);
		std::unordered_map<unsigned long long int, std::vector<double>>::iterator itr;

		for (itr = white_q_table.begin(); itr != white_q_table.end(); itr++) {
			output_file_handle << itr->first << " " <<itr->second[0];
			for (size_t i = 1; i < itr->second.size(); i++) {
				output_file_handle << " " << itr->second[i];
			}
			output_file_handle << std::endl;
		}

		

		output_file_handle.close();

	}



	void read_black_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& black_q_table, const std::string& filename) {

		std::ifstream input_file_handle;
		input_file_handle.open(filename, std::ifstream::in);
		std::string input_line, current_word;
		int element_count = 0;
		unsigned long long int board_state = 0;
		int move = 0;
		char* p_end;
		std::vector<std::string> current_line;
		std::vector<double> q_values;
		if (input_file_handle.is_open()) {
			if (!file_is_empty(input_file_handle)) {

				while (std::getline(input_file_handle, input_line)) {
					element_count = 0;
					q_values.clear();
					current_line.clear();
					std::stringstream current_buffer(input_line);

					while (std::getline(current_buffer, current_word, ' ')) {

						current_line.emplace_back(current_word);
					}

					for (int i = 1; i < current_line.size(); i++) {

						q_values.emplace_back(std::stod(current_line[i]));


					}
					
					black_q_table[std::strtoull(&current_line[0][0], &p_end, 10)] = q_values;

				}


			}



		}



		input_file_handle.close();
		return;


	}


	void write_black_q_table(std::unordered_map<unsigned long long int, std::vector<double>>& black_q_table, const std::string& filename) {

		write_white_q_table(black_q_table, filename);
	}






}