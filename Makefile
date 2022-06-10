searcher:  main.o Searcher.o
	g++ main.o Searcher.o -o searcher


main.o: main.cpp
	g++ -c main.cpp

Searcher.o: Searcher.cpp
	g++ -c Searcher.cpp

clean:
	rm *.o searcher
