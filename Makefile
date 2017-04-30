PROGRAM=src/program.out
TEST-PROGRAM=test/program.out

# @ before commands prevents print of commands to console
all: build run clean

build:
	@ g++ src/*.cpp -o $(PROGRAM)

run:
	@ $(PROGRAM)

clean:
	@ rm -f $(PROGRAM)
	@ rm -f $(TEST-PROGRAM)

tests:
	@ g++ test/*.cpp -o $(TEST-PROGRAM)
	@ $(TEST-PROGRAM)
