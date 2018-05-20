#ifndef Q_PLAYER_H
#define Q_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <random>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;
using namespace cv;

class Q_player : public QObject {
    Q_OBJECT

private: 
	// Global Variables !!!!
	cv::Mat Q_table;
	vector<int> pos_start_of_turn;
	vector<int> pos_end_of_turn;
	int dice_roll;
	
	vector<tuple<int,int,int>> player_state_action;
	tuple<int,int,int,int> player_state_action_previous_position;
	int player_played;
	//bool update;
	
	// Functions !!!!
	int state_representation(int states[7]);	
	void current_state(int states[7], int board_position, int token);
	void get_possible_moves(int actions[11], int board_position, int dice, int token);
	vector<tuple<int, int, int>> play_maker(int states[7], int actions[11], int token); // player state-action interpreter
	tuple<int, int, int, int> e_greedy(double eps);
	void update_Q_table(tuple<int,int,int,int> player_state_action_i);

	// Already came	
	int make_decision();

public: 
	// Class object constructor.
	Q_player();

	double acc_reward_player1;
        int acc_reward_player2;
        int acc_reward_player3;
        int acc_reward_player4;

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
