// Tools of AI - SDU Robotics
// Intelligent LUDO player using Q-Learning
// Carlos Viescas Huerta


#include "Q_player.h"

using namespace std;
using namespace cv;

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
	acc_reward_player1 = 0;
    	acc_reward_player2 = 0;
    	acc_reward_player3 = 0;
    	acc_reward_player4 = 0;

	FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::READ);
	fs["Q_table"] >> Q_table;
	fs.release();
	// Display Q-table.
	cout << "Q-Learning Table (rows = " << Q_table.rows << ", columns = " << Q_table.cols << "):" << endl;
	cout << Q_table << endl;
	
	//update = false;	
	
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
int Q_player::state_representation(int states[7])
{
    if(states[0]  == 1)
    {
        cout << "In Home" << endl;
        return 0;
    }
    else if(states[1]  == 1)
    {
        cout << "On free spot" << endl;
        return 1;
    }
    else if(states[2]  == 1)
    {
        cout << "On star" << endl;
        return 2;
    }
    else if(states[3]  == 1)
    {
        cout << "On globe" << endl;
        return 3;
    }
    else if(states[4]  == 1)
    {
        cout << "Forming a Shield Wall" << endl;
        return 4;
    }
    else if(states[5]  == 1)
    {
        cout << "On the Red carpet" << endl;
        return 5;
    }
    else if(states[6]  == 1)
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
void Q_player::current_state(int states[7], int board_position, int token)
{
	// Check if we are in home
    	if(board_position == -1)
    	{

        	states[0] = 1;
        	cout << "Token in home" << endl;

    	} // if -1

	// Check if we are in a star
	if(board_position == 5 || board_position == 18 || board_position == 31 || board_position == 44 || board_position == 11 || board_position == 24 || board_position == 37 || board_position == 50 )
    	{
        	states[2] = 1;
        	cout << "Token on Star. Board position: #" << board_position << endl;

    	} // if star

	// Check if we are a globe
	if(board_position == 8 || board_position == 13 || board_position == 21 || board_position == 26 || board_position == 34 || board_position == 39 || board_position == 47 || board_position == 0)
    	{
        	states[3] = 1;
        	cout << "Token on globe. Board position: #" << board_position << endl;

    	} // if globe

	// Check if we are in goal
	if(board_position == 99 || board_position == 56)
    	{
       		states[6] = 1;
        	cout << "Toke in Goal!!!" << endl;

    	} // if goal

	// Check if we are in the Red Carpet
	if(board_position > 50 && board_position < 56)
    	{
        	states[5] = 1;
        	cout << "Token on the Red Carpet. Keep pushing!!" << endl;

	} // if Red Carpet
		
	
	// Check if we are forming a wall with other token and therfore, safe
	if(board_position != -1 && board_position != 99)
    	{
        	if(((board_position == pos_start_of_turn[0])  && (token != 0)) || ((board_position == pos_start_of_turn[1]) && (token != 1)) || ((board_position == pos_start_of_turn[2]) && (token != 2)) || ((board_position == pos_start_of_turn[3]) && (token != 3)))
        	{
			// Activate 'Shield wall' input
           		states[4] = 1;

			// Turn off previous inputs to the Shield wall
	
			if(states[2] == 1 ) // star
            		{
                		states[2] = 0;
           		} // if 2

			if(states[3] == 1) // globe
			{
			            states[3] = 0;
			} // if 1

			if(states[5] == 1) // Red Carpet
			{
                		states[5] = 0;
            		} // if 4	
            
			cout << "¡¡¡¡ Shield Wall !!!! Board position: #" << board_position << endl;
		
		} // if board_position && !token

	} // if !=-1 && !=99

	// Check if we are just in a regular position
	if(states[0] != 1 && states[1] != 1 && states[2] != 1 && states[3] != 1 && states[4] != 1 && states[5] != 1)
    	{
        	cout << "On free spot. Board position: #" << board_position << endl;
        	states[6] = 1;
    
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
void Q_player::get_possible_moves(int actions[11], int board_position, int dice, int token)
{
	
	int board_idx; // Index = board_postion + dice; Shows where can we go
	
	if(board_position != -1 || board_position >= 99) // At home or goal no possible action.             
	{
	
		board_idx = board_position + dice; 

	} // if home or goal

	// Condition to get the hell outta home
	if(dice == 6 && board_position == -1)
	{

		actions[0] = 1;		

	}// if at home + 6

	// No possible move
	if((board_position == -1 && dice !=6) || board_position == 99)
	{
		actions[1] = 1;

	}// No move

	// For all moves that include not being at home.   
	if(board_position != -1)
	{

        	// Make it to goal (binary 11)
        	if(board_idx == 56)
        	{
            		actions[10] = 1;
          
        	} // if move to goal
       
		// Move to a star (binary 4)
		if(board_idx == 5 || board_idx == 18 || board_idx == 31 || board_idx == 44 || board_idx == 11 || board_idx == 24 || board_idx == 37)
        	{
            		actions[3] = 1;
           
	        }// if move to star

		// Move to a globe (binary 5)
	        if(board_idx == 8 || board_idx == 13 || board_idx == 21 || board_idx == 26 || board_idx == 34 || board_idx == 39 || board_idx == 47)
	        {
			actions[4] = 1;
            
	        }// if move to globe


	        // Make a Shield Wall with other token (binary 6)
	        if( ( (board_idx == pos_start_of_turn[0])  && (token != 0) ) || ( (board_idx == pos_start_of_turn[1]) && (token != 1) ) || ( (board_idx == pos_start_of_turn[2]) && (token != 2) ) || ( (board_idx == pos_start_of_turn[3]) && (token != 3) ) && board_idx != 99)
	        {
	                actions[5] = 1;

	        }// if shield wall

        	// Move to Goal using star (binary 10)
        	if(board_idx == 50)
        	{
            		actions[9] = 1;
            		
	        }// if move star to goal

	        // Get into the Red Carpet (binary 6)
	        if(board_idx > 50 && board_idx < 56)
	        {
			actions[6] = 1;
			if (actions[5] == 1)
			{
				actions[5] = 0;
			}
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
                			actions[7] = 1;
                			if(actions[4] == 1)
                    			{
                        			// Update array by removing array position corresponding to globe" << endl;
                        			actions[4] = 0;
                    			}
		
					if(actions[10] == 1)
                    			{
                        			// Update array by removing array position corresponding to globe" << endl;
                        			actions[7] = 0;
                    			}
					if(actions[9] == 1)
                    			{
                        			// Update array by removing array position corresponding to globe" << endl;
                        			actions[7] = 0;
                    			}
                    			//cout << " Move to Globe Kamikaze" << endl;
                    			//cout << "player already on: " << i << endl;

                		}
	
	                	// Born to KILL
	                	else
	                	{
	 	        		actions[8] = 1;

					if(actions[2] == 1)
                	    		{
                	        		//cout << "Remove regular move!" << endl;
                	        		actions[2] = 0;
					}
	
         				if(actions[4] == 1)
                	    		{
                	        		//cout << "Remove Going to globe!" << endl;
                	        		actions[4] = 0;
                	    		}// if

                	    		if(actions[3] == 1)
                			{
                        			//cout << "star" << endl;
                        			actions[3] = 0;
                    			}// if

                    			if(actions[6] == 1)
                    			{
                        			//cout << "reset going to winnerRoad to killing someone on winner road" << endl;
                        			actions[6] = 0;
                    			}// if

                    			if(actions[9] == 1)
                    			{
                        			cout << "move to goal Not killing anyone" << endl;
                        			actions[8] = 0;

                    			}// if 
				
					if(actions[10] == 1)
                    			{
                        			cout << "move to goal Not killing anyone" << endl;
                        			actions[8] = 0;

                    			}// if 


                    			cout << "Move to Kill Opponent" << endl;

                		} // else (kill the dj)

            		}// if board_idx == pos_start_turn[i]


		        // Two opponent on the same square
        		if(actions[5] != 1 || actions[7] != 1) // Shield Wall || Suicide
        		{
            			for(int j = 1 ; j < 4 ; j++)
            			{
                			// Token 1 and 2
                			if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+1])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                 	   			{
                 	       				actions[7] = 1;
                        				actions[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+1 << endl;
                 	   			}// if

                			}// else if

                			// Token 1 and 3+
                			else if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+2])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				actions[8] = 1; // suicide
                        				actions[7] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+2 << endl;
                    				}// if

                			}// else if

                			// Token 1 and 4
                			else if(pos_start_of_turn[j*4] == pos_start_of_turn[j*4+3])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				actions[7] = 1;
				                        actions[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4 << "  " << j*4+3 << endl;
				                }// if 
                			
					}// else if
                
					// Token 2 and 3
			                else if(pos_start_of_turn[j*4+1] == pos_start_of_turn[j*4+2])
				        {
                   				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				actions[7] = 1;
                        				actions[8] = 0;
                        				//cout << "Move to Two opponent on the same square" << j*4+1 << "  " << j*4+2 << endl;
                    				}// if

                			}// else if

                			// Token 2 and 4
                			else if(pos_start_of_turn[j*4+1] == pos_start_of_turn[j*4+3])
                			{
                    				if(board_idx == pos_start_of_turn[j*4])
                    				{
                        				actions[7] = 1;
                        				actions[8] = 0;
                        				//cout << "Move to Two opponent on the same square: " << j*4+1 << "  " << j*4+3 << endl;
                    				}// if 
	
                			}// else if
                
					// Token 3 and 4
			                else if(pos_start_of_turn[j*4+2] == pos_start_of_turn[j*4+3])
			                {
						if(board_idx == pos_start_of_turn[j*4])
                				{
                				        actions[7] = 1;
                				        actions[8] = 0;
                				        //cout << "Move to Two opponent on the same square" << j*4+2 << "  " << j*4+3 << endl;
                				}// if

                			}// else if

            			}// for (two opponents)
				if(actions[2] == 1)
                    		{
                        		// Update array by removing array position corresponding to globe" << endl;
                        		actions[2] = 0;
                    		}
				if(actions[4] == 1)
                    		{
                        		// Update array by removing array position corresponding to globe" << endl;
                        		actions[4] = 0;
                    		}
		
				if(actions[10] == 1)
                    		{
                        		// Update array by removing array position corresponding to globe" << endl;
                        		actions[7] = 0;
                    		}
				if(actions[9] == 1)
                    		{
                        		// Update array by removing array position corresponding to globe" << endl;
                        		actions[7] = 0;
                    		}	        	


			}// if - Two opponents

			// No possibility of special moves or not possible moving at all
        		if(actions[0] == 0 && actions[1] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
       			{
				// No Special move found!
            			if(board_position != 99) // If I am not in goal
            			{
                			actions[2] = 1;
                			// Just move
            			
				}// if
            
			
        		}// if - no special moves or no move at all
    		
		}// for (int i ---> rival tokens)

	}// if board_position != -1

}// get_possible_moves()


