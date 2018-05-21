// Tools of AI - SDU Robotics
// Intelligent LUDO player using Q-Learning
// Carlos Viescas Huerta

/*
	Q-LEARNING ALGORITHM
	
		1. Split the game into discrete set of States & Actions.
		2. Create Q-Table.
		3. State representation.
		4. Define rewards.
		5. Update Q-values using the learning formula:
			delta_Q(s,a) = alpha * ( r + gamma * max[a1]Q(s1, a1) - Q(s,a) )
		6. Action Selection.


	STATE REPRESENTATION

	1. Home		 	--> 		state = 0
	2. On free spot		-->		state = 1
	3. On star		-->		state = 2
	4. On globe		-->		state = 3
	5. Shield wall		-->		state = 4
	6. Red carpet		-->		state = 5
	7. On goal 		-->		state = 6

	POSSIBLE MOVES REPRESENTATION

	1. Home breakout		-->		move = 1 
	2. No possible move		-->		move = 0
	3. Regular move			--> 		move = 2
	4. Move to star			-->		move = 3
	5. Move to globe	 	-->		move = 4
	6. Form a Shield Wall		-->		move = 5
	7. Enter the Victory Road	-->		move = 6
	8. Commit suicide		-->		move = 7
	9. Kill oponent			-->		move = 8
	10. Make it to goal		-->		move = 9

	BOARD POSITIONS

	Home: -1
	Stars: 5, 11, 18, 24, 31, 37, 44, 50 
	Globes: 0, 8, 13, 21, 26, 34, 39, 47
	Red Carpet: from 50 to 56
	Hitting 50 goes to goal directly
	Goal = 56 or 99
	
*/


//-----------------------------------------------------------------------------------------------------------------------------------------------------

#include "Q_player_fast.h"

using namespace std;
using namespace cv;

// Q_player object constructor.
Q_player_fast::Q_player_fast(): pos_start_of_turn(16), pos_end_of_turn(16), dice_roll(0)
{
	// Load and initialize Q-table
	acc = 0;
	srand(time(NULL));
	pre_state = 0;
	pre_move = 0;
	pre_token = 0;
	pre_position = -1;
	FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::READ);
	fs["Q_table"] >> Q_table;
	fs.release();
	// Display Q-table.
	cout << "Q-Learning Table (rows = " << Q_table.rows << ", columns = " << Q_table.cols << "):" << endl;
	cout << Q_table << endl;
	
}// Q_player()



// This function calculates the current state for each token the player has. Requires board spot number.
vector<int> Q_player_fast::state()
{
	int state = 0;
	vector<int> states;
	
	cout << "---- states ----" << endl;

	for( int z=0; z<4; z++)
	{
		int board_position = pos_start_of_turn[z];

		// Check if we are in home
    		if(board_position == -1)
    		{

        		state = 0;
        		cout << "Token #" << z << " in jail" << endl;
	
    		} // if -1
	
		// Check if we are in a star
		if(board_position == 5 || board_position == 11 || board_position == 18 || board_position == 24 || board_position == 31 || board_position == 37 || board_position == 44 || board_position == 50)
 	   	{
 		       	state = 2;
 		       	cout << "Token #" << z << " on Star. Board position: #" << board_position << endl;

 	   	} // if star

		// Check if we are a globe
		if(board_position == 8 || board_position == 13 || board_position == 21 || board_position == 26 || board_position == 34 || board_position == 39 || board_position == 47 || board_position == 0)
 	   	{
 		       	state = 3;
 		       	cout << "Token #" << z << " on globe. Board position: #" << board_position << endl;

 	   	} // if globe

		// Check if we are in goal
		if(board_position == 99 || board_position == 56)
    		{
       			state = 6;
        		cout << "Toke #" << z << " in Goal!!!" << endl;

    		} // if goal

		// Check if we are in the Red Carpet
		if(board_position > 50 && board_position < 56)
    		{
        		state = 5;
        		cout << "Token #" << z << " on the Red Carpet. Keep pushing!!" << endl;

		} // if Red Carpet
		
	
		// Check if we are forming a wall with other token and therfore, safe
		if(board_position != -1 && board_position != 99)
    		{
        		if(((board_position == pos_start_of_turn[0])  && (z != 0)) || ((board_position == pos_start_of_turn[1]) && (z != 1)) || ((board_position == pos_start_of_turn[2]) && (z != 2)) || ((board_position == pos_start_of_turn[3]) && (z != 3)))
        		{
				// Activate 'Shield wall' input
        	   		state = 4;

				if(state == 5) // Red Carpet
				{
        	        		state = 5;
        	    		} // if 4	
            
				cout << "Token #" << z << " ¡¡¡¡ Shield Wall !!!! Board position: #" << board_position << endl;
		
			} // if board_position && !token

		} // if !=-1 && !=99

		// Check if we are just in a regular position
		if(state != 0 && state != 2 && state != 3 && state != 4 && state != 5 && state != 6)
    		{
        		cout << "Token #" << z << " on free spot. Board position: #" << board_position << endl;
        		state = 1;
		} // if free spot

		// Check for errors
		if (state < 0 || state > 6)
		{
			cout << "An error ocurred when getting the state." << endl;		
			exit(0);
			
		}// if

		states.push_back(state);	
	
	}// for z
	cout << "---- states ----" << endl << endl;
	return states;

}// current_state()



