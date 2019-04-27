CC = gcc
data = ./data

rtest : rtest.c hw1_c.h
	$(CC) -o rtest rtest.c hw1_c.h

dtest : dtest.c hw1_c.h
	$(CC) -o dtest dtest.c hw1_c.h

run_rtest : rtest
	./rtest < $(data)/1.txt
	./rtest < $(data)/2.txt
	./rtest < $(data)/3.txt
	./rtest < $(data)/4.txt


run_dtest : dtest
	./dtest < $(data)/1.txt
	./dtest < $(data)/2.txt
	./dtest < $(data)/3.txt
	./dtest < $(data)/4.txt

test : run_rtest run_dtest

clean :
	rm -rf rtest
	rm -rf dtest
