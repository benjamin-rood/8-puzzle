Main.out	: Main.o board_obj.o algorithm.o
	g++ -std=c++11 -Wl -o Main.out Main.o board_obj.o algorithm.o
			
Main.o	: Main.cpp board_obj.h algorithm.h 
	g++ -std=c++11 -c -fpermissive -Wno-write-strings Main.cpp
	
board_obj.o		: board_obj.cpp board_obj.h
	g++ -std=c++11 -c -fpermissive -Wno-write-strings  board_obj.cpp
	
algorithm.o	: algorithm.cpp algorithm.h 
	g++ -std=c++11 -c -fpermissive -Wno-write-strings algorithm.cpp
	
clean:
	del *.o
	del *.exe
	