// This function analizes the game after throwing the dice and calculates all the possible moves (actions) to be taken.
vector<int> Q_player_fast::get_possible_moves(int dice)
{
	
	int idx = -1; // Index = board_postion + dice; Shows where can we go
	int move = 0;
	vector<int> moves;
	
	cout << "---- moves ----" << endl;

	// Calculate moves for each token
	for (int i=0; i<4; i++)
	{
		int board_position = pos_start_of_turn[i];
		
		// no move
		if(board_position == -1 && dice != 6)
		{
			move = 0;
			cout << "Token #" << i << " trapped in jail with no possible move" << endl;
		}// if no move

		if(board_position == 99)
		{
			move = 0;
			cout << "Token #" << i << " is already in goal :)" << endl;
		}// if no move			

		// Jailbreak
		if(board_position == -1 && dice == 6)
		{
			move = 1;
			cout << "Token #" << i << " Jailbreak!!" << endl;
			
		}// if -1

		// if token is in game
		if(board_position != -1 && board_position != 99)
		{
			idx = board_position + dice;

			// Goal
			if(idx == 56)
			{
				move = 9;
				cout << "Token #" << i << " Moving to goal!!" << endl; 
			}// if goal 56

			if(idx == 50 && board_position >= 44 && board_position < 50)
			{
				move = 9;
				cout << "Token #" << i << " Moving to goal!!" << endl; 
			}// if goal 50

			// stars
			if(idx == 5 || idx == 11 || idx == 18 || idx == 24 || idx == 31 || idx == 37 || idx == 44)
			{	
				move = 3;
				cout << "Token #" << i << " Moving to star on position #" << idx << endl;
			}//if stars

			// shield wall
			for(int j=0; j<4; j++)
			{
				if(i != j && idx == pos_start_of_turn[j] && pos_start_of_turn[j] != 99)
				{
					move = 5; 
					cout << "Token #" << i << " SHIELDWALL with token #" << j << " in position #" << idx << endl;
					break;
				}// if 
			}// for

			bool killer = false;
			bool dumbass = false;
			
			// Meeting other player's token
			for( int h=4; h<16; h++)
			{
				// Check if there is someone in idx
				if( idx == pos_start_of_turn[h] )
				{
					// Check if idx is a globe (then suicide)
					if( idx == 8 || idx == 13 || idx == 21 || idx == 26 || idx == 34 || idx == 39 || idx == 47)
					{
						dumbass = true;	
						cout << "Token #" << i << " commited suicide in globe #" << idx << endl;
						break;
					} // if globe

					// Check if the oponent was in shieldwall
					for(int k=0; k<4; k++)
					{
						int tkn = int(h/4) * k;
						if( idx == pos_start_of_turn[tkn] && tkn != h)
						{
							dumbass = true;
							cout << "Token #" << i << " suicide in position #" << idx << endl;
						}// if shieldwall
						
					}// for
						
					killer = true;
					cout <<  "Token #" << i << " killing that son of a bitch #" << h << " in position #" << idx << endl;
					
				}// if meeting people
			}// for

			if(killer == true)
			{
				move = 8;
			}// if killer
			
			if(dumbass == true)
			{
				move = 7;
			}// if dumbass

			// Move to globe
			if( (idx == 8 || idx == 13 || idx == 21 || idx == 26 || idx == 34 || idx == 39 || idx == 47) && dumbass == false)
			{
				move = 4;	
				cout << "Token #" << i << " in globe #" << idx << endl;
			}// if globe

			// Victory Road
			if(idx > 50 && idx < 56)
			{
				move = 6;
				cout << "Token #" << i << " in the Victory Road #" << idx << endl; 
			}// if Victory

			
			if( move != 0 && move != 1 && move != 3 && move != 4 && move != 5 && move != 6 && move != 7 && move != 8 && move != 9 )
			{
				move = 2;
				cout << "Token #" << i << " just moving to position #" << idx << endl;
			}// regular move

		}// if token is in game

		if( move < 0 || move > 9)
		{
			cout << "An error ocurred when getting moves." << endl;
			exit(0);		
		}
			
		moves.push_back(move);

	}// for int i (tokens)

	cout << "---- moves ----" << endl << endl;

	return moves;

}// get_possible_moves()


