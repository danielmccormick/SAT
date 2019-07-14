# SAT

By Daniel McCormick

This is a fairly standard DPLL Sat Solver. 

It involves the basic features

* It has some amount of gentle error handling, so if it takes a DIMACs file it may fail gracefully
* It can do pure literal propogation, and does so at first pass. Since it's a linear time search, doing this a lot would probably be relatively expensive after the first pass. 
* It has unit clause propogation, and does so at every phase. This is relatively fast, except if an error is detected here, it throws an exception, unwinding the stack. That's a bit of a janky solution, but seems relatively ok.
* It generally is built to handle CNF formulas, but can also handle DIMACS DNF formulas. Since DNF can be found sat in linear time, there's not really any tricks to this one. 

TODO:

* Implement multithreaded DPLL solving - it seems like every branch taken could be replaced by a thread  
* Fix the build system - this is pretty wonky.
