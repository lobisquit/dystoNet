PROGRAM=src/program.out
TEST-PROGRAM=test/program.out

all: build run

build:
	@ g++ -Wall -std=c++0x src/*.cpp -o $(PROGRAM)

run:
	@ $(PROGRAM)

clean:
	@ rm -f $(PROGRAM)
	@ rm -f $(TEST-PROGRAM)

tests:
	@ g++ -Wall -std=c++0x test/*.cpp -o $(TEST-PROGRAM)
	@ $(TEST-PROGRAM)

docs:
	doxygen doc/Doxyfile
