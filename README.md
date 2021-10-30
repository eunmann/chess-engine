# Chess Engine C++20
The purpose of this project is to create a simple chess engine using C++20 standards. The core code is inspired by [Stockfish](https://github.com/official-stockfish/Stockfish). More information about chess programming can be found at the [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page).

# Implementation

## Interface
The [UCI](http://wbec-ridderkerk.nl/html/UCIProtocol.html) is implemented so the engine can work with GUI applications like [Arena](http://www.playwitharena.de/).

## Move Generation
All pseduo-legal moves are generated. The illegal moves are tossed during move searching. Pushing back the computation to move search allows us to take advantage of the pruning at that step. We can avoid computing the legality of moves that we wouldn't even consider.

## Move Search
The current implementation is a single thread alpha-beta pruning search.

# TODO

## Code
* Probably need to refactor data structures and code
  * Use templates
  * Create an array of psuedo legal moves
  * Creates a Moves structure
* Unit Tests
  * I could probably use text files to define input/output and compare

## AI
* Book openings

## Performance
* Maybe setup bitboards as a structure of arrays?
  * SIMD for generating moves?
  * SIMD for computing heuristics?
* Might be able to swap if-statements with bit operations
* GPU
  * What would be useful here?
    * Generating moves?
    * Computing the heuristic?
* Need to profile what is taking so long
* Multithreaded search

## Engine
* Accept clock time and obey it
* Print info strings for in-game info and also debugging strings
