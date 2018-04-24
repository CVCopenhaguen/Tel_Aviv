// Tools of AI - SDU Robotics
// Intelligent LUDO player using Q-Learning
// Carlos Viescas Huerta


#include "Q_player.h"


using namespace std;



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


//-----------------------------------------------------------------



// 1. Split the game into discrete set of States & Actions

/*  		STATES 						ACTIONS

	1. Home						1. Home breakout
	2. On free spot					2. No possible move
	3. On star					3. Regular move
	4. On globe					4. Move to star spot
	5. The wall					5. Move to globe 
	6. 'Red Carpet'					6. Form a wall with a homie
	7. Goal						7. Enter the 'Red Carpet'
							8. Commit suicide (When an opponent is on a globe spot)
							9. Kill rival
							10. Move to goal using a star
							11. Arrive to goal
*/


//-------------------------------------------------------------------


// 2. Create Q-table


/*

			  | 1. Home breakout | 2. No p. move | 3. reg. move | 4. mv to star | 5. mv to globe | 6. Wall | 7. Red Carpet | 8. Suicide | 9. Kill | 10. Star to goal | 11. goal |
     _____________________|__________________|_______________|______________|_______________|________________|_________|_______________|____________|_________|__________________|__________|___
			  |
	1. Home		  |				
	2. On free spot	  |				
	3. On star	  |				
	4. On globe	  |				
	5. The wall	  |				
	6. 'Red Carpet'	  |				
	7. Goal	          |
     _____________________|_____________________________________________________________________________________________________________________________________________________________________

*/



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	// This already came (ludo_player.cpp)


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

void Q_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void Q_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
