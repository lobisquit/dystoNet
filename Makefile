PROGRAM=src/program.out
TEST-PROGRAM=test/program.out

all: build run clean

build:
	@ g++ src/*.cpp -o $(PROGRAM)

run:
	@ $(PROGRAM)

clean:
	@ rm -f $(PROGRAM)
	@ rm -f $(TEST-PROGRAM)

tests:
	@ g++ -std=c++0x test/*.cpp -o $(TEST-PROGRAM)
	@ $(TEST-PROGRAM)