// Funtion that reads the state of the game  
vector<tuple<int, int, int>> Q_player::play_maker(int states[7], int actions[11], int token)
{
	std::vector<std::tuple<int,int,int>> ludo;

	// TOKEN IN JAIL
    	if(states[0]  == 1)
   	 {
        	cout << "We are now in Jail" << endl;
        	int state = 0;
        	std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for

    	}// if states[0] = 1


	// FREE SPOT
    	if(states[1]  == 1)
    	{
        	cout << "We are now in  free spot" << endl;
        	int state = 1;
        	std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for
    	
	}// if states[1] == 1


	// ON STAR
    	if(states[2]  == 1)
    	{
        	cout << "We are now on a star" << endl;
        	int state = 2;
        	std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for
    
	}// if states[2] == 1

	// ON GLOBE
    	if(states[3]  == 1)
    	{
        	cout << "We are now on a globe." << endl;
        	int state = 3;
        	std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for

    	}// if state[3] == 1

	// SHIELDWALL
    	if(states[4]  == 1)
    	{
        	cout << "We are now on SHIELDWALL!!" << endl;
        	int state = 4;
        	std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for

    	}// if state [4] == 1


	// RED CARPET
    	if(states[5]  == 1)
    	{
        	cout << "We are now on the Red Carpet" << endl;
        	int state = 5;
        std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for
    	
	}// states[5] == 1


	// GOAL
    	if(states[6]  == 1)
    	{
    		cout << "We are in goal" << endl;
    		int state = 6;
    		    std::vector<int> actions_counter;
	
        	if(actions[0] == 1 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(0);
           	 	cout << "JAILBREAK!" << endl;
        	}// if 10000000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 1)
        	{
            		actions_counter.push_back(10);
            		cout << "Goal!" << endl;
        	}// else if 00000000001

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 1 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(3);
            		cout << "Star!" << endl;
        	}// else if 00010000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 1 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(4);
           		 cout << "Globe!" << endl;
        	}// else if 00001000000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 1 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(5);
            		cout << "SHIELDWALL!!" << endl;
        	}// else if 00000100000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 1 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(6);
            		cout << "Red Carpet!" << endl;
        	}// else if 00000010000

       		else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 1 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
        		actions_counter.push_back(7);
            		cout << "Opponent in Globe, you are heading to your death sentence dumbass..." << endl;
        	}// else if 00000001000

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 1 && actions[9] == 0 && actions[10] == 0)
        	{
            		actions_counter.push_back(8);
            		cout << "Take the bastard down!" << endl;
        	}// else if 00000000100

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 1 && actions[10] == 0)
        	{
            		actions_counter.push_back(9);
            		cout << "Get to goal using a star!" << endl;
        	}// else if 00000000010

        	else if(actions[0] == 0 && actions[1] == 0 && actions[2] == 1 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0)
        	{
           		actions_counter.push_back(2);
           		cout << "Regular move!" << endl;
        	}// else if 00100000000

        	else if(actions[0] == 0 && actions[1] == 1 && actions[2] == 0 && actions[3] == 0 && actions[4] == 0 && actions[5] == 0 && actions[6] == 0 && actions[7] == 0 && actions[8] == 0 && actions[9] == 0 && actions[10] == 0 )
        	{
			actions_counter.push_back(1);
        		cout << "Cannot move!" << endl;
        	}// else 01000000000


        	cout << "Number of Actions for given state: " << actions_counter.size() << endl;
        	for(unsigned int i = 0 ; i < actions_counter.size(); i++)
       		{
        		int action = actions_counter[i];
        		ludo.push_back(make_tuple(token, state, action));

        	}// for
    
	}// if states[6] == 1
    

	return ludo;

}// play_maker()



