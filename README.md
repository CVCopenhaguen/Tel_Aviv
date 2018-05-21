# AI2 Final Project - Tools of Artificial Intelligence

Implementing an intelligent LUDO player using Q-learning. This project belongs to the course of _Tools of Artificial Intelligence_ at University of Southern Denmark.

## LUDO GUI:

Provided by Nikolaj Iversend and Niculai Lynnerup, March 18th 2016. To get the Game:

```sh
$ mkdir LUDO-AI2
$ cd LUDO-AI2
$ git clone git@gitlab.com:niive12/ludo-gui.git
```

## LUDO Project

#### 1. Structure:

	- dialog.cpp: Game design (default).
	- game.cpp: Controls game structure and GUI updates (default).
	- main.cpp: Run the game (default).
	- ludo\_player.cpp: Example of class initialization to build a new player (default).
	- ludo\_player-random.cpp: Random player to test against (default).
	- Q\_player.cpp: Intelligent player implemented with Q-Learning (implemented). 

#### 2. Build & Compile

If new implemented classes (like adding _Q\_player.cpp_, modify the _ludo.pro_ file to look like this:

```
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ludo
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    game.cpp \
    ludo_player.cpp \
    ludo_player_random.cpp \
    Q_player.cpp

HEADERS  += dialog.h \
    game.h \
    ludo_player.h \
    positions_and_dice.h \
    ludo_player_random.h \
    Q_player.h

FORMS    += dialog.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused
```

Then, compile it with the following commands:

```sh
$ cd <project_directory>
$ mkdir -p build
$ cd build
$ qmake ../ludo/ludo.pro
$ make 
```
