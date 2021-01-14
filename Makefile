.PHONY: all clean

all:
	gcc --std=c17 -O3 -Wall -Wpedantic -Iinclude/ src/*.c src/visitors/*.c -o jadec

clean:
	rm -f jadec