// Update Q_table values
void Q_player::update_Q_table(tuple<int,int,int,int> player_state_action_i)
{
	double alfa = 0.5; // Learning rate € [0, 1]
    	double gamma = 0.1; // Gamma € [0, 1]
	
	cout << "Learning rate = " << alfa << ";  Gamma = " << gamma << endl;    

	int player_played_i = get<0>(player_state_action_i);
    	int previous_state = get<1>(player_state_action_i);
    	int performed_action = get<2>(player_state_action_i);
    	int previous_position = get<3>(player_state_action_i);
    	int current_position = pos_start_of_turn[player_played_i];
    
	cout << "previous_position: "<< previous_position << endl;
    	cout << "current_position: "<< current_position << endl;    	

    	int states_now[7];

    	Q_player::current_state(states_now, current_position, player_played_i);
    	int current = state_representation(states_now);


    	//int current_player_state = current[0];
    	double reward = current_position; // should be based on everyones position...
    
	//double reward = (current_position_0 + current_position_1 + current_position_2 + current_position_3) * 10;
    	if(previous_position == current_position && previous_state == current)
    	{
        	cout << "Not moved!" << endl;
        	reward += -0.5; //
    	}

    	
	if((current_position == 99 || current_position == 56) && previous_position != current_position)
    	{
        	cout << "you are in goal!" << endl;
        	reward += 99;
    	}
    
	if(current_position == 0 && previous_position == -1 && performed_action == 0)
    	{
        	cout << "Added to enforce moving tokens out as a good thing" << endl;
        	reward += 0.5;
    	}
    		
	acc += reward;
    	cout <<"Accumulated: "<< acc << endl;

    	cout << "Reward for #0 " << pos_start_of_turn[0]*10 << " #1: " << pos_start_of_turn[1]*10 <<" #2: "  << pos_start_of_turn[2]*10 <<" #3: " << pos_start_of_turn[3]*10 << " Player Played: "<< 	player_played_i << endl;
    	
    	cout << "current position: " << current_position << endl;
   	cout << "Immediate Reward: " << reward << endl;

    	double max = -10000000000000;
    	for(int i = 0; i< 11 ;  i++)
    	{
        	//cout << current_player_state << " " << i << endl;
        	double test = Q_table.at<float>(current, i);
        	if(test > max)
        	{
        		max  = i;
        	}
    	}

   	cout << "Previous state: " << previous_state  << endl;
    	cout << "Action taken: " << performed_action << endl;
    	
    	Q_table.at<float>(previous_state, performed_action) += alfa * (reward + gamma * max - Q_table.at<float>(previous_state, performed_action));
    	

}// update_Q_table()


