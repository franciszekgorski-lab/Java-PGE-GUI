all: graph

graph: algorithm.c graph.c main.c headers/algorithm.h headers/graph.h
	gcc -Wall --pedantic algorithm.c graph.c main.c -o graph -lm

test_runner: tests/test.c
	gcc -Wall --pedantic tests/test.c -o tests/test_runner -lm

test: graph test_runner
	cd tests && ./test_runner

clean-test:
	rm -rf tests/out/*