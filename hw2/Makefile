input.txt: test_gen.c
	gcc -o gen test_gen.c
	rm -rf input.txt
	touch input.txt
	./gen > input.txt

run: hw2_c.h main.c
	gcc -g -o run main.c hw2_c.h

shell: run
	./run

random: run input.txt
	./run < input.txt
	rm input.txt gen

all: shell

clean:
	rm -f run gen