//---------------------------------------------------------------------------------------------------------------------


// This will require update
int Q_player::make_decision()
{
	// Save Q-table
	//if (update == true)
	//{
    		cout << "peform update" << endl;
        	cout << "previous player played: " << get<0>(player_state_action_previous_position) << endl;
        	cout << "Previous state: " << get<1>(player_state_action_previous_position) << " and action: " << std::get<2>(player_state_action_previous_position) << endl;
        
		update_Q_table(player_state_action_previous_position);
       		player_state_action.clear();

        
        	FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::WRITE);
        	fs << "Q_table" << Q_table;
		fs.release();
        	
		cout << "table saved" << endl;
		//update == false;
	//} /// if

	player_state_action.clear();

	for(int i = 0 ; i < 4 ; i++)
    	{
        	if(pos_start_of_turn[i]>=-1 && pos_start_of_turn[i] < 99) // Maybe the if should be changed with no end limit
        	{
            		cout << "Player token " << i << " with pos " << pos_start_of_turn[i] << endl;

            		int input_actions[11] = {0,0,0,0,0,0,0,0,0,0,0};
            		int input_states[7] = {0,0,0,0,0,0,0};
	
           
            		Q_player::get_possible_moves(input_actions, pos_start_of_turn[i], dice_roll, i);
            		Q_player::current_state(input_states, pos_start_of_turn[i], i);

            		cout << "Token #" << i << endl;
            		cout << "Start position: " << pos_start_of_turn[i] << endl;
            		cout << "dice_roll: "<< dice_roll << endl;

            		cout <<"Actions possible: " << 	input_actions[0] <<
                        				input_actions[1] << input_actions[2] <<
                               				input_actions[3] << input_actions[4] <<
                                			input_actions[5] << input_actions[6] <<
                                			input_actions[7] << input_actions[8] << 
							input_actions[9] << input_actions[10] << endl;

            		cout << "Current state: " << input_states[0] << input_states[1]
                        		          << input_states[2] << input_states[2]
                                      		  << input_states[3] << input_states[4]
                                      		  << input_states[5] << input_states[6] << endl;

            		vector<tuple<int,int,int>> token_i = Q_player::play_maker(input_states, input_actions, i);
            		player_state_action.insert(player_state_action.end(), token_i.begin(), token_i.end());
        	}// if

        	cout << endl;
    	}// for
	
	//update == false;
	cout << "Size after all tokens: "<< player_state_action.size() << endl;

    	if(player_state_action.size() > 4)
    	{
        	cout << "something wrong!!" << endl;
        	exit(0);
    	}
    	
	cout << "Gonna make decision" << endl;

	double epsilon = 0.1;
	player_state_action_previous_position = e_greedy(epsilon); // 70% explotation, 30% exploration.

    	cout << "Player: " << player_played << " In state: " << get<1>(player_state_action_previous_position) << " Peforms action: " << get<2>(player_state_action_previous_position) << endl;

    	if(pos_start_of_turn[player_played]+dice_roll == 56)
    	{
        	cout << "Player is in goal" << endl;
        	cout << "In goal" << endl;
    	}
    
	return player_played;

} // make_decision()


