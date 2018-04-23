// Tools of AI - SDU Robotics
// Intelligent LUDO player using Q-Learning
// Carlos Viescas Huerta

/*

	Q-LEARNING ALGORITHM:
	
		1. Split the game into discrete set of States & Actions.
		2. Create Q-Table.
		3. State representation.
		4. Define rewards.
		5. Update Q-values using the learning formula:
			delta_Q(s,a) = alpha * ( r + gamma * max[a1]Q(s1, a1) - Q(s,a) )
		6. Action Selection.

*/


#ifndef Q_PLAYER_H
#define Q_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <random>

class Q_player : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
public:
    Q_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // Q_PLAYER_H
