SRC=src/main.cpp src/node_value.cpp src/node_operator.cpp src/node_result.cpp src/calculator_simple.cpp src/utils.cpp src/mdp.cpp src/node_column.cpp src/calculator_complex.cpp src/operator.cpp
INC=inc/node_base.h inc/node_operator.h inc/node_result.h inc/calculator_simple.h inc/utils.h inc/mdp.h inc/node_column.h inc/calculator_complex.h inc/operator.h
FLAG=-std=c++11 -Iinc

.PNONY=all clean

all: main
	@./main

clean:
	rm main

main: $(SRC) $(INC)
	g++ -o main $(SRC) $(FLAG)