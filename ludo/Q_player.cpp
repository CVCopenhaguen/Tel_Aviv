// Tools of AI - SDU Robotics
// Intelligent LUDO player using Q-Learning
// Carlos Viescas Huerta


#include "Q_player.h"

using namespace std;

double acc = 0;


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
	5. Shield wall					5. Move to globe 
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
	5. Shield wall	  |				
	6. 'Red Carpet'	  |				
	7. Goal	          |
     _____________________|_____________________________________________________________________________________________________________________________________________________________________

*/



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// Q_player object constructor.
Q_player::Q_player()//: pos_start_of_turn(16), pos_end_of_turn(16), dice_roll(0)
{
	// Load and initialize Q-table.
	FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::READ);
	fs["Q_table"] >> Q_table;
	fs.release();
	// Display Q-table.
	cout << "Q-Learning Table (rows = " << Q_table.rows << ", columns = " << Q_table.cols << "):" << endl;
	cout << Q_table << endl;
	
}// Q_player()



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// 3. STATE REPRESENTATION


// Representation of states. Binary state-indicator:
/*

	1. Home 	= 1 0 0 0 0 0 0
	2. On free spot = 0 1 0 0 0 0 0
	3. On star	= 0 0 1 0 0 0 0
	4. On globe	= 0 0 0 1 0 0 0
	5. Shield wall	= 0 0 0 0 1 0 0 
	6. Red carpet	= 0 0 0 0 0 1 0
	7. On goal	= 0 0 0 0 0 0 1

*/
int Q_player::state_representation(int input[7])
{
    if(input[0]  == 1)
    {
        cout << "In Home" << endl;
        return 0;
    }
    else if(input[1]  == 1)
    {
        cout << "On free spot" << endl;
        return 1;
    }
    else if(input[2]  == 1)
    {
        cout << "On star" << endl;
        return 2;
    }
    else if(input[3]  == 1)
    {
        cout << "On globe" << endl;
        return 3;
    }
    else if(input[4]  == 1)
    {
        cout << "Forming a Shield Wall" << endl;
        return 4;
    }
    else if(input[5]  == 1)
    {
        cout << "On the Red carpet" << endl;
        return 5;
    }
    else if(input[6]  == 1)
    {
        cout << "On Goal" << endl;
        return 6;
    } 

}// state_representation()




// This function calculates the current state for each token the player has. Requires board spot number.
// Board positions:
/*
	Home: -1
	Stars: 5, 18, 31, 44, 11, 24, 37, 50 
	Globes: 0, 8, 13, 21, 26, 34, 39, 47
	Red Carpet: from 50 to 56
		
*/
void Q_player::current_state(int input[7] int board_position, int token)
{
	// Check if we are in home
    	if(board_position == -1)
    	{

        	input[0] = 1;
        	cout << "Token in home" << endl;

    	} // if -1

	// Check if we are in a star
	if(board_position == 5 || board_position == 18 || board_position == 31 || board_position == 44 || board_position == 11 || board_position == 24 || board_position == 37 || board_position == 50 )
    	{
        	input[2] = 1;
        	cout << "Token on Star. Board position: #" << board_position << endl;

    	} // if star

	// Check if we are a globe
	if(board_position == 8 || board_position == 13 || board_position == 21 || board_position == 26 || board_position == 34 || board_position == 39 || board_position == 47 || board_position == 0)
    	{
        	input[3] = 1;
        	cout << "Token on globe. Board position: #" << board_position << endl;

    	} // if globe

	// Check if we are in goal
	if(board_position == 99 || board_position == 56)
    	{
       		input[6] = 1;
        	cout << "Toke in Goal!!!" << endl;

    	} // if goal

	// Check if we are in the Red Carpet
	if(board_position > 50 && board_position < 56)
    	{
        	input[5] = 1;
        	cout << "Token on the Red Carpet. Keep pushing!!" << endl;

	} // if Red Carpet
		
	
	// Check if we are forming a wall with other token and therfore, safe
	if(board_position != -1 && board_position != 99)
    	{
        	if(((board_position == pos_start_of_turn[0])  && (token != 0)) || ((board_position == pos_start_of_turn[1]) && (token != 1)) || ((board_position == pos_start_of_turn[2]) && (token != 2)) || (board_position == pos_start_of_turn[3]) && (token != 3)))
        	{
			// Activate 'Shield wall' input
           		input[4] = 1;

			// Turn off previous inputs to the Shield wall
	
			if(input[2] == 1 ) // star
            		{
                		input[2] = 0;
           		} // if 2

			if(input[3] == 1) // globe
			{
			            input[3] = 0;
			} // if 1

			if(input[5] == 1) // Red Carpet
			{
                		input[5] = 0;
            		} // if 4	
            
			cout << "¡¡¡¡ Shield Wall !!!! Board position: #" << board_position << endl;
		
		} // if board_position && !token

	} // if !=-1 && !=99

	// Check if we are just in a regular position
	if(input[0] != 1 && input[1] != 1 && input[2] != 1 && input[3] != 1 && input[4] != 1 && input[5] != 1)
    	{
        	cout << "On free spot. Board position: #" << board_position << endl;
        	input[6] = 1;
    
	} // if free spot



}// current_state()




