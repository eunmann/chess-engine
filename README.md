# Chess Engine C++20

The purpose of this project is to create a simple chess engine using C++20 standards. The core code is inspired
by [Stockfish](https://github.com/official-stockfish/Stockfish). More information about chess programming can be found
at the [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page).

# Implementation

## Interface

The [UCI](http://wbec-ridderkerk.nl/html/UCIProtocol.html) is implemented so the engine can work with GUI applications
like [Arena](http://www.playwitharena.de/).

## Board Representation

A board position is represented using a combination of `BitBoard` values. A `BitBoard` is just a `uint64_t`. Each bit is
a flag for one of the 64 squares on the board. That means a position can be represented with a `BitBoard` for each piece
type, and a `BitBoard` for each piece color. The advantage of this representation is the common operations for the
engine become bit operations, making them efficient computation and memory wise. For example, imagine if you wanted to
check if a bishop is attacking anything along a diagonal. If the board was represented as a 2d array, then you would
need to iterate through along that diagonal. Additionally, the smallest 2D board representation would
be `unsigned char[64]`, 64 bytes. The instructions to perform that iteration include increments for the index, loading
from memory, and storing the result into another array. With the `BitBoard` representation, a single `BitBoard` with the
proper bits set can be `&`'d with the positions `BitBoard`. The opreation being a single instruction and the output is a
8 byte value. To check if there was overlaps, that result value just needs to be compared to `0`, another quick
operation.

## Move Generation

All PSUEDO-legal moves are generated. The illegal moves are filtered during move searching. Pushing back the computation
to move search allows us to take advantage of the pruning at that step. We can avoid computing the legality of moves
that we wouldn't even consider.

## Cached Moves

Instead of computing possible moves for each board state during move searching, the moves for each piece are calculated
once on startup and stored in memory. For the king and knight, this is a simple task since the possible moves for a king
or knight have simple rules. For example, if the destination square is occupied by the same color, then it cannot move
there. For the king, another check to see if the square is being threatened by the opponent. So all of the PSUEDO-legal
moves can be computed and stored, and illegal moves can be filtered out easily. Pawns are also a simple case, but
require a check for diagonal captures and a check for occupancy of forward squares.

For bishops, rooks, and queens, it becomes a bit more complicated. The PSUEDO-legal moves can be generated, but since
these are sliding pieces (pieces that can move in a direction and stop due to a blocker), you neeed to account for the
position of the board while filtering out the illegal moves. To solve this
problem, [Magic Bit Boards](https://www.chessprogramming.org/Magic_Bitboards) are implemented.

To briefly summarize Magic Bit Boards, they are methods to precompute all possible moves for either a bishop or rook,
and then create a way to easily index into the move based on the square the piece is and the relevant blockers for that
square. Precomputing all of the positions and storing the entire positions would be impossible. That would mean storing
at least 2^64 positions for each square. Instead, we only focus on the relevant squares for each position and piece. For
example, take the square A1 for a bishop. It only has 7 possible moves, and only 6 of those are important for computing
whether the bishop can continue to a next square (the squares at the edge of the board always stop the bishop, so we can
ignore them at the moment). Now for the square on A1, we only need to compute 2^6=64 possible combinations of blocker
positions. Now after we generate every possible permutation of blocker positions, we need to figure out each possible
attack positions the bishop can move to. At this point we have 2 tables, all blocker positions for a bishop on A1, and
the corresponding bishops moves for each blocker position. Now, the magic comes in. We use brute force to find a single
magic number that allows us to create a index based on the blocker position. We check if this magic number (
a `uint64_t`) multiplied by *EVERY* blocker position, then shifted by `64-6` (to force the index into the range
of `[0,2^6=64)`) just so happens to map correctly into the bishop attack table. If that condition is met, we have found
our magic number for the A1 square, which allows us to create an index from just the blockers into the bishop attack
table. This needs to be done for every square on the board, so you end up with two tables of `uint64_t[64]` magic
numbers for the bishop and rook. A queen is just a bishop and rook combined, so it can use the bishop and rook tables to
save memory. The implemention for this engine is
the [Plain Magic Bit Boards](https://www.chessprogramming.org/Magic_Bitboards#Plain).

## Compile Time Execution

Since the computation of Cached Moves is the same on every start, the computation was moved to compile. In this way, the
startup is slightly faster and initialization is implicit. Rather than having `init()` functions that need to be called
at the start of the program.

## Move Search

The current implementation is a multi-thread alpha-beta pruning search. The searching is done on a separate thread to
allow I/O processing while searching.

## Testing

Testing is done througha custom framework named TestFW (Testing Framework). It was created specifically for this projec
to be light-weight and easy to use. It provides the most basic functionality, but only what is needed for this project.

# TODO

## Refactoring

* TFW Unit Tests need to use TFW_ASSERT
* Cleanup unused functions
* Use the LOG and LOG_ASSERT macros to make debugging easier
* Refactor tests to be in different files/directory

## Code

* Fix Bugs
* Unit Tests
    * Create more tests that validate famous games
    * Create tests to valid best moves in certain positions

## AI

* Book openings
* Better Heuristics
    * Double Pawns

## Performance

* Optimize Searching
    * Profile performance again

## Engine

* Accept clock time and obey it
* Print info strings for in-game info and also debugging strings
* Print line for best move

## Play Testing

* Get the engine playing on a website like [Lichess](https://lichess.org).

# Utility

## PGN Extract

[PGN-Extract](https://www.cs.kent.ac.uk/people/staff/djb/pgn-extract/) is a useful tool for turning PGN format into a
format more consumable for the engine's test.