build:
	#g++ -Wall -Werror -std=c++14 main.cpp -o minerva-creating-a-server
	g++ -Wall -std=c++14 main.cpp -o minerva-creating-a-server

run: build
	./minerva-creating-a-server

