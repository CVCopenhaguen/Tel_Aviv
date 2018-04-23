# AI2 Final Project - Tools of Artificial Intelligence

Implementing a LUDO player using AI tools.

## LUDO GUI:

Implemented by Nikolaj Iversend and Niculai Lynnerup, March 18th 2016. Building:

```
$ mkdir LUDO-AI2
$ cd LUDO-AI2
$ git clone git@gitlab.com:niive12/ludo-gui.git
$ mkdir -p build
$ cd build
$ qmake ../Ludo
$ make 
```

## LUDO Project

#### 1. Structure:

	- dialog.cpp: Game design (default).
	- game.cpp: Controls game structure and GUI updates (default).
	- main.cpp: Run the game (default).
	- ludo-player.cpp: Example of class initialization to build a new player (default).
	- ludo-player-random.cpp: Random player to test against (default).
	- Q-player.cpp: Intelligent player implemented with Q-Learning (implemented). 
