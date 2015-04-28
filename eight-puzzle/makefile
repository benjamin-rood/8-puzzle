Main.exe	: Main.o graphics.o board_obj.o algorithm.o
	g++ -std=c++11 -Wl,-s -o Main.exe Main.o graphics.o board_obj.o algorithm.o -l gdi32 -static-libgcc -static-libstdc++
			
Main.o	: Main.cpp graphics.h board_obj.h algorithm.h
	g++ -std=c++11 -c -fpermissive -fconserve-space -Wno-write-strings Main.cpp
	
board_obj.o		: board_obj.cpp board_obj.h
	g++ -std=c++11 -c -fpermissive -fconserve-space -Wno-write-strings  board_obj.cpp
	
algorithm.o	: algorithm.cpp algorithm.h 
	g++ -std=c++11 -c -fpermissive -fconserve-space -Wno-write-strings algorithm.cpp

graphics.o  : graphics.cpp graphics.h
	g++ -std=c++11 -c -fpermissive -fconserve-space -Wno-write-strings graphics.cpp
	
clean:
	del *.o
	del *.exe
	

