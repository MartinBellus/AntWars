# AntWars

Proboj is an activity from KSP camp, where players create programs (bots) that
compete in a game.

This repository contains implementation of one such game.

## Setting

The game takes place in a world of ants, where each player controls a separate
ant colony. The goal of the game is to eliminate all other players and take
control of the whole map. The ants battle each other, gather resources to
create new ants, and destroy enemy hills.

## How to start programming?

Before you start, you should probably read the [rules](RULES.md).

Every bot lives in a separate directory. The `players` directory contains
templates for popular languages (currently only `C++`), so feel free to copy
the template and start. If you want to create a bot in a different language,
you can refer to the *Implementation details* section in the [rules](RULES.md).

### C++ template

A `C++` template consists of the `common` directory, which contains all the
necessary files for communication with the server, and the `bot.cpp` file,
which is the main file of the bot. To create a `C++` bot, you need to implement
two functions (`init` and `get_turn`) of the class `Bot` in this file. The
`init` function is called once before the game starts and can be used to
initialize data structures. Before each turn, the `get_turn` function is called
with the current game state as an argument. The output of this function is a
set of commands that your ants will execute.

You don’t need to read the files in the `common` directory; the only file of
interest there is `data.h`, which contains data structures used to store the
game state for your bot during the turn.

After you are done, you can compile your bot by simply running `make` in the
bot's directory. This will create executable `player`, which can be then run on
the server.

## Running the game

After creating and compiling your bot, it’s time to run the game. If you
haven’t compiled the server yet, do so by running `make` in the `server`
directory. This will create a `srv` binary in the root of this repository.

Then, start the game using `./srv <game_config> <player_config>`. This will
simulate the game and create a directory (the default is `game`) with
information about the game. It contains `recording.txt`, which is used to
replay the game in the observer, `scores.txt` with end game scores, and the
`logs` directory. This directory contains logs of all players and the server.
Everything that your bot prints to `stderr` will be saved here.

> [!IMPORTANT] Do not write anything to `stdout`, as it is used by the server
> to communicate with your bot. Doing so will interfere with communication and
> cause undefined behavior for your bot.

### Configuration

The game is configured using two files: `game_config` and `player_config`.
Lines in `game_config` are in the format `<KEY> <value>`, and lines in
`player_config` are in the format `<name> <path_to_directory> <optional_args>`.
Empty lines and lines starting with `#` are ignored. For a complete set of
`game_config` parameters, refer to the default [game_config](game_config.txt)
file.

#### Maps

Currently, the server supports map files only in ASCII `.ppm` format. Every
pixel can have one of three colors, each representing a different tile type:

- White `0xFFFFFF` - Empty tile
- Red `0xFF00000` - Hill
- Blue `0x0000FF` - Water tile

You can create custom maps using any software that can export images in `.ppm`
format, just be sure to use ASCII mode.

## Using observer

The observer is a utility program written in Python that can replay games
played on the server based on a recording file.

To run the observer, execute the command `python3 observer/observer.py
<game_directory>`. This will open a Tkinter window with the replay. You can
control the replay using the buttons below the canvas.
