boro
====

Boro is a simple program for playing amazons. This is something I spent a few days working on in 2010 while at the ICGA Computer Olympiad, where I was competing with Rondo, my chess engine. I saw the final match of the Amazons tournament, and heard that the winning program used UCT, a pretty simple algorithm I was familiar with.

So I wrote a very basic engine using monte carlo/UCT. It has zero domain-specific logic, and really no intelligence in its UCT implementation (no priors, dumb light playouts). It only really uses a command line interface.

The name is something I just made up since it was unnamed up to this point. It comes from "bow/arrow".

There's no makefile apparently, so here's a gcc command line:

    cc -std=c99 -lm -o boro *.c
