all: compile link

compile:
	g++ -I src/include -c main.cpp sort.cpp helper.cpp
link:
	g++ main.o sort.o helper.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system