// E-Greedy: Choose randomly with probability E, otherwise greedy.
tuple<int, int, int, int> Q_player::e_greedy(double eps)
{
	// Set e-greedy probability	
	double limit  = eps*100;

	cout << "Exploration rate = " << limit << "%  |  Explotation rate =  " << 100-limit << "%" << endl;
	
    	random_device rd;
    	mt19937 mt(rd());
    	uniform_int_distribution<int> dist(1 , 100);

   	// Random action
	if(dist(mt)< limit)
	{
        	int firstPlayer = get<0>(player_state_action[0]);
	       	int lastPlayer = get<0>(player_state_action[player_state_action.size() -1]);
	       	cout << "Random decision" << endl;
        
		random_device player_r;
	        mt19937 player_mt(player_r());
	        uniform_int_distribution<int> dist_p(firstPlayer, lastPlayer);

	        int player = dist_p(player_mt);
	        player_played = player;

	        int previous_position = pos_start_of_turn[player_played];
	        auto it = find_if(player_state_action.begin(), player_state_action.end(), [](const tuple<int,int,int>& e) {return get<0>(e) == 0;});
	        int position = distance(player_state_action.begin(),it);
	        cout << "position: " << position << endl;

	        int state = get<1>(player_state_action[position]);
	        int action = get<2>((player_state_action[position]));

	        for(unsigned int i = 0 ; i < player_state_action.size(); i++)
	        {
	            int player = get<0>(player_state_action[i]);
	            int state = get<1>(player_state_action[i]);
	            int action = get<2>(player_state_action[i]);
	            double test = Q_table.at<float>(state, action);
	            cout << test << " ";
	        }// for
		
		cout << endl;
	        cout << "Random player: " << player << " state: " << state << " Action: " << action << endl;

	        return make_tuple(player, state, action, previous_position);
	}// if

	// Greedy learned action
	else
	{	
		cout << "Greedy decision" << endl;
        	//Return max
        	double max = -1000000;
        	int position;
        	for(unsigned int i = 0 ; i < player_state_action.size(); i++)
        	{
        		int player = get<0>(player_state_action[i]);
        		int state = get<1>(player_state_action[i]);
        		int action = get<2>(player_state_action[i]);
        		double test = Q_table.at<float>(state, action);
        		cout << test;

        		if(test > max)
            		{
                		max = test;
                		position = i;
                		player_played = player;
            		}// if
 	               cout << endl;
        	}// for
        
		int previous_position = pos_start_of_turn[player_played];
	        int state = get<1>(player_state_action[position]);
	        int action = get<2>(player_state_action[position]);

	        return make_tuple(player_played, state, action, previous_position);

	}// else


}// e_greedy()


//----------------------------------------------------------------------------------------------------

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
