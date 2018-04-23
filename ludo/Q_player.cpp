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



#include "Q_player.h"


Q_player::Q_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
}

int ludo_player::make_decision(){
    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

void ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
