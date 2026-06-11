all: compile

compile:
	g++ main.cpp src/*.cpp -o game -lSDL3 -Iinclude

clean:
	rm game
