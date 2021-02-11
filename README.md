# cyberpawn
A chess engine personal project

## Architecture thoughts
There is a VS project called cyberpawn. This is the core engine itself. It is just a statically linkable library.

There is a VS solution called cyberpawnService. This will be a windows service of some sort that will take requests and return results (e.g. here's a game position, give me the best 10 moves, etc.)

Maybe I'll make a node server that serves a webpage and lets you play chess against the engine. It will run on a machine with the cyberpawn service and make calls to it for moves.