// Function to assign rewards to the actions taken by the algorithm
double Q_player_fast::assign_reward(int previous_move, int previous_position, int current_position, int previous_state, int current)
{
	double reward = 0;
	static int games = 0;

	// Not Moved!!
	if(previous_position == current_position && previous_state == current)
	{
		reward -= 0.3;
	}// if not moved

	// Moved
	if(previous_position != current_position && previous_position < current_position)
	{
		reward += 0.25;
	}// moved

	// Jailbreak
	if( previous_move == 1 && previous_position == -1 && current_position == 0)
	{	
		reward += 0.3;
	}// if jailbreak

	// Getting a token in goal
	if( previous_move == 9 )
	{
		reward += 1.5;
	}

	// Kill bill
	if(previous_move == 8)
	{
		reward += 0.15;
	}// if kill

	// Suicide
	if(previous_move == 7)
	{
		reward -= 0.7;
	}// Suicide

	// Getting in the Victory Road
	if(previous_move == 6 && previous_position < 50 && current_position > 50)
	{
		reward += 0.12;
	}// if VR
	
	// Shieldwall
	if(previous_move == 5)
	{
		reward += 0.05;
	}// if shieldwall

	// Moving to star better than just moving
	if(previous_move == 3 && previous_state != 2 && current == 2)
	{
		reward += 0.05;
	}// if 

	// Move to globe better than just moving
	if(previous_move == 4 && previous_state != 3 && current == 3)
	{
		reward += 0.025;
	}// if 

	// Getting a token killed
	for(int i=0; i<4; i++)
	{
		if(pos_start_of_turn[i] == -1 && pos_end_of_turn[i] != -1)
		{
			reward -= 0.2;
		}
	}

	//---------------------------------------------------

	// Winning game
	bool winning = true;
	for(int j=0; j<4; j++)
	{
		if(pos_end_of_turn[j] != 99)
		{
			winning = false;
		}// if
	}// for j

	if(winning == true)
	{
		reward += 5;
		games++;
	}
	
	// Losing the game
	bool loosing_yellow = true;
	for(int h=4; h<8; h++)
	{
		if(pos_end_of_turn[h] != 99)
		{
			loosing_yellow = false;
		}// if
	}// for j
	
	bool loosing_bleu = true;
	
	for(int k=8; k<12; k++)
	{
		if(pos_end_of_turn[k] != 99)
		{
			loosing_bleu = false;
		}// if
	}// for j
	
	bool loosing_red = true;
	
	for(int l=12; l<16; l++)
	{
		if(pos_end_of_turn[l] != 99)
		{
			loosing_red = false;
		}// if
	}// for j
	
	if( loosing_yellow == true ||  loosing_bleu == true || loosing_red == true )
	{
		reward -= 3;
		games++;
	}

	return reward;

}// assign_reward()


// Update Q-table values
void Q_player_fast::update_Q_table(int previous_token, int previous_move, int previous_state, int current, int previous_position, int move)
{
	cout << endl << "UPDATE Q_TABLE" << endl << endl;

	double alfa = 0.7; // Learning rate € [0, 1]
    	double gamma = 0.45; // Discount factor € [0, 1]
	
	cout << "LEARNING_RATE = " << alfa << ";  DISCOUNT_FACTOR = " << gamma << endl;    

	int current_position = pos_start_of_turn[previous_token];

    	double reward = Q_player_fast::assign_reward(previous_move, previous_position, current_position, previous_state, current);
    		
	acc += reward; 
    	
   	cout << "Immediate Reward: " << reward << endl;
	cout << "Accumulated: "<< acc << endl;

	if( reward != 0)
	{
    		Q_table.at<float>(previous_state, previous_move) += alfa * (reward + gamma * Q_table.at<float>(current, move) - Q_table.at<float>(previous_state, previous_move));
		float update = Q_table.at<float>(previous_state, previous_move);
		cout << "Update ---> Q-Table(" << previous_state << ", " << previous_move << ")  ==  " <<  update << endl;
	}    	

	cout << endl << "Q-TABLE UPDATED" << endl << endl;

}// update_Q_table()


//---------------------------------------------------------------------------------------------------------------------


