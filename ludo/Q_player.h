#ifndef Q_PLAYER_H
#define Q_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <random>
//#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <vector>

class ludo_player : public QObject {
    Q_OBJECT

private: // Variables !!!!
	cv::Mat Q_table;
	std::vector<int> pos_start_of_turn;
	std::vector<int> pos_end_of_turn;
	int dice_roll;
	/*std::vector<std::tuple<int,int,int>> player_state_action;
        std::tuple<int,int,int,int> player_state_action_previous_position;
	int player_played;
	bool update;*/

	int make_decision();
	
	// Functions !!!!
	
	int state_representation(int input[7]);	
	void current_state(int input[7] int board_position, int token);
	void get_possible_moves(int input[11], int board_position, int dice, int token);
	std::vector<std::tuple<int, int, int>> play_maker(); // player state-action interpreter
	std::tuple<int, int, int, int> e_greedy_learning();
	void update_Q_table();


public: // Functions !!!
	Q_player();
	/*double acc_reward_player1;
        int acc_reward_player2;
        int acc_reward_player3;
        int acc_reward_player4;*/

signals:
	void select_piece(int);
	void turn_complete(bool);

public slots:
	void start_turn(positions_and_dice relative);
	void post_game_analysis(std::vector<int> relative_pos);
};

#endif // Q_PLAYER_H

// List of variables needed to run the Q_player

/*
	int input[7]
	int input[11]
	int board_position
	int token
	int dice
*/


// AT THE END OF EACH TURN IT IS EXTREMELY IMPORTANT TO SET INPUT[] ARRAYS ALL ELEMENTS TO 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!