// This function analizes the game after throwing the dice and calculates all the possible moves (actions) to be taken.
// Binary action-indicator
/*

	1. Home breakout	= 1 0 0 0 0 0 0 0 0 0 0 
	2. No possible move	= 0 1 0 0 0 0 0 0 0 0 0
	3. Regular move		= 0 0 1 0 0 0 0 0 0 0 0
	4. Move to star		= 0 0 0 1 0 0 0 0 0 0 0
	5. Move to globe	= 0 0 0 0 1 0 0 0 0 0 0 
	6. Form a Shield Wall	= 0 0 0 0 0 1 0 0 0 0 0
	7. Enter the Red Carpet	= 0 0 0 0 0 0 1 0 0 0 0
	8. Commit suicide	= 0 0 0 0 0 0 0 1 0 0 0
	9. Kill oponent		= 0 0 0 0 0 0 0 0 1 0 0
	10. Star to goal	= 0 0 0 0 0 0 0 0 0 1 0
	11. Make it to goal	= 0 0 0 0 0 0 0 0 0 0 1

*/
void Q_player::get_possible_moves(int input[11], int board_position, int dice, int token)
{
	
	int board_idx; // Index = board_postion + dice; Shows where can we go
	
	if(board_position != -1 || board_position >= 99) // At home or goal no possible action.             
	{
	
		board_idx = board_position + dice; 

	} // if home or goal

	// Condition to get the hell outta home
	if(dice == 6 && board_position == -1)
	{

		input[0] = 1;		

	}// if at home + 6

	// For all moves that include not being at home.   
	if(board_position != -1)
	{

        	// Make it to goal (binary 11)
        	if(board_idx == 56)
        	{
            		input[11] = 1;
          
        	} // if move to goal
       
		// Move to a star (binary 4)
		if(board_idx == 5 || board_idx == 18 || board_idx == 31 || board_idx == 44 || board_idx == 11 || board_idx == 24 || board_idx == 37)
        	{
            		input[3] = 1;
           
	        }// if move to star

		// Move to a globe (binary 5)
	        if(board_idx == 8 || board_idx == 13 || board_idx == 21 || board_idx == 26 || board_idx == 34 || board_idx == 39 || board_idx == 47)
	        {
			input[4] = 1;
            
	        }// if move to globe


	        // Make a Shield Wall with other token (binary 6)
	        if(((board_idx == pos_start_of_turn[0])  && (token != 0))|| ((board_idx == pos_start_of_turn[1]) && (token != 1)) || ((board_idx == pos_start_of_turn[2]) && (token != 2)) || ((board_idx == pos_start_of_turn[3]) && (token != 3)) && board_idx != 99)
	        {
	                input[5] = 1;

	        }// if shield wall

        	// Move to Goal using star (binary 10)
        	if(board_idx == 50)
        	{
            		input[10] = 1;
            		
	        }// if move star to goal

	        // Get into the Winner Road (binary 6)
	        if(board_idx > 50 && board_idx < 56)
	        {
			input[6] = 1;

	        }

        	// Kill an opponent (binary 9)
		// int i --> represents the tokens outta there
        	for(int i = 4 ; i < 16 ; i++ )
		{
            		if(board_idx == pos_start_of_turn[i]) // If the index indicates the same position as another token, then:
            		{
            			// Commit Suicide (move to opponent when it is on a globe) (binary 9)
                		if(board_idx == 8 || board_idx == 13 || board_idx == 21 || board_idx == 26 || board_idx == 34 || board_idx == 39 || board_idx == 47)
                		{
                			input[7] = 1;
                			if(input[4] == 1)
                    			{
                        			// Update array by removing array position corresponding to globe" << endl;
                        			input[4] = 0;
                    			}
                    			//cout << " Move to Globe Kamikaze" << endl;
                    			//cout << "player already on: " << i << endl;

                		}
	
	                	// Born to KILL
	                	else
	                	{
	 	        		input[8] = 1;
	
         				if(input[4] == 1)
                	    		{
                	        		//cout << "Remove Going to globe!" << endl;
                	        		input[4] = 0;
                	    		}// if

                	    		if(input[3] == 1)
                			{
                        			//cout << "star" << endl;
                        			input[3] = 0;
                    			}// if

                    			if(input[6] == 1)
                    			{
                        			//cout << "reset going to winnerRoad to killing someone one winner road" << endl;
                        			input[6] = 0;
                    			}// if

                    			if(input[11] == 1)
                    			{
                        			cout << "move to goal Not killing anyone" << endl;
                        			input[8] = 0;

                    			}// if 

                    			cout << "Move to Kill Opponent" << endl;

                		} // else (kill the dj)

            		}// if board_idx == pos_start_turn[i]


		        // Two opponent on the same square
        		if(input[5] != 1 || input[7] != 1) // Shield Wall || Suicide
        		{
            			for(int j = 1 ; j < 4 ; j++)
            			{
                			// Token 1 and 2
                			if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+1])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                 	   			{
                 	       				input[7] = 1;
                        				input[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+1 << endl;
                 	   			}// if

                			}// else if

                			// Token 1 and 3+
                			else if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+2])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				input[8] = 1; // suicide
                        				input[7] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+2 << endl;
                    				}// if

                			}// else if

                			// Token 1 and 4
                			else if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+3])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				input[7] = 1;
				                        input[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+3 << endl;
				                }// if 
                			
					}// else if
                
					// Token 2 and 3
			                else if(pos_start_of_turn[j*4+1] == pos_start_of_turn[j*4+2])
				        {
                   				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				input[7] = 1;
                        				input[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4+1 << "  " << j*4+2 << endl;
                    				}// if

                			}// else if

                			// Token 2 and 4
                			else if(pos_start_of_turn[j*4+1] == pos_start_of_turn[j*4+3])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				input[7] = 1;
                        				input[8] = 0;
                        				//cout << "Move to Two opponent on the same square: " << j*4+1 << "  " << j*4+3 << endl;
                    				}// if 
	
                			}// else if
                
					// Token 3 and 4
			                else if(pos_start_of_turn[j*4+2] == pos_start_of_turn[j*4+3])
			                {
						if(board_idx == pos_start_of_turn[j*4])
                				{
                				        input[7] = 1;
                				        input[8] = 0;
                				        //cout << "Move to Two opponent on the same square" << j*4+2 << "  " << j*4+3 << endl;
                				}// if

                			}// else if

            			}// for (two opponents)
	        	
			}// if - Two opponents

			// No possibility of special moves or not possible moving at all
        		if(input[0] == 0 && input[1] == 0 && input[3] == 0 && input[4] == 0 && input[5] == 0 && input[6] == 0 && input[7] == 0 && input[8] == 0)
       			{
				// No Special move found!
            			if(board_position != 99) // If I am not in goal
            			{
                			input[2] = 1;
                			// Just move
            			
				}// if
            
				else
            			{
					input[1] = 1;                			
					// Already in goal! -  no move possible!

            			}// else	

        		}// if - no special moves or no move at all
    		
		}// for (int i ---> rival tokens)

	}// if board_position != -1

}// get_possible_moves()





//---------------------------------------------------------------------------------------------------------------------

// This will require update
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



// This one I don't fucking know
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