// Function that calls the routine and takes a decision
int Q_player_fast::make_decision()
{
	cout << endl << "MAKE DECISION" << endl;

	cout << "Update game-state." << endl;
	cout << "Dice roll (twist of fate): " << dice_roll << endl;
        cout << "previous Token moved: #" << pre_token << endl;
	cout << "Previous move: " << pre_move << endl;        
	cout << "Previous state: " << pre_state << endl;
	

	vector<int> states = Q_player_fast::state();
	vector<int> movements = Q_player_fast::get_possible_moves(dice_roll);

	cout << "Current state of previous token (#" << pre_token << "): " << states[pre_token] << endl;        

	Q_player_fast::update_Q_table(pre_token, pre_move, pre_state, states[pre_token], pre_position, movements[pre_token]);

        
        FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::WRITE);
        fs << "Q_table" << Q_table;
	fs.release();
        	
	cout << "Q-Table saved" << endl;
	
	for(int v = 0 ; v < 4 ; v++)
    	{
        	if(pos_start_of_turn[v] >=-1 && pos_start_of_turn[v] < 99) // Maybe the if should be changed with no end limit
        	{
            		cout << "Player token " << v << " with pos " << pos_start_of_turn[v] << endl;

            		// Action and state
        		
            		cout << "Token #" << v << endl;
            		cout << "Start position: " << pos_start_of_turn[v] << endl;
            		cout << "dice_roll: "<< dice_roll << endl;

            		cout << "Current state: " << states[v] << endl;	
            		cout << "Posible moves: " << movements[v] << endl;
        	}// if

    	}// for
	
	double epsilon = 0.1;
	pre_token = Q_player_fast::e_greedy_move_selection(epsilon, states, movements); 

	// Update previous parameters
	pre_state = states[pre_token]; 
	pre_move = movements[pre_token];
    	pre_position = pos_start_of_turn[pre_token];

	cout << endl << "DECISION MADE" << endl << endl;	
    
	return pre_token;

} // make_decision()


// E-Greedy: Choose randomly with probability E, otherwise greedy. Returns decision on which token to choose
int Q_player_fast::e_greedy_move_selection(double eps, vector<int> states, vector<int> moves)
{
	// tuple< token, action> 
	int token_decision = 0; 

	// Set e-greedy probability	
	double EXPLORE_RATE  = eps*100;
	double explore_activation = (double)(rand() % 1000) / 1000.0;

	cout << endl << "E-GREEDY SELECTION" << endl << endl;
	cout << "Exploration rate = " << EXPLORE_RATE << "%  |  Explotation rate =  " << 100-EXPLORE_RATE << "%" << endl;
	
	// Greedy
	if(explore_activation < EXPLORE_RATE)
	{
		cout << "Greedy decision" << endl;
		float max = -10000;
		
		// Check all tokens
		for(int i=0; i<4; i++)
		{
			if( (pos_start_of_turn[i] == -1 && dice_roll != 6) || (pos_start_of_turn[i] == 99 && pos_start_of_turn[i] >= 56) )
			{
				continue;
			} // if  

			float value = Q_table.at<float>(states[i], moves[i]);
			if(value > max && moves[i] != 0)
			{
				max = value;
				token_decision = i;
			}//if value
		}// for


	}// if greedy
	
	// Random move
	else
	{
		bool playing = false;
		for(int j=0; j<4; j++)
		{
			if(pos_start_of_turn[j] !=-1 && pos_start_of_turn[j] != 99 && pos_start_of_turn[j] < 56)
			{
				playing = true;
				break;
			}
		}// for

		while(true)
		{
			token_decision = rand() % 4;
			if( pos_start_of_turn[token_decision] != 99 && pos_start_of_turn[token_decision] < 56 )
			{
				if( pos_start_of_turn[token_decision] != -1 && playing == true)			
				{
					break;
				}// if playing
				else if(playing == false)
				{
					break;
				}// else if
			}// if

		}// while

	}// else

	while(pos_start_of_turn[token_decision] == 99)
	{
		token_decision++;
		token_decision = token_decision % 4;
	}// while

	cout << "Decision is token #" << token_decision << endl;
	cout << endl << "E-GREEDY SELECTION" << endl << endl;
	
	return token_decision;

}// e_greedy()


//----------------------------------------------------------------------------------------------------



void Q_player_fast::start_turn(positions_and_dice relative)
{
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = Q_player_fast::make_decision();
    emit select_piece(decision);

}// start_turn()



void Q_player_fast::post_game_analysis(std::vector<int> relative_pos)
{
    	pos_end_of_turn = relative_pos;
    	bool game_complete = true;
    	for(int i = 0; i < 4; ++i)
	{
        	if(pos_end_of_turn[i] < 99)
		{
        		game_complete = false;
        	}// if
    	}// for	

    emit turn_complete(game_complete);

}// post_game_analyis()
