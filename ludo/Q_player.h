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
#include <time.h>

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
	
	int pre_token;
	int pre_state;
	int pre_move;
	int pre_position;
	//bool update;
	
	// Added Functions !!!!
	vector<int> state();
	vector<int> get_possible_moves(int dice);
	double assign_reward(int previous_move, int previous_position, int current_position, int previous_state, int current);
	int e_greedy_move_selection(double eps, vector<int> states, vector<int> moves);
	void update_Q_table(int previous_token, int previous_move, int previous_state, int current, int previous_position, int moves);

	// Modified	
	int make_decision();

public: 
	// Class object constructor.
	Q_player();

	double acc;
        
signals:
	void select_piece(int);
	void turn_complete(bool);

public slots:
	void start_turn(positions_and_dice relative);
	void post_game_analysis(std::vector<int> relative_pos);
};

#endif // Q_PLAYER_H


