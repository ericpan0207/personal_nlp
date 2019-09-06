journal_analysis: journal_analysis.cpp
	g++ -std=c++17 -Wall -g -o journal_analysis journal_analysis.cpp -lstdc++fs

all: journal_analysis ;

clean:
	rm -f journal_analysis journal_analysis.o
