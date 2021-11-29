# Chess Engine C++20
The purpose of this project is to create a simple chess engine using C++20 standards. The core code is inspired by [Stockfish](https://github.com/official-stockfish/Stockfish). More information about chess programming can be found at the [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page).

# Implementation

## Interface
The [UCI](http://wbec-ridderkerk.nl/html/UCIProtocol.html) is implemented so the engine can work with GUI applications like [Arena](http://www.playwitharena.de/).

## Move Generation
All pseduo-legal moves are generated. The illegal moves are filtered during move searching. Pushing back the computation to move search allows us to take advantage of the pruning at that step. We can avoid computing the legality of moves that we wouldn't even consider. The only except is castling, the legality checks are done during move generation.

## Move Search
The current implementation is a single thread alpha-beta pruning search.

# TODO

## Code
* Fix Bugs
* Unit Tests
  * Create more tests that validate famous games
  * Create tests to valid best moves in certain positions

## AI
* Book openings

## Performance
* Might be able to swap if-statements with bit operations
* Need to profile what is taking so long
* Multithreaded search

## Engine
* Accept clock time and obey it
* Print info strings for in-game info and also debugging strings

## Utility
https://www.cs.kent.ac.uk/people/staff/djb/pgn-extract/