Hello,
Both one threaded and multi threaded programs are done

However multi threaded has some bugs.
Currently known bugs: 
1) Some times it doesnt find the data entry - might be due to some kind of corruption of memory in consumer or the program finishes early
2) Some times this happens: 
free(): double free detected in tcache 2
Aborted (core dumped)
I tried checking it with valgrind and it didnt find anything.
I suspect that my "garbage collector" leaves a dangling pointer somewhere, but I cant find it.
Then when i clear everything it tries to clean it 2 times.


