build:
	rm -f program.exe
	g++ -g -std=c++11 -Wall main.cpp -o program.exe

run:
	./program.exe

valgrind:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes ./program.exe
