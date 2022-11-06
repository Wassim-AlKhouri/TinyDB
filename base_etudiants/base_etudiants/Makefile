
FLAGS=-std=c++17 -Wall -Werror -Wpedantic -D_GNU_SOURCE
COMPILER=g++

main: main.cpp student.o parsing.o db.o query.o utils.o child_functions.o
	${COMPILER} -o tinydb main.cpp parsing.o student.o db.o query.o utils.o child_functions.o ${FLAGS}

run:
	make main && ./tinydb

parsing.o: parsing.cpp parsing.h student.o
	${COMPILER} -c parsing.cpp ${FLAGS}

student.o: student.cpp student.h
	${COMPILER} -c student.cpp ${FLAGS}

query.o: query.cpp query.h
	${COMPILER} -c query.cpp ${FLAGS}

db.o: db.cpp db.h
	${COMPILER} -c db.cpp ${FLAGS}
	
utils.o: utils.cpp utils.h
	${COMPILER} -c utils.cpp ${FLAGS}
	
child_functions.o: child_functions.cpp child_functions.h
	${COMPILER} -c child_functions.cpp ${FLAGS}
	
tests: tests/run_tests.py
	./tests/run_tests.py

clean:
	rm logs/*
