input.txt: test_gen.c
	gcc -o gen test_gen.c
	rm -rf input.txt
	touch input.txt
	./gen > input.txt

run: hw2_c.h main.c
	gcc -g -o run main.c hw2_c.h

verbose: input.txt
	gcc -g -DVERBOSE -o run main.c hw2_c.h

shell: run
	./run
	rm ./run

random: run input.txt
	./run < input.txt
	rm -rf input.txt gen

debug: run
	gdb ./run
	rm ./run

all: shell

clean:
	rm -f run gen
