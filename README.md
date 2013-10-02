Random.cpp
----------
Outputs _n_ random numbers. Tries out a few different things: binding a random number generator to a distribution to create a `rand()` analog, overloading `operator*` to accept a number and a function object to enable Ruby-like "n-times" expressions, use of new `std::stoX` functions.

Stopwatch.cpp
-------------
Implements a terminal stopwatch. Untested on Windows. Uses new C++11 threads to achieve asynchrous IO. Also demonstrates how to enable unbuffered input for POSIX terminals and Windows consoles, as well as properly cleaning up after yourself.

The parent thread monitors unbuffered input for the ctrl-c sequence, and then signals death. The child thread continuously updates a timer. When the parent thread receives any other character, it signals the child thread to output a newline. Through the signalling, there is no tearing in output.

Countdown.cpp
-------------
Ugly copy and paste of Stopwatch. Wanted a countdown timer a while back, so I reused `Stopwatch.cpp` and made it count backwards. Demonstrates how to mangle a date/time into a C++11 `std::time_point`.
